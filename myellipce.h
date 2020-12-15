#ifndef MYELLIPCE_H
#define MYELLIPCE_H

#include"myshape.h"
#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QPolygon>
#include <QTimer>

class MyEllipce : public MyShape
{
    Q_OBJECT

public:
    explicit MyEllipce(QObject *parent = 0);
    ~MyEllipce();


private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // MYELLIPCE_H
