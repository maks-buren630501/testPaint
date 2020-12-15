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

bool MyPolygon::atShape(QPoint point)
{
    int y = point.y();
    int x = point.x() + 1000;
    QPoint p2(x,y);
    QLine crosLine(point,p2);
    int t = 0;
    for(int i = 1; i < polygon.size() - 1; i++)
    {
        QLine line(polygon[i],polygon[i+1]);
        if(isCross(line,crosLine))
        {
            t++;
        }
    }
    std::cout <<"t = "<<t<<"\n";
    if(t % 2 == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool MyPolygon::isCross(QLine a, QLine b)
{
    double x1 = a.x1();
    double x2 = a.x2();
    double x3 = b.x1();
    double x4 = b.x2();
    double y1 = a.y1();
    double y2 = a.y2();
    double y3 = b.y1();
    double y4 = b.y2();
    if(x1 >= x2)
    {
        std::swap(x1,x2);
        std::swap(y1,y2);
    }
    if(x3 >= x4)
    {
        std::swap(x3,x4);
        std::swap(y3,y4);
    }
    double k1 = 0;
    double k2 = 0;
    if(y2 != y1)
    {
       k1 =  ( y2 - y1 ) / ( x2 - x1 );
    }
    if(y3 != y4)
    {
        k2 =  ( y4 - y3 ) / ( x4 - x3 );
    }
    if(k1 == k2)
    {
        return false;
    }
    if((x1 <= x4 && x4 <= x2)||(x1 <= x3 && x3 <= x2))
    {
        return true;
    }
    else
    {
        return false;
    }

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

