#ifndef MYSHAPE_H
#define MYSHAPE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QPolygon>
#include <QTimer>


class MyShape : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    explicit MyShape(QObject *parent = 0);
    virtual ~MyShape();

    QPoint getCenter() const;

    virtual void setCenter(int x,int y);

    void setPointStart(const QPointF &value);

    void setPointEnd(const QPointF &value);

    QPointF getPointStart() const;

    QPointF getPointEnd() const;

    void setPainted(bool value);

    QPolygon getPolygon() const;
    void setPolygon(const QPolygon &value);

    QString getName() const;
    void setName(const QString &value);

    virtual void setPontEndStartOffset(int x, int y);

signals:
    void addShapeSignal();
public slots:
    virtual void slotTarget(QPointF point);
    virtual void slotGameTimer();

private:
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QString name;
    QTimer *timer;
    QPointF target;
    QPointF pointStart;
    QPointF pointEnd;
    QPoint center;
    bool state;
    bool painted;
    QPolygon polygon;
};
#endif // MYSHAPE_H
