#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPixmap>
#include <QPainter>

class Explosion : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit Explosion(QPointF point, QObject* parent = nullptr);
    ~Explosion();

    enum { typeExplosion = UserType + 2 };

    int type() const override;

signals:

public slots:

private slots:
    void nextFrame();

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

private:
    QTimer *timer;
    QPixmap *explosionImage;
    int currentFrame;
};

#endif // EXPLOSION_H
