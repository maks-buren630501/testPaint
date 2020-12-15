#ifndef MYLINE_H
#define MYLINE_H

#include"myshape.h"
#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QPolygon>
#include <QTimer>

class MyLine : public MyShape
{
    Q_OBJECT

public:
    explicit MyLine(QObject *parent = 0);
    ~MyLine();
    MyShape *getPointStart() const;
    void setPointStart(MyShape *value);

    MyShape *getPointEnd() const;
    void setPointEnd(MyShape *value);

public slots:

    virtual void slotGameTimer();

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    MyShape *pointStart;
    MyShape *pointEnd;
};

#endif // MYLINE_H
