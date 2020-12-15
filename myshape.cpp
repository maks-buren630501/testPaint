#include "myshape.h"
#include<iostream>
#include<iphlpapi.h>

MyShape::MyShape(QObject *parent) :
    QObject(parent), QGraphicsItem()
{
    setRotation(0);      // Set the starting triangle rotation
    gameTimer = new QTimer();   // Init game timer
    // Connect the signal from the timer and the slot game processing timer
    connect(gameTimer, &QTimer::timeout, this, &MyShape::slotGameTimer);
    gameTimer->start(30);   // Start timer
    target = QPointF(0,0);  // Set the initial position of the cursor
    state = false;
    painted = false;
}

MyShape::~MyShape()
{
    gameTimer->stop();
    delete gameTimer;
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
    //pointStart.setX();
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
            emit addShapeSignal();
            painted = true;
        }
    }
}
