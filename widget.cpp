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

    scene   = new MyScene();    // Init custom scene

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
    connect(hero, &MyHero::signalBullet, this, &Widget::slotBullet);

    Target* target1 = new Target();
    Target* target2 = new Target();

    target1->setPos(300, 100);
    target2->setPos(300, 400);

    scene->addItem(target1);
    scene->addItem(target2);

    targets.append(target1);
    targets.append(target2);

    setLabel();
    connect(hero, &MyHero::signalDamage,this, &Widget::setLabel);

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

void Widget::slotBullet(QPointF start, QPointF end)
{

    Bullet *bullet = new Bullet(start, end, hero);
    bullet->setCallbackFunc(damageTarget);
    scene->addItem(bullet);
}



void Widget::damageTarget(QGraphicsItem *item)
{
    foreach (QGraphicsItem *targ, targets) {
        if(targ == item){
            Target *t = qgraphicsitem_cast <Target *> (targ);
            t->hit(QRandomGenerator::global()->bounded(1,3));
        }
    }

}
