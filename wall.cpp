#include "wall.h"
#include "myhero.h"
#include "target.h"

Wall::Wall(QObject *parent)
    : QObject(parent), QGraphicsItem()
{
}

Wall::~Wall()
{

}

QRectF Wall::boundingRect() const
{
    return QRectF(0, 0, 20, 480);
}

void Wall::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(Qt::darkGray);
    painter->setBrush(Qt::darkGray);
    painter->drawRect(0, 0, 20, 480);

}

QPainterPath Wall::shape() const
{
    QPainterPath path;
    QRectF rect = QRectF(0, 0, 20, 480);
    path.addRect(rect);
    return path;
}

int Wall::type() const {
    return typeWall;
}
