#ifndef TARGET_H
#define TARGET_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>

class Target : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem);
public:
    explicit Target(QObject *parent = 0);
    ~Target();

    void hit(int damage);

    int getHealth() const;

signals:
    void signalHit();

public slots:

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    int health;
    int maxHealth;
};

#endif // TARGET_H
