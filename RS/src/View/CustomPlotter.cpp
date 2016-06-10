#include "include/View/CustomPlotter.h"
#include "include/View/CustomPlotZoom.h"
#include "include/View/AnalysisOptions.h"
#include "include/Model/Analysis/DataReport.h"

//#include "ui_Plotter.h"

#include <QCheckBox>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

CustomPlotter::CustomPlotter(QWidget* parent) : Plotter(parent) {

    this->plotter = new CustomPlotZoom(this);
    this->ui->plotWidget->layout()->addWidget(plotter);

    plotter->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plotter->setAutoAddPlottableToLegend(true);
    plotter->legend->setVisible(true);
    plotter->setZoomMode(true);

    connect(plotter,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(updateCoordinate(QMouseEvent*)));

    QAction *exportGraph = this->ui->menubar->addAction(trUtf8("Export graph"));
    connect(exportGraph,SIGNAL(triggered(bool)),this,SLOT(exportGraph()));
}

void CustomPlotter::viewGraphic(QVector<QList<DataReport *> *> *data, AnalysisOptions *aop, QStringList legends) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/QwtPlotter::viewGraphic" << std::endl;
#endif

    plotter->plotLayout()->insertRow(0);
    plotter->plotLayout()->addElement(0, 0, new QCPPlotTitle(plotter, aop->getTitle()));
    plotter->xAxis->setLabel(aop->getXAxisLabel());
    plotter->yAxis->setLabel(aop->getYAxisLabel());

    srand(time_t(NULL));

    for(int i = 0; i < data->size(); i++) {
        QList<DataReport *>* d = data->at(i);

        QCheckBox* checkBox = new QCheckBox(legends.at(i));
        checkBox->setProperty("index",i);
        checkBox->setChecked(true);
        QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(this->ui->legend->layout());
        layout->insertWidget(i, checkBox);
        connect(checkBox,SIGNAL(toggled(bool)),this,SLOT(enableGraph(bool)));

        // Start define curves
        int countData = d->size();
        QVector<double> xAxis, yAxis;
        for( int x = 0; x < countData; x++ ) {
            DataReport* dr = d->at(x);
            if( aop->isLatencyDistribution() ) {
                xAxis.append(dr->latencyCycle);
                yAxis.append(dr->packetCount);
            } else {

                // Selected Graphic
                switch( aop->getXAxis() ) {
                    case 0 : // FClk
                        xAxis.append( dr->fClk );
                        break;
                    case 1: // Offered traffic
                        xAxis.append( dr->avgRequiredBwNorm );
                        break;
                    case 2: // Accepted Traffic (f/c/n)
                        xAxis.append( dr->acceptedTrafficFlits );
                        break;
                    case 3: // Accepted Traffic (Mbps/n)
                        xAxis.append( dr->acceptedTrafficBps );
                        break;
                }

                switch ( aop->getYAxis() ) {
                    case 0: // Average latency (cycles)
                        yAxis.append(dr->avgLatencyCycles);
                        break;
                    case 1: // Average latency (ns)
                        yAxis.append(dr->avgLatencyNs);
                        break;
                    case 2: // Accepted Traffic (f/c/n)
                        yAxis.append(dr->acceptedTrafficFlits);
                        break;
                    case 3: // Accepted Traffic (Mbps/n)
                        yAxis.append(dr->acceptedTrafficBps);
                        break;
                    case 4: // % Met deadlines RT0
                        yAxis.append(dr->metDeadlinesPer0);
                        break;
                    case 5: // % Met deadlines RT1
                        yAxis.append(dr->metDeadlinesPer1);
                        break;
                    case 6: // % Met deadlines RT2
                        yAxis.append(dr->metDeadlinesPer2);
                        break;
                    case 7: // % Met deadlines RT3
                        yAxis.append(dr->metDeadlinesPer3);
                        break;
                }
            }
        }

        // Define point style - 5 options
        // Define line style - 5 options
        QCPScatterStyle::ScatterShape shape;
        Qt::PenStyle penStyle;
        switch( i%5 ) {
            case 0:
                shape = QCPScatterStyle::ssCircle;
                penStyle = Qt::SolidLine;
                break;
            case 1:
                shape = QCPScatterStyle::ssSquare;
                penStyle = Qt::DashLine;
                break;
            case 2:
                shape = QCPScatterStyle::ssCross;
                penStyle = Qt::DotLine;
                break;
            case 3:
                shape = QCPScatterStyle::ssTriangle;
                penStyle = Qt::DashDotLine;
                break;
            case 4:
                shape = QCPScatterStyle::ssPlus;
                penStyle = Qt::DashDotDotLine;
                break;
            default:
                shape = QCPScatterStyle::ssDiamond;
                penStyle = Qt::SolidLine;

        }


        // Define curve
        QColor cor = QColor( aop->getColor(i) );
        plotter->addGraph();
        plotter->graph(i)->setData(xAxis,yAxis);
        plotter->graph(i)->setName(legends.at(i));
        plotter->graph(i)->setAntialiased(true); //verificar se é necessário
        plotter->rescaleAxes();

        // Define point
        QCPScatterStyle scatterStyle = QCPScatterStyle(shape,cor,aop->getPointSize() * 6);
        plotter->graph(i)->setScatterStyle(scatterStyle);
        QPen pen = QPen(QBrush(cor), aop->getLineWidth(), penStyle);
        plotter->graph(i)->setPen(pen);

        if( aop->isLatencyDistribution() ) {
            plotter->graph(i)->setLineStyle(QCPGraph::lsImpulse);
        }
        // end

    }

    plotter->replot();
    this->show();

}

void CustomPlotter::enableGraph(bool toggled) {

    int index = sender()->property("index").toInt();

    plotter->graph(index)->setVisible(toggled);
    plotter->legend->clearItems();

    for( int i = 0; i < plotter->graphCount(); i++ ) {
        if(plotter->graph(i)->visible()){
            plotter->graph(i)->addToLegend();
        }
    }

    if(plotter->legend->itemCount() == 0){
        plotter->legend->setVisible(false);
    }else{
        plotter->legend->setVisible(true);
    }

    plotter->replot();
    this->show();
}

void CustomPlotter::updateCoordinate(QMouseEvent *event){

    double x = plotter->xAxis->pixelToCoord(event->pos().x());
    double y = plotter->yAxis->pixelToCoord(event->pos().y());

    this->ui->statusbar->showMessage(QString("%1 , %2").arg(x).arg(y));
}

void CustomPlotter::exportGraph(){

    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this,trUtf8("Export graph"), QDir::homePath(),trUtf8("PNG image (*.png);;JPG image (*.jpg);;PDF document (*.pdf)"),&selectedFilter);

    if(fileName.isEmpty()) {
        return;
    }
    if(selectedFilter.endsWith("(*.png)")){
        plotter->savePng(fileName);
    }else if(selectedFilter.endsWith("(*.jpg)")){
        plotter->saveJpg(fileName);
    }else if(selectedFilter.endsWith("(*.pdf)")){
        plotter->savePdf(fileName);
    }
}
