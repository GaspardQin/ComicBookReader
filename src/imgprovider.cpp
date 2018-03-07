#include "../include/imgprovider.h"

ImgProvider::ImgProvider()
    : QQuickImageProvider(QQuickImageProvider::Image, QQmlImageProviderBase::ForceAsynchronousImageLoading )
{

    is_path_set = false; //initially no image to show
    g_is_path_changed = false;
    g_page_num_total = 1;


    //for parallel preloading

    preload_thread = std::thread(&PreLoadWorker::parallelLoadPage, &preload_worker);


    cache.setMaxCost(50); // set cache's capacity, can save 100 images;
    //emit setPreloadPageNumTotal(page_num_total);


}

ImgProvider::~ImgProvider(){

    g_is_exit = true;

    //free cache
    cache.clear();
    g_preload_cv.notify_one();
    preload_thread.join();

}

void ImgProvider::setRootObject(QObject *ptr){
    root_object_ptr = ptr;
    QObject* slidbar_ptr = root_object_ptr->findChild<QObject*>("SlideBar");
    slidbar_ptr->setProperty("to", g_page_num_total);
}

QImage ImgProvider::cvMatToQImage(const cv::Mat *cv_image_ptr) {
    return QImage((uchar*)cv_image_ptr->data, cv_image_ptr->cols, cv_image_ptr->rows, cv_image_ptr->step, QImage::Format_RGB888);
}

QImage ImgProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
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


    if (g_is_preload_run == true)
        g_is_page_current_changed = true;


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

void ImgProvider::preloadImage(const int page_num){

    g_preload_params.page_num_current = page_num;
    g_preload_params.page_preload_left_size = 5;
    g_preload_params.page_preload_right_size = 5;
    g_preload_params.page_type = page_type;

    g_preload_cv.notify_one();

}

void ImgProvider::handlePathChange() {
    image_processor.loadArchive(g_archive_path);
    g_page_num_total = image_processor.getPageNumTotal();
    QObject* slidbar_ptr = root_object_ptr->findChild<QObject*>("SlideBar");
    slidbar_ptr->setProperty("to", g_page_num_total);
    cache_lock.lockForWrite();
    cache.clear();
    cache_lock.unlock();
    page_type = 2;
    is_path_set = true;


    //tell showImage and showImage2 to request new image
    QObject* show_image_ptr = root_object_ptr->findChild<QObject*>("ShowImage");
    QObject* show_image2_ptr = root_object_ptr->findChild<QObject*>("ShowImage2");
    show_image_ptr->setProperty("pageNum", 1);
    show_image_ptr->setProperty("trigger", !(show_image_ptr->property("trigger").toInt()));
    if(show_image2_ptr->property("isShow").toBool() == true)
        show_image2_ptr->setProperty("trigger", !(show_image2_ptr->property("trigger").toInt()));
}

void ImgProvider::filePathSlot(const QString &p) {
#ifdef _WIN64
    QStringList pieces = p.split("///");
#endif
#ifdef __linux__
    QStringList pieces = p.split("//");
#endif
    g_archive_path = pieces[1].toStdString();
    g_is_path_changed = true;
    handlePathChange();
}
