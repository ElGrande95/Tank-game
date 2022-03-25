#ifndef TARGET_H
#define TARGET_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QTimer>
#include "widget.h"

class Target : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem);
public:

    enum { typeTarget = UserType + 2 };
    int type() const override;

    explicit Target(QObject *parent = 0);
    ~Target();

    void hit(int damage);

    int getHealth() const;
    bool getMove() const;
    void setMove(bool newMove);

signals:
    void signalHit();
    void signalDestroy();

private slots:
    void slotGameEnemy();
    void slotEnemyFocus();

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;

private:
    int health;
    int maxHealth;
    int speed;
    bool move;

    void setPosition();
    QTimer* timerGameEnemy;

};

#endif // TARGET_H
