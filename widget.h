#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "myhero.h"
#include "myscene.h"
#include <QGraphicsScene>
#include <bullet.h>
#include "barrier.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void setLabel();

private:
    Ui::Widget *ui;
    MyScene* scene;
    MyHero* hero;
    Barrier* barrier;

    static QList<QGraphicsItem *> targets;

    static void damageTarget(QGraphicsItem* item);

private slots:
    void slotBullet(QPointF start, QPointF end);
};
#endif // WIDGET_H
