#ifndef MYRECT_H
#define MYRECT_H

#include"myshape.h"
#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QPolygon>
#include <QTimer>


class MyRect : public MyShape
{
    Q_OBJECT

public:
    explicit MyRect(QObject *parent = 0);
    ~MyRect();


private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


};

#endif // MYRECT_H
