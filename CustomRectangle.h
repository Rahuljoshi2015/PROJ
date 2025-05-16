#ifndef CUSTOMRECTANGLE_H 
#define CUSTOMRECTANGLE_H

#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QBrush>

class CustomRectangle : public QGraphicsItem {
public:
    CustomRectangle();
    
    // Set color for the rectangle
    void setColor(const QColor &color);

    // Set text on the rectangle
    void setText(const QString &text);

    // Override bounding rect for QGraphicsItem
    QRectF boundingRect() const override;

    // Override paint for custom drawing
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QGraphicsRectItem *rect;  // Rectangle item
    QGraphicsTextItem *label; // Label text on the rectangle
};

#endif // CUSTOMRECTANGLE_H
