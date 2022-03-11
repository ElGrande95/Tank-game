#include "target.h"

#include <QRandomGenerator>
#include "targetdestroy.h"

Target::Target(QObject *parent)
    :QObject(parent), QGraphicsItem()
{
    health = QRandomGenerator::global()->bounded(1,15);
    maxHealth = health;
}

Target::~Target()
{

}

QRectF Target::boundingRect() const
{
    return QRectF(-20,-20,40,40);
}

void Target::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(Qt::green);
    painter->drawRect(-19,-20,38,34);

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    painter->drawRect(-20, 15, (int) 40*health/maxHealth,3);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

int Target::getHealth() const
{
    return health;
}

void Target::hit(int damage)
{
    health -= damage;
    this->update(QRectF(-20,-20,40,40));
    if(health <= 0)
    {
        health = 0;
        this->deleteLater();
        QPoint point = QPoint(this->pos().x() + 10, this->pos().y() + 10);
        scene()->addItem(new TargetDestroy(point));
    }

    emit signalHit();
}

