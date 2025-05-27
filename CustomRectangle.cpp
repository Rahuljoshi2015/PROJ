#include "CustomRectangle.h"
#include <QPainter>
#include <QDebug>

CustomRectangle::CustomRectangle() {
    rect = new QGraphicsRectItem(0, 0, 50, 50, this);
    rect->setBrush(Qt::green);
    label = new QGraphicsTextItem("", this);
    label->setPos(5, 15);
}

QRectF CustomRectangle::boundingRect() const {
    return rect->boundingRect();
}

void CustomRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    // Painting handled by child items
}

void CustomRectangle::setColor(const QColor &color) {
    qDebug() << "CustomRectangle: Setting brush to color:" << color.name();
    rect->setBrush(QBrush(color));
}

void CustomRectangle::setText(const QString &text) {
    label->setPlainText(text);
}