#include "target.h"

#include <QRandomGenerator>
#include "targetdestroy.h"
#include <QDebug>
#include <QtMath>
#include "bullet.h"
#include "explosion.h"
#include "barrier.h"
#include "myhero.h"

#define Pi 3.14159265358979323846264338327950288419717
#define TwoPi (2.0 * Pi)


static qreal normalizeAngleRadDeg(qreal angle)
{
    while (angle < 0)
        angle += 360;
    while (angle >= 360)
        angle -= 360;
    return angle;
}

static qreal normalizeAngleRad(qreal angle)
{
    while (angle < 0)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}

Target::Target(QObject *parent)
    :QObject(parent), QGraphicsItem()
{
    this->setRotation(QRandomGenerator::global()->bounded(360));
    speed = 1;
    move = true;

    health = QRandomGenerator::global()->bounded(15,25);
    maxHealth = health;

    timerGameEnemy = new QTimer();
    connect(timerGameEnemy, &QTimer::timeout, this, &Target::slotGameEnemy);
    timerGameEnemy->start(10);

    connect(this, &Target::signalHit, this, &Target::slotEnemyFocus);

}

Target::~Target()
{

}

QRectF Target::boundingRect() const
{
    return QRectF(-20,-20,40,40);
}

QPainterPath Target::shape() const
{
    QPainterPath path;
    QPolygonF poly;
    poly << QPoint(-15, -15) << QPoint(-15, 20)  << QPoint(20, 20)<< QPoint(20, -15)
         << QPoint(4, -15) << QPoint(4, -20) << QPoint(-3, -20) << QPoint(-3, -15);
    path.addPolygon(poly);
    return path;
}

void Target::setMove(bool newMove)
{
    move = newMove;
}

bool Target::getMove() const
{
    return move;
}

void Target::setPosition()
{
    if(x() < 50)
        setX(50);
    else if (x() > 450)
        setX(450);
    else if(y() < 50)
        setY(50);
    else if (y() > 450)
        setY(450);
}

void Target::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect1 = QRectF(-15, -15, 31, 31);
    QRectF rect2 = QRectF(-3, -20, 7, 14);

    painter->setBrush(Qt::darkRed);
    painter->drawRect(rect1);
    painter->setBrush(Qt::black);
    painter->drawRect(rect2);

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    painter->drawRect(-15, 17, (int) 30*health/maxHealth,3);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

int Target::getHealth() const
{
    return health;
}

void Target::slotGameEnemy()
{
    if(move) {
        setPos(mapToParent(0, -speed));

        QList<QGraphicsItem *> foundItems = scene()->collidingItems(this);
        foreach(QGraphicsItem *item, foundItems)
        {
            if(item->type() == type())
                break;

            this->setRotation(normalizeAngleRadDeg(this->rotation()));
            if(item->type() != Bullet::typeBullet && item->type() != Explosion::typeExplosion)
            {
                setPos(mapToParent(0, speed));
                if(QRandomGenerator::global()->bounded(2))
                {
                    setRotation(rotation() + 87 + QRandomGenerator::global()->bounded(10));
                    if(!scene()->collidingItems(this).isEmpty())
                        setRotation(rotation() - 180  - QRandomGenerator::global()->bounded(10));
                }
                else
                {
                    setRotation(rotation() - 87  - QRandomGenerator::global()->bounded(10));
                    if(!scene()->collidingItems(this).isEmpty())
                        setRotation(rotation() + 180  + QRandomGenerator::global()->bounded(10));
                }

                setPosition();
                break;
            }
        }
    }
}

void Target::slotEnemyFocus()
{
    move = false;
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

int Target::type() const {
    return typeTarget;
}
