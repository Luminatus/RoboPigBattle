#include "Headers/tableview.h"

TableView::TableView(QWidget* parent): QWidget(parent)
{
}


void TableView:: setViewData(ViewData* viewData)
{
   _viewData = viewData;
    int tableSize = _viewData->tableSize;
    int tileSize = _viewData->tileSize;
   setMinimumSize(tableSize*tileSize+10,tableSize*tileSize+10);
   setBaseSize(tableSize*tileSize,tableSize*tileSize);
   getPixmaps();
}


void TableView::paintEvent(QPaintEvent*)
{

    _viewData->RefreshPositions();
    Position position;
    int tableSize = _viewData->tableSize;
    int tileSize = _viewData->tileSize;
    setMinimumSize(tableSize*tileSize,tableSize*tileSize);
    setBaseSize(tableSize*tileSize,tableSize*tileSize);
    QPixmap* activePixmap;
    QPainter painter(this);
    QPen tilePen;
    tilePen.setWidth(1);
    tilePen.setColor(QColor(30,40,40));
    painter.setPen(tilePen);
    QColor bgColor;
    QColor activeColor;
    activeColor.setRgb(50,60,85);
    QBrush normalBrush;
    normalBrush.setStyle(Qt::SolidPattern);
    normalBrush.setColor(Qt::transparent);
    QBrush activeBrush;
    activeBrush.setColor(activeColor);
    activeBrush.setStyle(Qt::SolidPattern);
    bgColor.setRgb(40,50,60);
    painter.fillRect(0,0,tableSize*tileSize,tableSize*tileSize,bgColor);
    for (int i=0; i<tableSize; i++)
    {
        for (int j=0; j<tableSize; j++)
        {
            painter.drawRect(i*tileSize,j*tileSize,tileSize,tileSize);
        }
    }
    QPixmap pixmap;
    position = _viewData->BluePosition;
    pixmap = getDirection(_blue,position);
    painter.drawPixmap(position.X*tileSize,position.Y*tileSize,tileSize,tileSize,pixmap);
    painter.resetTransform();
    position = _viewData->RedPosition;
    pixmap = getDirection(_red,position);
    painter.drawPixmap(position.X*tileSize,position.Y*tileSize,tileSize,tileSize,pixmap);
    painter.resetTransform();
    /*   for (int i=0; i<tableSize; i++)
    {
        for (int j=0; j<tableSize; j++)
        {
            int tileIndex = _viewData->GetTileState(i,j);
            if(_viewData->showActive || i!=_viewData->activeTile->PosX || j!=_viewData->activeTile->PosY || tileIndex==OccupiedByBlackHole)
            {
                switch(tileIndex)
                {
                case OccupiedByBlue: normalBrush.setColor(Qt::transparent); activePixmap=_blue; break;
                case OccupiedByRed: normalBrush.setColor(Qt::transparent); activePixmap=_red; break;
                case OccupiedByBlackHole: normalBrush.setColor(Qt::transparent); activePixmap=_blackhole; break;
                case Unoccupied: normalBrush.setColor(Qt::transparent); activePixmap=NULL; break;
                default: break;
                }
                if(i==_viewData->activeTile->PosX && j==_viewData->activeTile->PosY && tileIndex!=OccupiedByBlackHole)
                    painter.setBrush(activeBrush);
                else
                    painter.setBrush(normalBrush);
            }
            else
            {
                normalBrush.setColor(Qt::transparent);
                painter.setBrush(normalBrush);
                activePixmap=NULL;
            }

            painter.drawRect(i*tileSize,j*tileSize,tileSize,tileSize);
            if(activePixmap!=NULL)
                painter.drawPixmap(i*tileSize,j*tileSize,tileSize,tileSize,*activePixmap);

        }
    }
    if(_viewData->showActive){
    QPixmap pixmap= getArrows();
    painter.setBrush(Qt::NoBrush);
    painter.drawPixmap(_viewData->activeTile->PosX*tileSize-tileSize,_viewData->activeTile->PosY*tileSize-tileSize,tileSize*3,tileSize*3,pixmap);
    }
    else
    {
        switch(_viewData->activePlayer)
        {
            case BluePlayer: activePixmap=_blue; break;
            case RedPlayer: activePixmap=_red; break;
            default: painter.setBrush(Qt::NoBrush); break;
        }
        //painter.drawRect(_viewData->goX,_viewData->goY,tileSize,tileSize);
        painter.drawPixmap(_viewData->goX,_viewData->goY,tileSize,tileSize,*activePixmap);
    }
*/
}

QPixmap TableView::getDirection(QPixmap* map,Position position)
{
    QPixmap pixmap(_viewData->tileSize,_viewData->tileSize);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.translate(_viewData->tileSize/2,_viewData->tileSize/2);
    painter.rotate(position.Angle*(90));
    painter.translate(-_viewData->tileSize/2,-_viewData->tileSize/2);
    painter.drawPixmap(0,0,_viewData->tileSize,_viewData->tileSize,*map);

    return pixmap;
}

void TableView::getPixmaps()
{
    int tileSize = _viewData->tileSize;
    QPixmap* blueShip=new QPixmap(tileSize,tileSize);
    QPixmap* redShip=new QPixmap(tileSize,tileSize);
    if(!(blueShip->load(":/pixmaps/bluepig_s.png")))
        blueShip->fill(Qt::blue);
    if(!(redShip->load(":/pixmaps/redpig_s.png")))
        redShip->fill(Qt::red);
    _blue = blueShip;
    _red = redShip;
}

TableView::~TableView()
{
    delete _red;
    delete _blue;
}
