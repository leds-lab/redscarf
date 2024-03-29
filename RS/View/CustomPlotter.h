#ifndef __CUSTOMPLOTTER_H__
#define __CUSTOMPLOTTER_H__

#include "View/Plotter.h"

class QDialog;
class CustomPlotZoom;
class QCPAxis;

class CustomPlotter : public Plotter {
    Q_OBJECT
private:
    CustomPlotZoom *plotter;

public:
    CustomPlotter(QWidget *parent = 0);
    void viewGraphic(QVector<QList<DataReport *> *> *data, AnalysisOptions *aop, QStringList legends);

private slots:
    void enableGraph(bool toggled);
    void updateCoordinate(QMouseEvent *event);
    void setFontSize(int size);
    void changeLegendPosition(int);
    void exportGraphic();
    void copyGraphic();

    void setAxisLimits(QCPAxis*);

};

#endif // CUSTOMPLOTTER_H
