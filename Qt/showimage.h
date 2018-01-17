#ifndef SHOWIMAGE_H
#define SHOWIMAGE_H
#include <QImage>
#include <QPainter>
#include <QQuickItem>
#include <QQuickPaintedItem>
#include <string>

class ShowImage : public QQuickPaintedItem {

Q_OBJECT
Q_PROPERTY(int pageNum READ getCurrentPageNum WRITE setPageNum NOTIFY PageNumChanged)
//only use the NOTIFY "ShowStatusChanged", it's used for refresh the painting
public:
    ShowImage(QQuickItem *parent = 0);
    void paint(QPainter *painter);
    void setDirectory_Debug(std::string path_debug);

    ////////////////////
    //For debug
    void updateTotalPageNum_debug(int value);
    //End for debug
    ////////////////////


    //void updateTotalPageNum(); //in real case, page_num_total also needs to be set.
    void setNextPageNum();
    void setPreviousPageNum();
    void setPageNum(int value); //directly jump into the specific page
    int getCurrentPageNum();
    Q_INVOKABLE int getTotalPageNum();
private:
    int page_num = 0; //init value
    int page_num_total = 1; //init value, cannot set 0 because of the calculation in slidebar.
    std::string path_debug = "C:/Users/qw595/Documents/GitHub/ComicBookReader/TestSamples/OnePiece/";

signals:
    void PageNumChanged();
};

#endif // SHOWIMAGE_H
