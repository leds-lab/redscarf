#ifndef __PERSONALGRAPHICSVIEW_H__
#define __PERSONALGRAPHICSVIEW_H__

#include <QGraphicsView>

class PersonalGraphicsView : public QGraphicsView {
    Q_OBJECT
private:
    double zoomFactor;

public:
    PersonalGraphicsView(QWidget* parent = Q_NULLPTR);

public slots:
    void zoomIn();
    void zoomOut();

    void setZoomFactor(double newFactor);
    void selectAllItems();

protected:
    void wheelEvent(QWheelEvent*);

};

#endif // PERSONALGRAPHICSVIEW_H
