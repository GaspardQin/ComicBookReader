#ifndef IMGPROVIDER_H
#define IMGPROVIDER_H
#include <QQuickImageProvider>
#include <iostream>
#include "preloadworker.h"
#include <QCache>
#include <QThread>
#include "cache.h"

class ImgProvider :  public QObject, public QQuickImageProvider
{
    Q_OBJECT
    QThread preloadThread;
public:

    ImgProvider()
        : QQuickImageProvider(QQuickImageProvider::Image, QQmlImageProviderBase::ForceAsynchronousImageLoading )
    {

        setPageNumTotal(10); // for debug


        //for parallel reloading

        PreLoadWorker *preload_worker = new PreLoadWorker;
        preload_worker->moveToThread(&preloadThread);
        connect(&preloadThread,&QThread::finished, preload_worker, &QObject::deleteLater);
        connect(this, &ImgProvider::preloadSignals, preload_worker, &PreLoadWorker::parallelLoadPage);
        connect(this, &ImgProvider::setPreloadPageNumTotal, preload_worker, &PreLoadWorker::setPageNumTotal);
        connect(this, &ImgProvider::pageChanged, preload_worker, &PreLoadWorker::pageChanged);
        preloadThread.start();
        preloadThread.setPriority(QThread::LowPriority); //set preloading thread's priority to low.
        cache.setMaxCost(100); // set cache's capacity, can save 100 images;
        emit setPreloadPageNumTotal(page_num_total);


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


    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize)
    {


        QStringList input_list = id.split('/');
        ImageData* image_data_ptr;
        int current_page = input_list[0].toInt();

        page_type = input_list[1].toInt();
        if(page_type != 0 && page_type !=1){
            // need more specific exception process
            std::cout<<"error text mode"<<std::endl;
            return image_data_ptr->image; //fake return
        }

        ImagePreloadParams test_change_params(current_page,0,0,page_type);
        emit pageChanged(test_change_params);


        //check if already exists
        cache_lock.lockForRead();
        if(cache.contains(current_page)){
            image_data_ptr = cache.object(current_page);
            if(image_data_ptr->page_type == page_type){

                // QSize show_size(image.width(), image.height());
                 if(size){
                     size->setWidth(image_data_ptr->image.width());
                     size->setHeight(image_data_ptr->image.height());
                 }
                cache_lock.unlock();
                return image_data_ptr->image;
            }
        }
        else cache_lock.unlock();



        //if not
        image_data_ptr = new ImageData;
        debugLoadImage(current_page,&(image_data_ptr->image));
        //save to the cache
        image_data_ptr->page_type = page_type;
        cache_lock.lockForWrite();
        cache.insert(current_page,image_data_ptr);
        cache_lock.unlock();

        // QSize show_size(image.width(), image.height());
         if(size){
             size->setWidth(image_data_ptr->image.width());
             size->setHeight(image_data_ptr->image.height());
         }


        // Send task to preloadWorker
        preloadImage(current_page);

        return image_data_ptr->image;
    }
    void preloadImage(const int page_num){
        ImagePreloadParams params;
        params.page_num_current = page_num;
        params.page_preload_left_size = 5;
        params.page_preload_right_size = 10;
        params.page_type = 0;

        emit preloadSignals(params);


    }
    bool debugLoadImage(const int page_num, QImage* load_image){

        QString image_path = path_debug;
        image_path += QString::number(page_num); //page number
        image_path += ".png";
        *load_image =  QImage(image_path);
        return true;
    }

    void setPageNumTotal(int value){
        page_num_total = value;
    }

public slots:

signals:
    void preloadSignals(const ImagePreloadParams &);
    void setPreloadPageNumTotal(const int &);
    void pageChanged(const ImagePreloadParams &);
private:
    QString path_debug = "C:/Users/qw595/Documents/GitHub/ComicBookReader/TestSamples/OnePiece/";
    int page_num_total = 1; //init value, cannot set 0 because of the calculation in slidebar.
    QObject *root_object_ptr;
    int page_type = 0; //by default, page_type set to image type. 0 image type, 1 text image


};

#endif // IMGPROVIDER_H
