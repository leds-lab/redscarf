#ifndef CUSTOMPLOTTER_H
#define CUSTOMPLOTTER_H

#include "include/View/Plotter.h"

class QDialog;
class CustomPlotZoom;

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

};

#endif // CUSTOMPLOTTER_H
