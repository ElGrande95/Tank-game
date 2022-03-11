#ifndef MYHERO_H
#define MYHERO_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>

class MyHero : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit MyHero(QObject* parent = nullptr);
    ~MyHero();

    enum { typeMyHero = UserType + 1 };

    int getSpeed() const;
    int type() const override;
    void damage(int dmg);
    void keyPressEvent(QKeyEvent *event);

    int getHealth() const;

private:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

signals:
    void signalBullet(QPointF start, QPointF end);
    void signalDamage();

public slots:
    void slotDirection(QPointF point);
    void slotShot(bool shot);

private slots:
    void slotGameTimer();
    void slotBulletTimer();

private:
    bool shot;
    QTimer *bulletTimer;
    QTimer *gameTimer;
    int speed;
    QPointF direction;

    int health;
    int maxHealth;

};

#endif // MYHERO_H
