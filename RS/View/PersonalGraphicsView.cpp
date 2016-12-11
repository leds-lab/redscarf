#include "View/PersonalGraphicsView.h"

#include <QApplication>
#include <QWheelEvent>

PersonalGraphicsView::PersonalGraphicsView(QWidget *parent)
    : QGraphicsView(parent),
      zoomFactor(0.15f)
{
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
//#include <QDebug>
//void PersonalGraphicsView::focusOutEvent(QFocusEvent *event) {
//    qDebug() << "focus out";
//    QGraphicsView::focusOutEvent(event);
//}
