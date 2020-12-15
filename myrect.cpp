#include "myrect.h"

MyRect::MyRect(QObject *parent)
    : MyShape(parent)
{
}

MyRect::~MyRect()
{

}

bool MyRect::atShape(QPoint point)
{
    if(pointStart.x() > pointEnd.x())
    {
        if(point.x() > pointStart.x() || point.x() < pointEnd.x())
        {
            return false;
        }
    }
    else
    {
        if(point.x() > pointEnd.x() || point.x() < pointStart.x())
        {
            return false;
        }
    }
    if(pointStart.y() > pointEnd.y())
    {
        if(point.y() > pointStart.y() || point.y() < pointEnd.y())
        {
            return false;
        }
    }
    else
    {
        if(point.y() > pointEnd.y() || point.y() < pointStart.y())
        {
            return false;
        }
    }
    return true;
}

QRectF MyRect::boundingRect() const
{
    return QRectF(-20,-30,40,60);
}

void MyRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int x = (pointEnd.x() + pointStart.x())/2;
    int y = (pointEnd.y() + pointStart.y())/2;
    int dx = pointEnd.x() - pointStart.x();
    int dy = pointEnd.y() - pointStart.y();
    painter->drawRect(pointStart.x(),pointStart.y(),dx,dy);
    if(!painted)
    {
        center.setX(x);
        center.setY(y);
    }
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
