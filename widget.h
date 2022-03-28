#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "myhero.h"
#include "myscene.h"
#include <QGraphicsScene>
#include <bullet.h>
#include "barrier.h"
#include "target.h"
#include <QElapsedTimer>
#include "wall.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void setLabelHealth();
    void setLabelDestroyed();

    //    void keyPressEvent(QKeyEvent *event) override;

signals:
    void signalDestroy();

private:
    Ui::Widget *ui;
    MyScene* scene;
    MyHero* hero;
    Barrier* barrier;
    int enemyDestroy;
    int numberEnemy;

    QElapsedTimer timerElapsed;

    QTimer* timerEnemyBullet;
    QTimer* timerEnemyCreate;
    QTimer* timerGameOver;


    static QList<QGraphicsItem *> targets;

    static void damageTarget(QGraphicsItem* item);
    static void nista(QGraphicsItem* item);

    void buildWalls();

private slots:
    void slotHeroBullet(QPointF start, QPointF end);
    void slotEnemyBullet();
    void slotEnemysFire(QPointF point);
    void slotEnemyCreate();
    void slotGameOver();
    void slotTimerGameOver();

};
#endif // WIDGET_H
