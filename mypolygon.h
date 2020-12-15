#ifndef MYPOLYGON_H
#define MYPOLYGON_H

#include"myshape.h"
#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QPolygon>
#include <QTimer>

class MyPolygon : public MyShape
{
    Q_OBJECT

public:
    explicit MyPolygon(QObject *parent = 0);
    ~MyPolygon();
    void setPontEndStartOffset(int x, int y);
    bool atShape(QPoint point);
    bool isCross(QLine a, QLine b);
public slots:

    virtual void slotGameTimer();

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int fl;

};

#endif // MYPOLYGON_H
