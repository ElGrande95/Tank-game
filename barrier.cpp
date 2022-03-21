#include "barrier.h"
#include "myhero.h"
#include "target.h"

Barrier::Barrier(QObject *parent)
    : QObject(parent), QGraphicsItem()
{
    directionDown = true;

    timerBarrier = new QTimer();
    connect(timerBarrier, &QTimer::timeout, this, &Barrier::slotBarrier);
    timerBarrier->start(10);
}

Barrier::~Barrier()
{

}

QRectF Barrier::boundingRect() const
{
    return QRectF(-10, -40, 20, 80);
}

void Barrier::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(Qt::darkGray);
    painter->setBrush(Qt::darkGray);
    painter->drawRect(-10, -40, 20, 80);

}

QPainterPath Barrier::shape() const
{
    QPainterPath path;
    QRectF rect = QRectF(-10, -40, 20, 80);
    path.addRect(rect);
    return path;
}

void Barrier::slotBarrier()
{
    QList<QGraphicsItem *> foundItems = this->collidingItems();
    foreach (QGraphicsItem *item, foundItems) {
        if (item->type() == MyHero::typeMyHero || item->type() == Target::typeTarget)
            directionDown = !directionDown;
    }


    if(mapToParent(x(),y()).y() == 900)
        directionDown = false;
    else if(mapToParent(x(),y()).y() == 120)
        directionDown = true;

    if(directionDown)
        this->setPos(this->x(), this->y() + 2);
    else
        this->setPos(this->x(), this->y() - 2);
}

int Barrier::type() const {
    return typeBarrier;
}
