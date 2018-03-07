#ifndef IMGPROVIDER_H
#define IMGPROVIDER_H
#include <QQuickImageProvider>
#include <iostream>
#include "preloadworker.h"
#include <QCache>
#include "cache.h"
#ifdef  _WIN64
#include "image_process.h"
#endif
#ifdef __linux__
#include "../include/image_process.h"
#endif
#include <opencv2/opencv.hpp>
#include <functional>
class ImgProvider :  public QObject, public QQuickImageProvider
{
    Q_OBJECT
	
public:

    ImgProvider();
    ~ImgProvider();
    void setRootObject(QObject* ptr);
    QImage cvMatToQImage(const cv::Mat *cv_image_ptr);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
    void preloadImage(const int page_num);



    void handlePathChange();
public slots:
    void filePathSlot(const QString &p);

private:

    ImageProcess image_processor;

    QObject *root_object_ptr;
    int page_type = 0; //by default, page_type set to image type. 0 image type, 1 text image
	bool is_path_set;
	std::thread preload_thread;
	PreLoadWorker preload_worker;
};

#endif // IMGPROVIDER_H
