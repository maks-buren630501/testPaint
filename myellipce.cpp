#include "myellipce.h"

MyEllipce::MyEllipce(QObject *parent)
    : MyShape(parent)
{
}

MyEllipce::~MyEllipce()
{

}

bool MyEllipce::atShape(QPoint point)
{
    double dx = abs(pointEnd.x() - pointStart.x()) / 2;
    double dy = abs(pointEnd.y() - pointStart.y()) / 2;
    double x = point.x() - center.x();
    double y = point.y() - center.y();
    if ((x*x)/(dx*dx) + (y*y)/(dy*dy) <= 1.0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

QRectF MyEllipce::boundingRect() const
{
    return QRectF(-20,-30,40,60);
}

void MyEllipce::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int x = (pointEnd.x() + pointStart.x())/2;
    int y = (pointEnd.y() + pointStart.y())/2;
    int dx = pointEnd.x() - pointStart.x();
    int dy = pointEnd.y() - pointStart.y();
    painter->drawEllipse(pointStart.x(),pointStart.y(),dx,dy);
    if(!painted)
    {
        center.setX(x);
        center.setY(y);
    }
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
