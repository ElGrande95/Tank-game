#include "bullet.h"
#include <math.h>
#include <QDebug>
#include "explosion.h"
#include "targetdestroy.h"
#include "myhero.h"
#include "target.h"
#include <QRandomGenerator>

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

Bullet::Bullet(QPointF start, QPointF end,  const int& heroType, QObject* parent)
    : QObject(parent), QGraphicsItem()
{
    this->setRotation(0);
    if(heroType == MyHero::typeMyHero)
        speed = 30;
    else
        speed = 10;
    this->heroType = heroType;
    this->start = start;
    this->setPos(start);

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

QPointF Bullet::getStart() const
{
    return start;
}

int Bullet::getHeroType() const
{
    return heroType;
}

void Bullet::slotTimerBullet()
{
    setPos(mapToParent(0, -speed));

    QList<QGraphicsItem *> foundItems = this->collidingItems();
    foreach (QGraphicsItem *item, foundItems) {
        if (item == this || item->type() == heroType ||
                item->type() == Explosion::typeExplosion || item->type() == TargetDestroy::typeDestroy)
            continue;

        if(heroType == MyHero::typeMyHero){
            QPoint point = QPoint(this->pos().x() + 12, this->pos().y() + 12);
            scene()->addItem(new Explosion(point));
            callbackFunc(item);
            this->deleteLater();

            emit signalTargetShot(item);
        }
        else if (heroType == Target::typeTarget) {
            if(item->type() == Target::typeTarget)
                continue;
            QPoint point = QPoint(this->pos().x() + 12, this->pos().y() + 12);
            scene()->addItem(new Explosion(point));
            if(item->type() == MyHero::typeMyHero) {
                MyHero *h = qgraphicsitem_cast <MyHero *> (item);
                h->damage(QRandomGenerator::global()->bounded(1,5));
            }
            this->deleteLater();
        }
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

int Bullet::type() const {
    return typeBullet;
}
