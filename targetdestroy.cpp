#include "targetdestroy.h"

TargetDestroy::TargetDestroy(QPointF point, QObject *parent) :
    QObject(parent), QGraphicsItem()
{
    this->setPos(point);
    currentFrame = 0;

    explosionImage = new QPixmap(":/Imgs/Images/targetDestroy.png");
    *explosionImage = explosionImage->scaled( 10, 10, Qt::IgnoreAspectRatio,Qt::FastTransformation);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &TargetDestroy::nextFrame);
    timer->start(20);
}

TargetDestroy::~TargetDestroy()
{

}

QRectF TargetDestroy::boundingRect() const
{
    return QRectF(-40,-40,80,80);
}

void TargetDestroy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(-40,-40, *explosionImage, currentFrame, 0, 80,80);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void TargetDestroy::nextFrame()
{
    *explosionImage = explosionImage->scaled( explosionImage->width() + 10, explosionImage->height() + 10, Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    if (explosionImage->width() >= 80 ) {
        this->deleteLater();
    } else {
        this->update(-40,-40,80,80);
    }
}

int TargetDestroy::type() const {
    return typeDestroy;
}
