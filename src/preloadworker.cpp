#include "../include/preloadworker.h"


PreLoadWorker::PreLoadWorker(){
    g_is_page_current_changed = false;
    g_is_preload_run = false;
    //parallelLoadPage();
}

void PreLoadWorker::loadAndCacheImage(const int page_num, const int page_type){
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

void PreLoadWorker::parallelLoadPage() {//using extern preload_params
    while (1) {

        std::unique_lock <std::mutex> lck(g_preload_mutex);
        g_preload_cv.wait(lck); //wait for the preload signal

        if(g_is_exit == true) return;
        if (g_is_path_changed == true) {
            image_processor.loadArchive(g_archive_path);
            g_is_path_changed = false;
        }

        g_is_preload_run = true;

        bool left_exceed = false;
        bool right_exceed = false;
        int page_type = g_preload_params.page_type;
        int page_num_current = g_preload_params.page_num_current;
        int page_preload_left_size = g_preload_params.page_preload_left_size;
        int page_preload_right_size = g_preload_params.page_preload_right_size;
        for (int i = 1; i <= qMax<int>(page_preload_left_size, page_preload_right_size); i++) {


            if (page_num_current + i <= g_page_num_total && i <= page_preload_right_size) {
                loadAndCacheImage(page_num_current + i, page_type);
            }
            else right_exceed = true;

            if (page_num_current - i >= 1 && i <= page_preload_left_size) {
                loadAndCacheImage(page_num_current - i, page_type);
            }
            else left_exceed = true;

            if (left_exceed == true && right_exceed == true) break;
            if (g_is_page_current_changed == true || g_is_path_changed == true) {
                //need to rerun this function from new params
                if(g_is_page_current_changed == true) g_is_page_current_changed = false;
                break;

            }
        }
        g_is_preload_run = false;
    }
    return;


}
