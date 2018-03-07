#ifndef PRELOADWORKER_H
#define PRELOADWORKER_H

#include <QImage>
#include "cache.h"
#include <opencv2/opencv.hpp>
#ifdef  _WIN64
#include "image_process.h"
#endif
#ifdef __linux__
#include "../include/image_process.h"
#endif
extern bool g_is_page_current_changed;
extern bool g_is_path_changed;
extern bool g_is_preload_run;
extern int g_page_num_total;
extern std::string g_archive_path;
extern ImagePreloadParams g_preload_params;
extern std::mutex g_preload_mutex;
extern std::condition_variable g_preload_cv;
extern bool g_is_exit;
class PreLoadWorker
{
public:
    PreLoadWorker();

    void loadAndCacheImage(const int page_num, const int page_type);


    void parallelLoadPage();



private:
	ImageProcess image_processor;
	
    
};

#endif // PRELOADWORKER_H
