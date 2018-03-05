#ifndef PRELOADWORKER_H
#define PRELOADWORKER_H

#include <QObject>
#include <QImage>
#include <cache.h>
#include <QAtomicInt>
#include <opencv2/opencv.hpp>
#include "image_process.h"
class PreLoadWorker : public QObject
{
    Q_OBJECT
public:
    PreLoadWorker(){
		 is_page_current_changed = false;
		 is_path_changed = false;
		 //archive_path = path;
         //for debug
         //image_processor.loadArchive(path);
    }

    void loadAndCacheImage(const int page_num, const int page_type){
        //check if it already exists
        ImageData* image_data_ptr;
        cache_lock.lockForRead();
        if(cache.contains(page_num)){
            image_data_ptr = cache.object(page_num);
            if(image_data_ptr->page_type == page_type){
                cache_lock.unlock();
                return ;
            }
        }
        cache_lock.unlock();

        //if it dosent exist
        image_data_ptr = new ImageData;
		image_processor.getImage(page_num, page_type, *(image_data_ptr->cv_image_ptr));
        image_data_ptr->page_type = page_type;
        cache_lock.lockForWrite();
        cache.insert(page_num,image_data_ptr);
        cache_lock.unlock();
    }

private:
	std::string archive_path;
    ImageProcess image_processor;
    int page_num_total;
    bool is_page_current_changed;
	bool is_run;
    ImagePreloadParams new_params;
	bool is_path_changed;

public slots:
    void parallelLoadPage(const ImagePreloadParams &params){
		is_run = true;
		is_path_changed = false;
        bool left_exceed = false;
        bool right_exceed = false;
        int page_type = params.page_type;
        int page_num_current = params.page_num_current;
        int page_preload_left_size = params.page_preload_left_size;
        int page_preload_right_size = params.page_preload_right_size;
        for(int i = 1 ; i<= qMax<int>(page_preload_left_size,page_preload_right_size); i++){


            if(page_num_current + i <= page_num_total && i <= page_preload_right_size){
                loadAndCacheImage(page_num_current + i, page_type);
            }
            else right_exceed = true;

			if (page_num_current - i >= 1 && i <= page_preload_left_size) {
				loadAndCacheImage(page_num_current - i, page_type);
			}
			else left_exceed = true;

            if(left_exceed == true && right_exceed == true) break;
            if(is_page_current_changed == true || is_path_changed == true){
                //need to rerun this function from new params 
				is_run = false;
                return;
                
            }
        }

		is_run = false;

    }

    void setPageNumTotal(const int t){
        page_num_total = t;
    }
    void pageChanged(){
		if (is_run == true)
			is_page_current_changed = true;
		std::cout << "page changed" << std::endl;
    }
	void setPath(const QString path) {

		if (is_run == true)
			is_path_changed = true;
		archive_path = path.toStdString();
		image_processor.loadArchive(archive_path);
		
	}
};

#endif // PRELOADWORKER_H
