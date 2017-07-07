#include "PersonalGraphicsView.h"

#include <QApplication>
#include <QWheelEvent>

#include <QGraphicsItem>

PersonalGraphicsView::PersonalGraphicsView(QWidget *parent)
    : QGraphicsView(parent),
      zoomFactor(0.15f)
{}

void PersonalGraphicsView::selectAllItems() {
    QList<QGraphicsItem *> allItems = this->items();
    for( int i = 0; i < allItems.size(); i++ ) {
        allItems.at(i)->setSelected(true);
    }
}

void PersonalGraphicsView::wheelEvent(QWheelEvent *event) {
    if(QApplication::keyboardModifiers() & Qt::ControlModifier) {
        if( event->delta() > 0 ) {
            this->zoomIn();
        } else {
            this->zoomOut();
        }
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void PersonalGraphicsView::zoomIn() {
    this->scale(1.0f+zoomFactor,1.0f+zoomFactor);
}

void PersonalGraphicsView::zoomOut() {
    this->scale(1.0f-zoomFactor,1.0f-zoomFactor);
}

void PersonalGraphicsView::setZoomFactor(double newFactor) {
    this->zoomFactor = newFactor;
}
