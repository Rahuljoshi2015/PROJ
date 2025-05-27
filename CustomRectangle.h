#ifndef CUSTOMRECTANGLE_H
#define CUSTOMRECTANGLE_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QColor>
#include <QString>

class CustomRectangle : public QGraphicsItem {
public:
    CustomRectangle();
    void setColor(const QColor &color);
    void setText(const QString &text);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QGraphicsRectItem* getRect() const { return rect; } // Required for animation

private:
    QGraphicsRectItem *rect;
    QGraphicsTextItem *label;
};

#endif // CUSTOMRECTANGLE_H