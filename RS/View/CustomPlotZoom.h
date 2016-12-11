#ifndef __CUSTOM_PLOT_ZOOM__
#define __CUSTOM_PLOT_ZOOM__

#include <QPoint>
#include "View/qcustomplot.h"

class QRubberBand;
class QMouseEvent;
class QWidget;

class CustomPlotZoom : public QCustomPlot
{
    Q_OBJECT

public:
    CustomPlotZoom(QWidget * parent = 0);
    virtual ~CustomPlotZoom();

    void setZoomMode(bool mode);

private slots:
    void mousePressEvent(QMouseEvent * event) override;
    void mouseMoveEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;

private:
    bool mZoomMode;
    QRubberBand * mRubberBand;
    QPoint mOrigin;
};

#endif // __CUSTOM_PLOT_ZOOM__
