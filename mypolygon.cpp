#include "mypolygon.h"
#include<iphlpapi.h>
#include<iostream>

MyPolygon::MyPolygon(QObject *parent)
    : MyShape(parent)
{
    fl = 1;
}

MyPolygon::~MyPolygon()
{

}

void MyPolygon::slotGameTimer()
{
    if( painted)
    {
        return;
    }
    if(!state &&  GetAsyncKeyState(VK_LBUTTON))
    {
        polygon << QPoint(target.x()-250,target.y()-250);
        xVector.push_back(target.x());
        yVector.push_back(target.y());
        pointStart = target;
        state = true;
    }
    if(state &&  !GetAsyncKeyState(VK_LBUTTON) && fl == 0)
    {
        fl = 1;
    }
    if(state &&  !GetAsyncKeyState(VK_LBUTTON) && fl == 3)
    {
        int sumX = std::accumulate(xVector.begin(),xVector.end(),0);
        int sumY = std::accumulate(yVector.begin(),yVector.end(),0);
        center.setX(sumX/xVector.size());
        center.setY(sumY/yVector.size());
        pointEnd = target;
        state = false;
        if(!painted && pointEnd != pointStart)
        {
            emit addShapeSignal();
            painted = true;
        }
    }
    if(state &&  GetAsyncKeyState(VK_LBUTTON) && fl == 1)
    {
        int x = target.x();
        int y = target.y();
        if(x - xVector[0] < 10 && y - yVector[0] < 10 && xVector.size()>2)
        {
            fl = 3;
        }
        else
        {
            polygon << QPoint(target.x()-250,target.y()-250);
            xVector.push_back(target.x());
            yVector.push_back(target.y());
            fl = 0;
        }
    }
}

QRectF MyPolygon::boundingRect() const
{
    return QRectF(-20,-30,40,60);
}

void MyPolygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(painted)
    {
        painter->drawEllipse(center.x()-this->x(),center.y()-this->y(),10,10);
        painter->drawPolygon(polygon);
    }
    else
    {
        for(auto i = 0; i < xVector.size()-1;i++)
        {
            painter->drawLine(xVector[i]-250,yVector[i]-250,xVector[i+1]-250,yVector[i+1]-250);
        }
    }
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

