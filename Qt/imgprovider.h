#ifndef IMGPROVIDER_H
#define IMGPROVIDER_H
#include <QQuickImageProvider>
#include <iostream>
#include "preloadworker.h"
#include <QCache>
#include <QThread>
#include "cache.h"
#include "image_process.h"
#include <opencv2/opencv.hpp>

class ImgProvider :  public QObject, public QQuickImageProvider
{
    Q_OBJECT
	
    QThread preloadThread;
public:

    ImgProvider()
        : QQuickImageProvider(QQuickImageProvider::Image, QQmlImageProviderBase::ForceAsynchronousImageLoading )
    {
		is_path_set = false; //initially no image to show
		is_path_changed = false; 
		page_num_total = 1;
        // for debug
		//image_processor.loadArchive(debug_path);
		//page_num_total = image_processor.getPageNumTotal();

        //for parallel reloading

        PreLoadWorker *preload_worker = new PreLoadWorker();
        preload_worker->moveToThread(&preloadThread);
        connect(&preloadThread,&QThread::finished, preload_worker, &QObject::deleteLater);
        connect(this, &ImgProvider::preloadSignals, preload_worker, &PreLoadWorker::parallelLoadPage);
        connect(this, &ImgProvider::setPreloadPageNumTotal, preload_worker, &PreLoadWorker::setPageNumTotal);
        connect(this, &ImgProvider::pageChanged, preload_worker, &PreLoadWorker::pageChanged);
		connect(this, &ImgProvider::setPreloadPath, preload_worker, &PreLoadWorker::setPath);
        preloadThread.start();
        preloadThread.setPriority(QThread::LowPriority); //set preloading thread's priority to low.
        cache.setMaxCost(100); // set cache's capacity, can save 100 images;
        //emit setPreloadPageNumTotal(page_num_total);


    }
    ~ImgProvider(){
        preloadThread.quit();
        preloadThread.wait();

        //free cache
        cache.clear();
	

    }
    void setRootObject(QObject* ptr){
        root_object_ptr = ptr;
        QObject* slidbar_ptr = root_object_ptr->findChild<QObject*>("SlideBar");
        slidbar_ptr->setProperty("to", page_num_total);
    }
	QImage cvMatToQImage(const cv::Mat *cv_image_ptr) {
		return QImage((uchar*)cv_image_ptr->data, cv_image_ptr->cols, cv_image_ptr->rows, cv_image_ptr->step, QImage::Format_RGB888);
	}

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize)
    {

		if (is_path_set == false) return QImage();
        QStringList input_list = id.split('/');
        ImageData* image_data_ptr;
        int current_page = input_list[0].toInt();

        page_type = input_list[1].toInt();
        if(page_type != 0 && page_type !=1 && page_type != 2){
            // need more specific exception process
            std::cout<<"error text mode"<<std::endl;
            return QImage() ; //fake return
        }

        ImagePreloadParams test_change_params(current_page,0,0,page_type);
        emit pageChanged(test_change_params);


        //check if already exists
        cache_lock.lockForRead();
        if(cache.contains(current_page)){
            image_data_ptr = cache.object(current_page);
            if(image_data_ptr->page_type == page_type){

                 if(size){
					 cv::Size sz = image_data_ptr->cv_image_ptr->size();
                     size->setWidth(sz.width);
                     size->setHeight(sz.height);
                 }
                cache_lock.unlock();

				// Send task to preloadWorker
				preloadImage(current_page);
                
				return cvMatToQImage(image_data_ptr->cv_image_ptr);
            }
			else
			{
				//delete the wrong cache with wrong type
				cache_lock.unlock();
				cache_lock.lockForWrite();
				cache.remove(current_page);
				cache_lock.unlock();
			}
        }
        else cache_lock.unlock();



        //if not
        image_data_ptr = new ImageData;
		image_processor.getImage(current_page, page_type, *(image_data_ptr->cv_image_ptr));
        //save to the cache
        image_data_ptr->page_type = page_type;
        cache_lock.lockForWrite();
        cache.insert(current_page,image_data_ptr);
        cache_lock.unlock();
		

         if(size){
			 cv::Size sz = image_data_ptr->cv_image_ptr->size();
			 size->setWidth(sz.width);
			 size->setHeight(sz.height);
         }


        // Send task to preloadWorker
        preloadImage(current_page);

        return cvMatToQImage(image_data_ptr->cv_image_ptr);
    }
    void preloadImage(const int page_num){
        ImagePreloadParams params;
        params.page_num_current = page_num;
        params.page_preload_left_size = 5;
        params.page_preload_right_size = 10;
        params.page_type = page_type;

        emit preloadSignals(params);


    }



	void handlePathChange() {
		image_processor.loadArchive(path);
		page_num_total = image_processor.getPageNumTotal();
		emit setPreloadPageNumTotal(page_num_total);
		QObject* slidbar_ptr = root_object_ptr->findChild<QObject*>("SlideBar");
		slidbar_ptr->setProperty("to", page_num_total);
		cache_lock.lockForWrite();
		cache.clear();
		cache_lock.unlock();
		page_type = 2;
		is_path_set = true;
		

		//tell showImage and showImage2 to request new image
		QObject* show_image_ptr = root_object_ptr->findChild<QObject*>("ShowImage");
		QObject* show_image2_ptr = root_object_ptr->findChild<QObject*>("ShowImage2");
		show_image_ptr->setProperty("trigger", !(show_image_ptr->property("trigger").toInt()));
		if(show_image2_ptr->property("isShow").toBool() == true)
			show_image2_ptr->setProperty("trigger", !(show_image2_ptr->property("trigger").toInt()));
	}
public slots:
	void filePathSlot(const QString &p) {
		QStringList pieces = p.split("///");
		path = pieces[1].toStdString();
		emit setPreloadPath(pieces[1]);
		handlePathChange();
	}
signals:
    void preloadSignals(const ImagePreloadParams &);
    void setPreloadPageNumTotal(const int &);
    void pageChanged(const ImagePreloadParams &);
	void setPreloadPath(const QString&);
private:
	std::string path;
    ImageProcess image_processor;
   // QString path_debug = "C:/Users/qw595/Documents/GitHub/ComicBookReader/TestSamples/OnePiece/";
	std::string debug_path = "C:/Users/qw595/Documents/GitHub/ComicBookReader/TestSamples/Injustice2.cbr";
    int page_num_total = 1; //init value, cannot set 0 because of the calculation in slidebar.
    QObject *root_object_ptr;
    int page_type = 0; //by default, page_type set to image type. 0 image type, 1 text image
	bool is_path_set;
	bool is_path_changed;

};

#endif // IMGPROVIDER_H
