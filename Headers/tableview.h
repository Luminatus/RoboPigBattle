#ifndef TABLEVIEW_H
#define TABLEVIEW_H
#include <QWidget>
#include <QPainter>
#include "Headers/viewdata.h"
#include <QMouseEvent>

class TableView: public QWidget
{
    Q_OBJECT
public:
    TableView(QWidget* parent=0);
    ~TableView();
    void setViewData(ViewData* viewData);
protected:
    void paintEvent(QPaintEvent*);
private:
    ViewData* _viewData;
    QPixmap getDirection(QPixmap* map, Position position);
    void getPixmaps();
    QPixmap* _blue;
    QPixmap* _red;
};

#endif // TABLEVIEW_H
