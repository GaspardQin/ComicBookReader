#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "showimage.h"


int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<ShowImage>("ComicBookReader.ShowImage", 1, 0, "ShowImage");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
