#include "include/View/CustomPlotter.h"
#include "include/View/qcustomplot.h"
#include "include/View/AnalysisOptions.h"
#include "include/Model/Analysis/DataReport.h"

CustomPlotter::CustomPlotter(QWidget* parent) : Plotter(parent) {

    this->plotter = new QCustomPlot(this);

    this->setCentralWidget(plotter);

    //GUI

}

void CustomPlotter::viewGraphic(QVector<QList<DataReport *> *> *data, AnalysisOptions *aop, QStringList legends) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/QwtPlotter::viewGraphic" << std::endl;
#endif

//    plotter->setTitle( aop->getTitle() );
//    plotter->setAxisTitle( QwtPlot::yLeft, aop->getYAxisLabel() );
//    plotter->setAxisTitle( QwtPlot::xBottom, aop->getXAxisLabel() );
    srand(time_t(NULL));

    for(int i = 0; i < data->size(); i++) {
        QList<DataReport *>* d = data->at(i);

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
        Qt::PenStyle lineStyle;
        switch( i%5 ) {
            case 0:
                shape = QCPScatterStyle::ssCircle;
                lineStyle = Qt::SolidLine;
                break;
            case 1:
                shape = QCPScatterStyle::ssSquare;
                lineStyle = Qt::DashLine;
                break;
            case 2:
                shape = QCPScatterStyle::ssCross;
                lineStyle = Qt::DotLine;
                break;
            case 3:
                shape = QCPScatterStyle::ssTriangle;
                lineStyle = Qt::DashDotLine;
                break;
            case 4:
                shape = QCPScatterStyle::ssPlus;
                lineStyle = Qt::DashDotDotLine;
                break;
            default:
                shape = QCPScatterStyle::ssDiamond;
                lineStyle = Qt::SolidLine;

        }


        // Define curve
        QColor cor = QColor( aop->getColor(i) );
        plotter->addGraph();
        plotter->graph(i)->setData(xAxis,yAxis);
        plotter->graph(i)->setName(legends.at(i)); //verificar se name corresponde mesmo ao setTitle
//        curve->setTitle( legends.at(i) );
        plotter->graph(i)->setAntialiased(true); //verificar se é necessário


        // Define point
        QCPScatterStyle scatterStyle = QCPScatterStyle(shape,cor,aop->getPointSize() * 6);
        plotter->graph(i)->setScatterStyle(scatterStyle);
//        curve->setLegendAttribute(QwtPlotCurve::LegendShowLine);
//        curve->setLegendAttribute(QwtPlotCurve::LegendShowSymbol);
//        curve->setLegendIconSize( QSize(20,20) );
        QPen pen = QPen(QBrush(cor), aop->getLineWidth(), lineStyle);
        plotter->graph(i)->setPen(pen);
        plotter->graph(i)->rescaleAxes();

//        if( aop->isLatencyDistribution() ) {
//           curve->setStyle( QwtPlotCurve::Sticks );
//        }
        // end

    }


//    QList<QwtLegendLabel*> objList = plotter->findChildren<QwtLegendLabel*>();
//    for( int i = 0; i < objList.size(); i++ ) {
//        QwtLegendLabel* legLabel = objList.at(i);
//        legLabel->setChecked(true);
//    }

//    new QwtPlotZoomer( plotter->canvas() );
    plotter->replot();
    this->show();

}

void CustomPlotter::exportGraphic(QImage image){

}
