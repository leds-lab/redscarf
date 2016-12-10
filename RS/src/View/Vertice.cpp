#include "include/View/Vertice.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>


Vertice::Vertice(int id, QColor color) {
    this->id = id;
    this->color = color;
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setFlag(QGraphicsItem::ItemIsFocusable);

    this->selected = color.darker();
}

QRectF Vertice::boundingRect() const {

    return QRectF(-SIZE/2,-SIZE/2,SIZE,SIZE);

}

QPainterPath Vertice::shape() const {

    QPainterPath path;
    path.addEllipse(-SIZE/2,-SIZE/2,SIZE,SIZE);
    return path;

}

void Vertice::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {

    if( this->isSelected() ) {
        painter->setBrush( selected );
    } else {
        painter->setBrush(color);
    }

    painter->drawEllipse(-SIZE/2,-SIZE/2,SIZE,SIZE);

    painter->setBrush(Qt::black);
    QString vertId = QString::number(this->id);

    painter->drawText( QRect(-SIZE/2,-SIZE/2,SIZE,SIZE),
                       Qt::AlignHCenter|Qt::AlignVCenter,vertId );

}

void Vertice::setColor(QColor cor) {
    this->color = cor;
}

QColor Vertice::getColor() {
    return this->color;
}
