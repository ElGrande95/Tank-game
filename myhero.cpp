#include "myhero.h"

#include <math.h>
#include <QDebug>
#include <barrier.h>
#include "targetdestroy.h"
#include <QRandomGenerator>
#include "explosion.h"
#include <QMessageBox>
#include <QApplication>

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

MyHero::MyHero(QObject *parent)
    :QObject(parent), QGraphicsItem()
{
    setRotation(90);
    direction = QPointF(70, 45);
    shot = false;
    speed = 5;
    health = 100;
    maxHealth = 100;

    gameTimer = new QTimer();
    connect(gameTimer, &QTimer::timeout, this, &MyHero::slotGameTimer);
    gameTimer->start(10);

    bulletTimer = new QTimer();
    connect(bulletTimer, &QTimer::timeout, this, &MyHero::slotBulletTimer);
    bulletTimer->start(170);
}

MyHero::~MyHero()
{

}

QRectF MyHero::boundingRect() const
{
    return QRectF(-20,-20,40,40);
}

QPainterPath MyHero::shape() const
{
    QPainterPath path;
    QPolygon polygon;
    polygon << QPoint(-15,-15) << QPoint(-15, 16) << QPoint(16,16) << QPoint(16,-15) << QPoint(4,-15) << QPoint(4,-20)
            << QPoint(-3,-20) << QPoint(-3,-15);
    path.addPolygon(polygon);
    return path;
}



void MyHero::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QRectF heroRect1 = QRectF(-15, -15, 31, 31);
    QRectF heroRect2 = QRectF(-3, -20, 7, 14);

    painter->setBrush(Qt::darkGreen);
    painter->drawRect(heroRect1);
    painter->setBrush(Qt::black);
    painter->drawRect(heroRect2);
}

void MyHero::slotDirection(QPointF point)
{
    direction = point;
    QLineF lineDirection(QPointF(0, 0), mapFromScene(direction));

    qreal angleDirection = ::acos(lineDirection.dx() / lineDirection.length());

    if (lineDirection.dy() < 0)
        angleDirection = TwoPi - angleDirection;
    angleDirection = normalizeAngleRad((Pi - angleDirection) + Pi / 2);

    if (angleDirection >= 0 && angleDirection < Pi) {

        setRotation(rotation() - angleDirection * 180 /Pi);
        if(!scene()->collidingItems(this).isEmpty()){
            setRotation(rotation() + angleDirection * 180 /Pi);
        }
    } else if (angleDirection <= TwoPi && angleDirection > Pi) {

        setRotation(rotation() + (angleDirection - TwoPi )* (-180) /Pi);
        if(!scene()->collidingItems(this).isEmpty()){
            setRotation(rotation() - (angleDirection - TwoPi )* (-180) /Pi);
        }
    }
}

void MyHero::slotShot(bool shot)
{
    this->shot = shot;
}


void MyHero::slotGameTimer()
{
    QList<QGraphicsItem *> foundItems = this->collidingItems();
    foreach (QGraphicsItem *item, foundItems) {
        if (item->type() == Barrier::typeBarrier)
            damage(QRandomGenerator::global()->bounded(30,40));
    }

    QLineF lineDirection(QPointF(-5, 12), mapFromScene(direction));

    qreal angleDirection = ::acos(lineDirection.dx() / lineDirection.length());
    if (lineDirection.dy() < 0)
        angleDirection = TwoPi - angleDirection;
    angleDirection = normalizeAngleRad((Pi - angleDirection) + Pi / 2);

    if (angleDirection >= 0 && angleDirection < Pi) {

        setRotation(rotation() - angleDirection * 180 /Pi);
        if(!scene()->collidingItems(this).isEmpty()){
            setRotation(rotation() + angleDirection * 180 /Pi);
        }
    }
    else if (angleDirection <= TwoPi && angleDirection > Pi) {

        setRotation(rotation() + (angleDirection - TwoPi )* (-180) /Pi);
        if(!scene()->collidingItems(this).isEmpty()){
            setRotation(rotation() - (angleDirection - TwoPi )* (-180) /Pi);
        }
    }
}

void MyHero::slotBulletTimer()
{
    int xStart = this->x() + 10 * sin(this->rotation() * TwoPi/360);
    int yStart = this->y() - 10 * cos(this->rotation() * TwoPi/360);


    int xEnd = this->x() + 30 * sin(this->rotation() * TwoPi/360);
    int yEnd = this->y() - 30 * cos(this->rotation() * TwoPi/360);

    if(shot) emit signalBullet(QPointF(xStart, yStart), QPointF(xEnd, yEnd));
}

int MyHero::getHealth() const
{
    return health;
}

int MyHero::getSpeed() const
{
    return speed;
}

int MyHero::type() const {
    return typeMyHero;
}

void MyHero::damage(int dmg)
{
    health -=dmg;
    if(health <= 0)
    {
        health = 0;
        this->deleteLater();
        QPoint point = QPoint(this->pos().x() + 10, this->pos().y() + 10);
        scene()->addItem(new TargetDestroy(point));
    }
    this->update(QRectF(-20,-20,40,40));

    emit signalDamage();
}


void MyHero::keyPressEvent(QKeyEvent *event)
{
    int speed = this->getSpeed();
    this->setRotation(normalizeAngleRadDeg(this->rotation()));

    if(event->key() == Qt::Key_A){
        this->setX(this->x() + speed * sin(this->rotation() * TwoPi/360 - Pi/2));
        this->setY(this->y() - speed * cos(this->rotation() * TwoPi/360 - Pi/2));

        if(!scene()->collidingItems(this).isEmpty()){
            this->setX(this->x() - speed * sin(this->rotation() * TwoPi/360 - Pi/2));
            this->setY(this->y() + speed * cos(this->rotation() * TwoPi/360 - Pi/2));
        }
    }
    if(event->key() == Qt::Key_D){

        this->setX(this->x() + speed * sin(this->rotation() * TwoPi/360 + Pi/2));
        this->setY(this->y() - speed * cos(this->rotation() * TwoPi/360 + Pi/2));

        if(!scene()->collidingItems(this).isEmpty()){
            this->setX(this->x() - speed * sin(this->rotation() * TwoPi/360 + Pi/2));
            this->setY(this->y() + speed * cos(this->rotation() * TwoPi/360 + Pi/2));
        }
    }
    if(event->key() == Qt::Key_W){

        this->setX(this->x() + speed * sin(this->rotation() * TwoPi/360));
        this->setY(this->y() - speed * cos(this->rotation() * TwoPi/360));

        if(!scene()->collidingItems(this).isEmpty()){
            this->setX(this->x() - speed * sin(this->rotation() * TwoPi/360));
            this->setY(this->y() + speed * cos(this->rotation() * TwoPi/360));
        }
    }
    if(event->key() == Qt::Key_S){
        this->setX(this->x() - speed * sin(this->rotation() * TwoPi/360));
        this->setY(this->y() + speed * cos(this->rotation() * TwoPi/360));

        if(!scene()->collidingItems(this).isEmpty()){
            this->setX(this->x() + speed * sin(this->rotation() * TwoPi/360));
            this->setY(this->y() - speed * cos(this->rotation() * TwoPi/360));
        }
    }
}
