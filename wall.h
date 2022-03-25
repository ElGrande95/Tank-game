#ifndef WALL_H
#define WALL_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPainter>
#include <QGraphicsScene>

class Wall : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:

    explicit Wall(QObject* parent = nullptr);
    ~Wall();

    enum { typeWall = UserType + 7 };

    int type() const override;

private:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;
};

#endif // WALL_H
