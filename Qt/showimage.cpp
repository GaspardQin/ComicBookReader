#include "showimage.h"

ShowImage::ShowImage(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    //////////////
    //For Debug
    page_num = 1;
    page_num_total = 10;
    //////////////
}
void ShowImage::paint(QPainter *painter){
    //////////////
    //For Debug
    std::string image_path = path_debug;
    image_path += std::to_string(page_num);
    image_path += ".png";
    painter->drawImage(contentsBoundingRect(), QImage( QString::fromStdString(image_path)));
    //QImage *image = new QImage("C:/Users/qw595/Documents/GitHub/ComicBookReader/TestSamples/OnePiece/graphe.png");
    //painter->drawImage(contentsBoundingRect(), *image);
    //Qimage cannot load jpg
    //image_path += ".jpg";
    //free(image);
    //End for Debug
    /////////////

}


//////////////
//For Debug
void ShowImage::updateTotalPageNum_debug(int value){
    page_num_total = value;
    emit PageNumChanged();
    update();
}
//End for Debug
/////////////

void ShowImage::setNextPageNum(){
    if(page_num + 1 <= page_num_total) {
        page_num++;
        emit PageNumChanged();
        update();
    }//if else, pop up a message "Last page!"
}
void ShowImage::setPreviousPageNum(){
    if(page_num - 1 >= 1) {
        page_num--;
        emit PageNumChanged();
        update();
    }
    //if else, pop up a message "First page!"
}
void ShowImage::setPageNum(int value){
    if(value >=1 && value <= page_num_total){
        page_num = value;
        emit PageNumChanged();
        update();
    }
    //else, pop up a message "Exceed range"
}
int ShowImage::getCurrentPageNum(){
    return page_num;
}
int ShowImage::getTotalPageNum(){
    return page_num_total;
}
