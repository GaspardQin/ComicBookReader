#include <QGuiApplication>
#include <QQmlApplicationEngine>
//#include "showimage.h"
#include "imgprovider.h"
#include "cache.h"


QCache<int,ImageData> cache;
QReadWriteLock cache_lock;


int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    //qmlRegisterType<ShowImage>("ComicBookReader.ShowImage", 1, 0, "ShowImage");


    qRegisterMetaType<ImagePreloadParams>("ImagePreloadParams");

    ImgProvider* img_provider_ptr = new ImgProvider;
    engine.addImageProvider(QString("ImageProvider"), img_provider_ptr);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    if (engine.rootObjects().isEmpty())
        return -1;
    img_provider_ptr->setRootObject(engine.rootObjects().first());
    return app.exec();
}
