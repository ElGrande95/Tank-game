#ifndef TARGETDESTROY_H
#define TARGETDESTROY_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPixmap>
#include <QPainter>

class TargetDestroy : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit TargetDestroy(QPointF point, QObject* parent = nullptr);
    ~TargetDestroy();

    enum { typeDestroy = UserType + 6 };

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

#endif // TARGETDESTROY_H
