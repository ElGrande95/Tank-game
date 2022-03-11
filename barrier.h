#ifndef BARRIER_H
#define BARRIER_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPainter>
#include <QGraphicsScene>

class Barrier : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:

    explicit Barrier(QObject* parent = nullptr);
    ~Barrier();

    enum { typeBarrier = UserType + 4 };

    int type() const override;

private:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;

private:
    QTimer *timerBarrier;
    int speed;
    bool directionDown;


private slots:
    void slotBarrier();
};

#endif // BARRIER_H
