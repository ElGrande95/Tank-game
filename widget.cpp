#include "widget.h"
#include "ui_widget.h"
#include <QtMath>
#include <QDebug>
#include "target.h"
#include <QRandomGenerator>
#include <QFont>

#define Pi 3.14159265358979323846264338327950288419717
#define TwoPi (2.0 * Pi)

QList<QGraphicsItem *> Widget::targets;


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    this->resize(600,600);
    this->setFixedSize(600,600);
    
    ui->setupUi(this);
    
    scene = new MyScene();
    
    ui->graphicsView->setScene(scene);  /// set scene into graphicsView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    scene->setSceneRect(0,0,500,500);
    
    scene->addRect(0,0,500,20,QPen(Qt::NoPen),QBrush(Qt::darkGray));
    scene->addRect(0,0,20,500,QPen(Qt::NoPen),QBrush(Qt::darkGray));
    scene->addRect(0,480,500,20,QPen(Qt::NoPen),QBrush(Qt::darkGray));
    scene->addRect(480,0,20,500,QPen(Qt::NoPen),QBrush(Qt::darkGray));
    
    barrier = new Barrier();
    barrier->setPos(240,100);
    scene->addItem(barrier);
    
    QPixmap* pixmap = new QPixmap(":/Imgs/Images/target.png");
    QCursor cursor = QCursor(pixmap->scaled(20,20));
    ui->graphicsView->setCursor(cursor);
    hero = new MyHero();
    hero->setPos(50,50);
    scene->addItem(hero);
    hero->setFlag(QGraphicsItem::ItemIsFocusable);
    hero->setFocus();
    
    ui->graphicsView->setMouseTracking(true);
    
    connect(scene, &MyScene::signalTargetCoordinate, hero, &MyHero::slotDirection);
    
    connect(scene, &MyScene::signalShot, hero, &MyHero::slotShot);
    connect(hero, &MyHero::signalBullet, this, &Widget::slotHeroBullet);

    Target* target = new Target();
    scene->addItem(target);
    targets.append(target);

    target->setRotation(QRandomGenerator::global()->bounded(360));
    int x = QRandomGenerator::global()->bounded(275, 425);
    int y = QRandomGenerator::global()->bounded(75, 425);

    target->setPos(x,y);

    timerEnemyCreate = new QTimer();
    connect(timerEnemyCreate, &QTimer::timeout, this, &Widget::slotEnemyCreate);
    timerEnemyCreate->start(3000);

    setLabel();
    connect(hero, &MyHero::signalDamage,this, &Widget::setLabel);
    
    timerEnemyBullet = new QTimer();
    connect(timerEnemyBullet, &QTimer::timeout, this, &Widget::slotEnemyBullet);
    timerEnemyBullet->start(200);

    connect(hero, &MyHero::signalChangePos, this, &Widget::slotEnemysFire);
    connect(hero, &MyHero::signalGameOver, this, &Widget::slotGameOver);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::setLabel()
{
    QFont f = QFont();
    f.setPixelSize(32);
    ui->health->setFont(f);
    ui->health->setText("Health: " +QString::number((hero->getHealth())));
    
}

void Widget::slotHeroBullet(QPointF start, QPointF end)
{
    Bullet *bullet = new Bullet(start, end, hero->type());
    bullet->setCallbackFunc(damageTarget);
    scene->addItem(bullet);
}

void Widget::slotEnemyBullet()
{
    foreach (QGraphicsItem *targ, targets) {

        Target *t = qgraphicsitem_cast <Target *> (targ);

        int xStart = t->x() + 30 * sin(t->rotation() * TwoPi/360);
        int yStart = t->y() - 30 * cos(t->rotation() * TwoPi/360);

        int xEnd = t->x() + 60 * sin(t->rotation() * TwoPi/360);
        int yEnd = t->y() - 60 * cos(t->rotation() * TwoPi/360);

        Bullet *bullet = new Bullet(QPointF(xStart, yStart), QPointF(xEnd, yEnd), t->type());
        bullet->setCallbackFunc(nista);
        scene->addItem(bullet);
    }
}

void Widget::slotEnemysFire(QPointF point)
{

    foreach(QGraphicsItem *targ, targets) {
        Target *tar = qgraphicsitem_cast <Target *> (targ);

        if(!tar->getMove())
        {
            foreach(QGraphicsItem *targ, targets) {
                Target *t = qgraphicsitem_cast <Target *> (targ);
                t->setMove(false);
                QLineF lineDirection(t->pos(), point);
                qreal angleDirection = ::acos(lineDirection.dx() / lineDirection.length());

                if(lineDirection.dy() <= 0) {
                    if(angleDirection < Pi / 2)
                    {
                        t->setRotation(90 - angleDirection * 180 /Pi);
                    }
                    else if (angleDirection < Pi)
                    {
                        t->setRotation(450 - angleDirection * 180 /Pi);
                    }
                }
                else {
                    t->setRotation(90 + angleDirection * 180 /Pi);
                }
            }
        }
    }
}

void Widget::slotEnemyCreate()
{
    Target* target = new Target();

    scene->addItem(target);

    targets.append(target);

    target->setRotation(QRandomGenerator::global()->bounded(360));
    int x = QRandomGenerator::global()->bounded(275, 425);
    int y = QRandomGenerator::global()->bounded(75, 425);

    target->setPos(x,y);

    QList<QGraphicsItem *> foundItems = scene->collidingItems(target);

    while(!foundItems.isEmpty())
    {
        x = QRandomGenerator::global()->bounded(275, 425);
        y = QRandomGenerator::global()->bounded(75, 425);

        target->setPos(x,y);
        foundItems = scene->collidingItems(target);
    }

}

void Widget::slotGameOver()
{

}

void Widget::damageTarget(QGraphicsItem *item)
{
    foreach (QGraphicsItem *targ, targets) {
        if(targ == item){
            Target *t = qgraphicsitem_cast <Target *> (targ);
            t->hit(QRandomGenerator::global()->bounded(1,5));
            if(t->getHealth() == 0)
                targets.removeOne(targ);
        }
    }
}

void Widget::nista(QGraphicsItem *item)
{
    Q_UNUSED(item)
}
