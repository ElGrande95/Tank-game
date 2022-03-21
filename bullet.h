#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPainter>
#include <QGraphicsScene>

class Bullet : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    enum { typeBullet = UserType + 3 };
    int type() const override;

    explicit Bullet(QPointF start, QPointF end, const int& heroType, QObject* parent = nullptr);
    ~Bullet();
    void setCallbackFunc(void (*func) (QGraphicsItem * item));

    int getHeroType() const;

    QPointF getStart() const;

signals:
    void signalTargetShot(QGraphicsItem* item);

private:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;

    void (*callbackFunc)(QGraphicsItem * item);

private:
    QTimer *timerBullet;
    int speed;
    int heroType;
    QPointF start;

private slots:
    void slotTimerBullet();
};

#endif // BULLET_H
