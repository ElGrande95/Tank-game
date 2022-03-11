#include "bullet.h"
#include <math.h>
#include <QDebug>
#include "explosion.h"
#include "targetdestroy.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

static qreal normalizeRad(qreal angle)
{
    while (angle < 0)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}

Bullet::Bullet(QPointF start, QPointF end,  QGraphicsItem* hero, QObject* parent)
    : QObject(parent), QGraphicsItem()
{
    this->setRotation(0);
    this->setPos(start);
    speed = 20;
    this->hero = hero;

    QLineF lineToTarget(start, end);

    qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());
    if (lineToTarget.dy() < 0)
        angleToTarget = TwoPi - angleToTarget;
    angleToTarget = normalizeRad((Pi - angleToTarget) + Pi / 2);

    if (angleToTarget >= 0 && angleToTarget < Pi) {
        setRotation(rotation() - angleToTarget * 180 /Pi);
    } else if (angleToTarget <= TwoPi && angleToTarget > Pi) {
        setRotation(rotation() + (angleToTarget - TwoPi )* (-180) /Pi);
    }

    if(start.x() == end.x() && start.y() < end.y())
        setRotation(180);

    timerBullet = new QTimer();
    connect(timerBullet, &QTimer::timeout, this, &Bullet::slotTimerBullet);
    timerBullet->start(20);
}

Bullet::~Bullet()
{

}

QRectF Bullet::boundingRect() const
{
    return QRectF(-1,-1,3,3);

}
void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(Qt::black);
    painter->drawRect(-1,-1,3,3);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QPainterPath Bullet::shape() const
{
    QPainterPath path;
    QRectF rect = QRectF(-1,-1,3,3);
    path.addRect(rect);
    return path;
}

void Bullet::slotTimerBullet()
{
    setPos(mapToParent(0, -speed));

    QPolygonF polygon = QPolygonF()<< mapToScene(-1, -1)
                                   << mapToScene(-1, 2)<< mapToScene(2, 2) << mapToScene(2, -1);
    QList<QGraphicsItem *> foundItems = scene()->items(polygon);

    foreach (QGraphicsItem *item, foundItems) {
        if (item == this || item == hero || item->type() == Explosion::typeExplosion || item->type() == TargetDestroy::typeDestroy)
            continue;
        scene()->addItem(new Explosion(this->pos()));
        callbackFunc(item);
        this->deleteLater();
    }

    if(this->x() < 20){
        this->deleteLater();
    }
    if(this->x() > 480){
        this->deleteLater();
    }
    if(this->y() < 20){
        this->deleteLater();
    }
    if(this->y() > 480){
        this->deleteLater();
    }
}

void Bullet::setCallbackFunc(void (*func)(QGraphicsItem *))
{
    callbackFunc = func;
}
