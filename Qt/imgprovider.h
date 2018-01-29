#ifndef IMGPROVIDER_H
#define IMGPROVIDER_H
#include <QQuickImageProvider>
#include <iostream>

class ImgProvider : public QQuickImageProvider
{


public:

    ImgProvider()
        : QQuickImageProvider(QQuickImageProvider::Image)
    {
        setPageNumTotal(10); // for debug
    }
    void setRootObject(QObject* ptr){
        root_object_ptr = ptr;
        QObject* slidbar_ptr = root_object_ptr->findChild<QObject*>("SlideBar");
        slidbar_ptr->setProperty("to", page_num_total);
    }

    bool checkPageNum(QString& pg){
        int pg_n = pg.toInt();
        if(pg_n <= page_num_total) {
            if(pg_n >= 1){
                return true;
            }else{
                // pg_n < 0
                pg = QString::number(1);
                return false;
            }
        }//if else, pop up a message "Last page!"
        else{
            //pg_n > page_num_total
            pg = QString::number(page_num_total);
            return false;
        }
    }
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize)
    {
        QString image_path = path_debug;

        QStringList input_list = id.split('/');
        QImage image;

        checkPageNum(input_list[0]);
        if(input_list[1] == QString("0")){
            // image mode

            image_path += input_list[0]; //page number
            image_path += ".png";
            image =  QImage( image_path);
            QSize show_size(image.width(), image.height());
            if(size){
                size->setWidth(image.width());
                size->setHeight(image.height());
            }


        }else if(input_list[1] == QString("1")){
            //text mode
        }else{
            // need more specific exception process
            std::cout<<"error text mode"<<std::endl;
        }

        return image;
    }
    void setPageNumTotal(int value){
        page_num_total = value;
    }


private:
    QString path_debug = "C:/Users/qw595/Documents/GitHub/ComicBookReader/TestSamples/OnePiece/";
    int page_num_total = 1; //init value, cannot set 0 because of the calculation in slidebar.
    QObject *root_object_ptr;
};

#endif // IMGPROVIDER_H
