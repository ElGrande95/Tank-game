#include "explosion.h"

Explosion::Explosion(QPointF point, QObject *parent) :
    QObject(parent), QGraphicsItem()
{
    this->setPos(point);
    currentFrame = 0;

    explosionImage = new QPixmap(":/Imgs/Images/explosion.png");
    *explosionImage = explosionImage->scaled( 5, 5, Qt::IgnoreAspectRatio,Qt::FastTransformation);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &Explosion::nextFrame);
    timer->start(15);
}

Explosion::~Explosion()
{

}

QRectF Explosion::boundingRect() const
{
    return QRectF(-15,-15,30,30);
}

void Explosion::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(-15,-15, *explosionImage, currentFrame, 0, 30,30);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Explosion::nextFrame()
{
    *explosionImage = explosionImage->scaled( explosionImage->width() + 5, explosionImage->height() + 5, Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    if (explosionImage->width() >= 40 ) {
        this->deleteLater();
    } else {
        this->update(-10,-10,20,20);
    }
}

int Explosion::type() const {
    return typeExplosion;
}
