#include "myscene.h"

MyScene::MyScene(QObject *parent) :
    QGraphicsScene()
{
    Q_UNUSED(parent);
}

MyScene::~MyScene()
{

}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit signalTargetCoordinate(event->scenePos());
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    emit signalMouseClick(mouseEvent->scenePos());
}



