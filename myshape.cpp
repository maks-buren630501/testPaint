#include "myshape.h"
#include<iostream>
#include<iphlpapi.h>

MyShape::MyShape(QObject *parent) :
    QObject(parent), QGraphicsItem()
{
    setRotation(0);
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MyShape::slotGameTimer);
    timer->start(30);
    target = QPointF(0,0);
    state = false;
    painted = false;
}

MyShape::~MyShape()
{
    timer->stop();
    delete timer;
}

QRectF MyShape::boundingRect() const
{
    return QRectF(-20,-30,40,60);
}

void MyShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QString MyShape::getName() const
{
    return name;
}

void MyShape::setName(const QString &value)
{
    name = value;
}

void MyShape::setPontEndStartOffset(int x, int y)
{
    int dx = pointEnd.x() - pointStart.x();
    int dy = pointEnd.y() - pointStart.y();
    pointStart.setX(pointStart.x() + x);
    pointStart.setY(pointStart.y() + y);
    pointEnd.setX(pointEnd.x() + x);
    pointEnd.setY(pointEnd.y() + y);
    center.setX(pointStart.x() + dx/2);
    center.setY(pointStart.y() + dy/2);
}

bool MyShape::atShape(QPoint point)
{

}

QPolygon MyShape::getPolygon() const
{
    return polygon;
}

void MyShape::setPolygon(const QPolygon &value)
{
    polygon = value;
}

void MyShape::setPainted(bool value)
{
    painted = value;
}

QPointF MyShape::getPointEnd() const
{
    return pointEnd;
}

QPointF MyShape::getPointStart() const
{
    return pointStart;
}

void MyShape::setPointEnd(const QPointF &value)
{
    pointEnd = value;
}

void MyShape::setPointStart(const QPointF &value)
{
    pointStart = value;
}

QPoint MyShape::getCenter() const
{
    return center;
}

void MyShape::setCenter(int x, int y)
{
    center.setX(x);
    center.setY(y);
}

void MyShape::slotTarget(QPointF point)
{
    target = point;
}

void MyShape::slotGameTimer()
{
    if( painted)
    {
        return;
    }
    if(!state &&  GetAsyncKeyState(VK_LBUTTON))
    {
        pointStart = target;
        pointEnd = target;
        state = true;
    }
    if(state &&  GetAsyncKeyState(VK_LBUTTON))
    {
        pointEnd = target;
    }
    if(state &&  !GetAsyncKeyState(VK_LBUTTON))
    {
        pointEnd = target;
        state = false;
        if(!painted && pointEnd != pointStart)
        {
            painted = true;
            emit addShapeSignal();
        }
    }
}


