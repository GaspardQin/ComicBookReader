QT += quick
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
win64 {
    ##########
    #Files for opencv
    INCLUDEPATH += $$PWD\..\3dparts\include\opencv
    LIBS += -L"$$PWD\\..\\3dparts\\lib\\opencv" \
        -lopencv_world331
    ##########
    #Files for unarr
    INCLUDEPATH += $$PWD\..\3dparts\include\unarr
    LIBS += -L"$$PWD\\..\\3dparts\\lib\\unarrlib" \
        -lunarrlib

    ##########
    #Files for zlib
    INCLUDEPATH += $$PWD\..\3dparts\include\zlib
    LIBS += -L"$$PWD\\..\\3dparts\\lib\\zlib" \
        -lzlib
}
unix {
    ##########
    #Files for opencv
    INCLUDEPATH += $$PWD/../3dparts/include/opencv
    LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc
    #Files for unarr
    INCLUDEPATH += $$PWD/../3dparts/include/unarr
    LIBS += -L$$PWD/../3dparts/lib/unarrlib -lunarr
}
##########
#Files for ComicBookReader
INCLUDEPATH += ..\include\
INCLUDEPATH += ..\src\
##########


SOURCES += main.cpp \
    showimage.cpp \
    imgprovider.cpp \
    preloadworker.cpp\
    ../src/ArchiveReader.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    showimage.h \
    imgprovider.h \
    preloadworker.h \
    cache.h \
    ../include/comic_book_reader_contract.h \
    ../include/image_process.h \
    ../include/ArchiveReader.h

DISTFILES +=
