#include "widget.h"
#include "ui_widget.h"
#include <QtMath>
#include <QDebug>
#include <QRandomGenerator>
#include <QFont>
#include <QMessageBox>
#include "gameover.h"
#include "highscore.h"

#define Pi 3.14159265358979323846264338327950288419717
#define TwoPi (2.0 * Pi)

QList<QGraphicsItem *> Widget::targets;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    this->resize(600,600);
    this->setFixedSize(600,600);

    enemyDestroy = -1;

    ui->setupUi(this);
    scene = new MyScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    scene->setSceneRect(0,0,500,500);
    scene->setFocus();

    QPixmap bgImages(":/Imgs/Images/tank2.jpeg");
    bgImages = bgImages.scaled(600, 600);

    ui->graphicsView->setStyleSheet("QGraphicsView { background-image: url(:/Imgs/Images/battle.jpg) 500 500;"
                                    "color: white;"
                                    "opacity: 200;"
                                    "font: bold 24px;};");

    Wall* wallLeft = new Wall();
    wallLeft->setPos(0,0);
    scene->addItem(wallLeft);

    Wall* wallRight = new Wall();
    wallRight->setPos(480,20);
    scene->addItem(wallRight);

    Wall* wallTop = new Wall();
    wallTop->setPos(500,0);
    wallTop->setRotation(90);
    scene->addItem(wallTop);

    Wall* wallBottom = new Wall();
    wallBottom->setPos(480,480);
    wallBottom->setRotation(90);
    scene->addItem(wallBottom);

    barrier = new Barrier();
    barrier->setPos(240,100);
    scene->addItem(barrier);
    
    ui->label->setStyleSheet("QLabel { background-color: gray;};");

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
    connect(target, &Target::signalDestroy, this, &Widget::setLabelDestroyed);

    target->setRotation(QRandomGenerator::global()->bounded(360));
    int x = QRandomGenerator::global()->bounded(275, 425);
    int y = QRandomGenerator::global()->bounded(75, 425);

    target->setPos(x,y);

    timerEnemyCreate = new QTimer();
    connect(timerEnemyCreate, &QTimer::timeout, this, &Widget::slotEnemyCreate);
    timerEnemyCreate->start(3000);

    setLabelDestroyed();

    setLabelHealth();
    connect(hero, &MyHero::signalDamage,this, &Widget::setLabelHealth);

    timerEnemyBullet = new QTimer();
    connect(timerEnemyBullet, &QTimer::timeout, this, &Widget::slotEnemyBullet);
    timerEnemyBullet->start(200);

    connect(hero, &MyHero::signalChangePos, this, &Widget::slotEnemysFire);
    connect(hero, &MyHero::signalGameOver, this, &Widget::slotGameOver);

    timerElapsed.start();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setLabelHealth()
{
    QFont f = QFont();
    f.setPixelSize(32);
    ui->health->setFont(f);
    ui->health->setText("Health: " +QString::number((hero->getHealth())));
}

void Widget::setLabelDestroyed()
{
    enemyDestroy++;

    QFont f = QFont();
    f.setPixelSize(32);
    ui->enemyDestroy->setFont(f);
    ui->enemyDestroy->setText("Destroyed: " +QString::number(enemyDestroy));
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
        Target *t = qgraphicsitem_cast <Target *> (targ);

        if(!t->getMove())
        {
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

void Widget::slotEnemyCreate()
{
    if(targets.length() == 1)
        return;

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

    connect(target, &Target::signalDestroy, this, &Widget::setLabelDestroyed);
}

void Widget::slotGameOver()
{
    timerEnemyCreate->deleteLater();
    timerEnemyBullet->deleteLater();

    foreach (QGraphicsItem *targ, targets) {
        Target *t = qgraphicsitem_cast <Target *> (targ);
        targets.removeOne(targ);
        t->deleteLater();
    }

    GameOver* w = new GameOver(this->parentWidget());
    double time = static_cast<double>(timerElapsed.elapsed()) / 1000;
    w->setTimeLabel(time);
    w->setTime(time);
    w->setDestroyLabel(enemyDestroy);
    w->setDestroy(enemyDestroy);

    HighScore* db = new HighScore();

    if(!db->checkBase(enemyDestroy, time)){
        w->hideUserScore();

    }

    w->show();

    db->deleteLater();
    delete this;
}

void Widget::damageTarget(QGraphicsItem *item)
{
    foreach (QGraphicsItem *targ, targets) {
        if(targ == item){
            Target *t = qgraphicsitem_cast <Target *> (targ);
            t->hit(QRandomGenerator::global()->bounded(1,5));
            if(t->getHealth() == 0) {
                targets.removeOne(targ);
                t->deleteLater();
            }
        }
    }
}

void Widget::nista(QGraphicsItem *item)
{
    Q_UNUSED(item)
}


//void Widget::keyPressEvent(QKeyEvent *event)
//{
//    int speed = hero->getSpeed();
//    hero->setRotation(normalizeAngleDeg(hero->rotation()));

//    if(event->key() == Qt::Key_A){
//        hero->setX(hero->x() + speed * sin(hero->rotation() * TwoPi/360 - Pi/2));
//        hero->setY(hero->y() - speed * cos(hero->rotation() * TwoPi/360 - Pi/2));

//        if(!scene->collidingItems(hero).isEmpty()){
//            hero->setX(hero->x() - speed * sin(hero->rotation() * TwoPi/360 - Pi/2));
//            hero->setY(hero->y() + speed * cos(hero->rotation() * TwoPi/360 - Pi/2));
//        }
//    }
//    if(event->key() == Qt::Key_D){

//        hero->setX(hero->x() + speed * sin(hero->rotation() * TwoPi/360 + Pi/2));
//        hero->setY(hero->y() - speed * cos(hero->rotation() * TwoPi/360 + Pi/2));

//        if(!scene->collidingItems(hero).isEmpty()){
//            hero->setX(hero->x() - speed * sin(hero->rotation() * TwoPi/360 + Pi/2));
//            hero->setY(hero->y() + speed * cos(hero->rotation() * TwoPi/360 + Pi/2));
//        }
//    }
//    if(event->key() == Qt::Key_W){

//        hero->setX(hero->x() + speed * sin(hero->rotation() * TwoPi/360));
//        hero->setY(hero->y() - speed * cos(hero->rotation() * TwoPi/360));

//        if(!scene->collidingItems(hero).isEmpty()){
//            hero->setX(hero->x() - speed * sin(hero->rotation() * TwoPi/360));
//            hero->setY(hero->y() + speed * cos(hero->rotation() * TwoPi/360));
//        }
//    }
//    if(event->key() == Qt::Key_S){
//        hero->setX(hero->x() - speed * sin(hero->rotation() * TwoPi/360));
//        hero->setY(hero->y() + speed * cos(hero->rotation() * TwoPi/360));

//        if(!scene->collidingItems(hero).isEmpty()){
//            hero->setX(hero->x() + speed * sin(hero->rotation() * TwoPi/360));
//            hero->setY(hero->y() - speed * cos(hero->rotation() * TwoPi/360));
//        }
//    }
//}
