#ifndef CUSTOMPLOTTER_H
#define CUSTOMPLOTTER_H

#include "include/View/Plotter.h"

class QDialog;
class QCustomPlot;

class CustomPlotter : public Plotter {
private:
    QCustomPlot *plotter;

public:
    CustomPlotter(QWidget *parent = 0);
    void viewGraphic(QVector<QList<DataReport *> *> *data, AnalysisOptions *aop, QStringList legends);
    void exportGraphic(QImage image);
};

#endif // CUSTOMPLOTTER_H
