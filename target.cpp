#include "target.h"

#include <QRandomGenerator>
#include "targetdestroy.h"
#include <QDebug>
#include <QtMath>
#include "bullet.h"
#include "explosion.h"
#include "barrier.h"
#include "myhero.h"
#include "wall.h"
#include "myhero.h"

#define Pi 3.14159265358979323846264338327950288419717
#define TwoPi (2.0 * Pi)

static qreal normalizeAngleDeg(qreal angle)
{
    while (angle < 0)
        angle += 360;
    while (angle >= 360)
        angle -= 360;
    return angle;
}


Target::Target(QObject *parent)
    :QObject(parent), QGraphicsItem()
{
    this->setRotation(QRandomGenerator::global()->bounded(360));
    speed = 2;
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
    path.addRect(QRectF(-20,-20,40,40));
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
    if(x() < 60)
        setX(60);
    else if (x() > 440)
        setX(440);
    else if(y() < 60)
        setY(60);
    else if (y() > 440)
        setY(440);
}

void Target::changeDirection(QGraphicsItem* item)
{
    setPos(mapToParent(0, speed));

    if(item->pos().x() >= this->pos().x())
    {
        //levo
        setRotation(360 - this->rotation());
        setX(200);
    }
    else if (item->pos().x() + 20 <= this->pos().x())
    {
        //desno
        setRotation(360 - this->rotation());
        setX(300);
    }

    else if(item->pos().y() < this->pos().y())
    {
        //dole

        if(this->rotation() < 90)
        {
            setRotation(180 - this->rotation());
        }
        else if(this->rotation() >270)
        {
            setRotation(540 - this->rotation());
        }
        setY(pos().y() + 30);
    }
    else
    {
        //gore

        if(this->rotation() > 90 && this->rotation() < 180)
        {
            setRotation(180 - this->rotation());
        }
        else if(this->rotation() <= 270 && this->rotation() >= 180)
        {
            setRotation(540 - this->rotation());
        }

        setY(pos().y() - 30);
    }
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
            this->setRotation(normalizeAngleDeg(this->rotation()));

            if(item->type() == Barrier::typeBarrier)
            {
                changeDirection(item);
            }

            else if(item->type() == Wall::typeWall)
            {
                setPos(mapToParent(0, speed));

                if(item->pos().x() == 0)
                {
                    //left
                    setRotation(360 - this->rotation());

                }
                else if(item->pos().x() == 480 && item->pos().y() == 20)
                {
                    //right
                    setRotation(360 - this->rotation());

                }
                else if(item->pos().x() == 500)
                {
                    //top

                    if(this->rotation() < 90)
                    {
                        setRotation(180 - this->rotation());
                    }
                    else
                    {
                        setRotation(540 - this->rotation());
                    }
                }
                else if(item->pos().x() == 480 && item->pos().y() == 480)
                {
                    //bottom

                    if(this->rotation() < 180)
                    {
                        setRotation(180 - this->rotation());
                    }
                    else
                    {
                        setRotation(540 - this->rotation());
                    }
                }

                setPosition();
            }

            else if(item->type() == MyHero::typeMyHero)
            {
                MyHero *hero = qgraphicsitem_cast <MyHero *> (item);
                hero->damage(100);
                hit(100);
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
        emit signalDestroy();
    }

    emit signalHit();
}

int Target::type() const {
    return typeTarget;
}
