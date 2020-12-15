#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>

class MyScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit MyScene(QObject *parent = 0);
    ~MyScene();

signals:
    // The signal of transmit the coordinates of the mouse position
    void signalTargetCoordinate(QPointF point);
    void signalMouseClick(QPointF point);

public slots:

private:
    // The function, which made tracking the mouse
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
};

#endif // MYSCENE_H
