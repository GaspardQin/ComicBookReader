#ifndef PRELOADWORKER_H
#define PRELOADWORKER_H

#include <QObject>
#include <QImage>
#include <cache.h>
#include <QAtomicInt>
class PreLoadWorker : public QObject
{
    Q_OBJECT
public:
    PreLoadWorker(){
         page_current_changed.store(0);
    }

    bool debugLoadImage(const int page_num, QImage* load_image){

        QString image_path = path_debug;
        image_path += QString::number(page_num); //page number
        image_path += ".png";
        *load_image =  QImage(image_path);
        return true;
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
        debugLoadImage(page_num, &(image_data_ptr->image));
        image_data_ptr->page_type = page_type;
        cache_lock.lockForWrite();
        cache.insert(page_num,image_data_ptr);
        cache_lock.unlock();
    }

private:
    QString path_debug = "C:/Users/qw595/Documents/GitHub/ComicBookReader/TestSamples/OnePiece/";

    int page_num_total;
    QAtomicInt page_current_changed;
    ImagePreloadParams new_params;

public slots:
    void parallelLoadPage(const ImagePreloadParams &params){

        bool left_exceed = false;
        bool right_exceed = false;
        int page_type = params.page_type;
        int page_num_current = params.page_num_current;

        for(int i = 1 ; i<= qMax<int>(params.page_preload_left_size,params.page_preload_right_size); i++){
            if(page_num_current - i >= 1 && i <= page_preload_left_size){
                loadAndCacheImage(page_num_current - i, page_type);
            }
            else left_exceed = true;

            if(page_num_current + i <= page_num_total && i <= page_preload_right_size){
                loadAndCacheImage(page_num_current + i, page_type);
            }
            else right_exceed = true;

            if(left_exceed == true && right_exceed == true) break;
            if(page_current_changed.fetchAndStoreRelaxed(0) == 1){
                //need to rerun this function from new params
                if(new_params.page_num_current != page_num_current || new_params.page_type != page_type){
                    //page_current_changed.store(0);
                    return;
                }
            }
        }


    }

    void setPageNumTotal(const int t){
        page_num_total = t;
    }
    void pageChanged(const ImagePreloadParams &res){
        new_params = res;
        page_current_changed.store(1);

    }
};

#endif // PRELOADWORKER_H
