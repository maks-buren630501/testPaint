#include "mypolygon.h"
#include"mainwindow.h"
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

void MyPolygon::setPontEndStartOffset(int x, int y)
{
    QPolygon polygonNew;
    int dx = center.x() - polygon[0].x();
    int dy = center.y() - polygon[0].y();
    x-=dx;
    y-=dy;
    polygonNew << QPoint(x,y);
    for(int i = 1; i < polygon.size();i++)
    {
        int dx = polygon[i].x() - polygon[0].x();
        int dy = polygon[i].y() - polygon[0].y();
        polygonNew << QPoint(x+dx,y+dy);
    }
    int cx = 0;
    int cy = 0;
    for(int i = 1; i < polygonNew.size();i++)
    {
        cx += polygonNew[i].x();
        cy += polygonNew[i].y();
    }
    cx /= (polygonNew.size()-1);
    cy /= (polygonNew.size()-1);
    center.setX(cx);
    center.setY(cy);
    polygon = polygonNew;
}

void MyPolygon::slotGameTimer()
{
    if( painted)
    {
        return;
    }
    if(!state &&  GetAsyncKeyState(VK_LBUTTON))
    {
        polygon << QPoint(target.x(),target.y());
        pointStart = target;
        state = true;
    }
    if(state &&  !GetAsyncKeyState(VK_LBUTTON) && fl == 0)
    {
        fl = 1;
    }
    if(state &&  !GetAsyncKeyState(VK_LBUTTON) && fl == 3)
    {
        int cx = 0;
        int cy = 0;
        for(int i = 1; i < polygon.size();i++)
        {
            cx += polygon[i].x();
            cy += polygon[i].y();
        }
        cx /= (polygon.size()-1);
        cy /= (polygon.size()-1);
        center.setX(cx);
        center.setY(cy);
        pointEnd = target;
        state = false;
        if(!painted && pointEnd != pointStart)
        {
            painted = true;
            emit addShapeSignal();
        }
    }
    if(state &&  GetAsyncKeyState(VK_LBUTTON) && fl == 1)
    {
        auto point  = target;
        point.setX(target.x());
        point.setY(target.y());
        if(MainWindow::neerPoints(point.toPoint(),polygon[0],10) && polygon.size()>3)
        {
            fl = 3;
        }
        else
        {
            polygon << QPoint(target.x(),target.y());
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
        for(auto i = 0; i < polygon.size()-1;i++)
        {
            painter->drawLine(polygon[i].x(),polygon[i].y(),polygon[i+1].x(),polygon[i+1].y());
        }
        if(polygon.size() > 0)
        {
            painter->drawLine(polygon[polygon.size()-1].x(),polygon[polygon.size()-1].y(),target.x(),target.y());
        }
    }
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

