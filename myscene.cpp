#include "myscene.h"

MyScene::MyScene(QObject *parent)
    : QGraphicsScene()
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

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    emit signalShot(true);
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    emit signalShot(false);
}
