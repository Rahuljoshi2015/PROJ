#include "CustomRectangle.h"
#include <QPainter>
#include <QFont>
#include <QBrush>

CustomRectangle::CustomRectangle() {
    // Create the rectangle
    rect = new QGraphicsRectItem(0, 0, 50, 50);
    rect->setBrush(QBrush(Qt::lightGray)); // Default color is light gray
    rect->setPen(QPen(Qt::black)); // Black border

    // Create the text label
    label = new QGraphicsTextItem();
    label->setFont(QFont("Arial", 12)); // Set the font of the label
}

void CustomRectangle::setColor(const QColor &color) {
    rect->setBrush(QBrush(color));  // Set the fill color of the rectangle
}

void CustomRectangle::setText(const QString &text) {
    label->setPlainText(text); // Set the text label
}

QRectF CustomRectangle::boundingRect() const {
    return rect->boundingRect(); // Return the bounding box of the rectangle
}

void CustomRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    // Paint the rectangle
    rect->paint(painter, option, widget);
    
    // Paint the label text on top of the rectangle
    label->setPos(rect->rect().x() + 10, rect->rect().y() + 10);  // Adjust position of the text
    label->paint(painter, option, widget);
}
