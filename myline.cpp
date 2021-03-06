#include "myline.h"
#include<Windows.h>

MyLine::MyLine(QObject *parent)
    : MyShape(parent)
{
    pointStart = nullptr;
    pointEnd = nullptr;
}

MyLine::~MyLine()
{
    pointStart = nullptr;
    pointEnd = nullptr;
}

void MyLine::slotGameTimer()
{

}

QRectF MyLine::boundingRect() const
{
    return QRectF(-20,-30,40,60);
}

void MyLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(painted)
    {
        painter->drawLine(pointStart->getCenter(),pointEnd->getCenter());
    }
    else
    {
        painter->drawLine(pointStart->getCenter(),target);
    }
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

MyShape *MyLine::getPointEnd() const
{
    return pointEnd;
}

void MyLine::setPointEnd(MyShape *value)
{
    pointEnd = value;
}

MyShape *MyLine::getPointStart() const
{
    return pointStart;
}

void MyLine::setPointStart(MyShape *value)
{
    pointStart = value;
}

