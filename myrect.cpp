#include "myrect.h"

MyRect::MyRect(QObject *parent)
    : MyShape(parent)
{
}

MyRect::~MyRect()
{

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
