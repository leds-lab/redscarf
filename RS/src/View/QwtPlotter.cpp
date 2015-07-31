/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* QwtPlotter.cpp
* Copyright (C) 2014 LEDS - Univali <zeferino@univali.br>
* Laboratory of Embedded and Distributed Systems
* University of Vale do Itajaí
*
* Authors: Laboratory of Embedded and Distributed Systems (LEDS)
*
* Contact: Profº Dr. Cesar Albenes Zeferino {zeferino@univali.br}
*
* RedScarf is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* RedScarf is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* ------------------------------ Reviews -------------------------------------
* Date       - Version - Author                      | Description
* ----------------------------------------------------------------------------
* 10/12/2014 - 1.0     - Eduardo Alves da Silva      | Initial release
*
*/

#include "include/View/QwtPlotter.h"

#include <qwt_plot.h>
#include <qwt_legend.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_legend_label.h>

#include <QVBoxLayout>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QMessageBox>

#include "include/View/AnalysisOptions.h"
#include "include/View/PersonalQwtPlotPicker.h"
#include "include/Model/Analysis/DataReport.h"
#include "include/Control/Control.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

QwtPlotter::QwtPlotter(QWidget *parent) : Plotter(parent) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class View/QwtPlotter" << std::endl;
#endif

    this->window = new QDialog(parent,Qt::Window);
    this->plotter = new QwtPlot(window);
    QVBoxLayout* layout = new QVBoxLayout(window);
    QToolBar* toolBar = new QToolBar("QwtPlotter Tool Bar");
    QStatusBar* statusBar = new QStatusBar(window);

    layout->addWidget(toolBar);
    layout->addWidget(plotter);
    layout->addWidget(statusBar);
    QAction* exportAction = new QAction(trUtf8("Export"),window);
    exportAction->setShortcut( QKeySequence::Save );
    QIcon icon = QIcon( QStringLiteral(":/icons/icons/icon_save.png") );
    exportAction->setIcon( icon );
    connect(exportAction,SIGNAL(triggered()),this,SLOT(exportPlot()));
    toolBar->addAction(exportAction);

    window->addAction(exportAction);

    plotter->setCanvasBackground( Qt::white );
    QwtLegend* leg = new QwtLegend(plotter);
    leg->setDefaultItemMode( QwtLegendData::Checkable );
    connect(leg,SIGNAL(checked(QVariant,bool,int)),this,SLOT(legendChecked(QVariant,bool,int)));
    plotter->insertLegend( leg );
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen( QColor(Qt::black),0.0, Qt::DotLine );
    grid->attach(plotter);

    PersonalQwtPlotPicker* picker = new PersonalQwtPlotPicker(plotter->xBottom,plotter->yLeft,QwtPicker::CrossRubberBand,QwtPicker::AlwaysOn,plotter->canvas());
    connect(picker,SIGNAL(mouseMoved(QString)),statusBar,SLOT(showMessage(QString)));

    new QwtPlotMagnifier(plotter->canvas());

    window->resize(600,400);

}

void QwtPlotter::viewGraphic(AnalysisOptions *aop, QStringList dirs) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/QwtPlotter::viewGraphic" << std::endl;
#endif

    plotter->setTitle( aop->getTitle() );
    plotter->setAxisTitle( QwtPlot::yLeft, aop->getYAxisLabel() );
    plotter->setAxisTitle( QwtPlot::xBottom, aop->getXAxisLabel() );

    srand(time_t(NULL));

    QStringList legendas;
    int size = dirs.size();
    for( int i = 0; i < size; i++ ) {
        QString dir = dirs.at(i);

        QString legenda;
        if( aop->isLatencyDistribution() ) {
            int lio = dir.lastIndexOf("/");
            QString diretorioExperimento = dir.left( lio );
            legenda = aop->getLegend( diretorioExperimento );
        } else {
            legenda = aop->getLegend( dir );
        }

        if( legenda.isEmpty() ) {
            if(aop->isLatencyDistribution()) {
                legenda = QString("Curve#%1").arg(i+1);
            } else {
                legenda = QString("Config#%1").arg(i+1);
            }
        }

        if( legendas.contains( legenda ) ) {
            legenda += QString("#%1").arg(i+1);
        }

        if( aop->isLatencyDistribution() ) {
            int lio = dir.lastIndexOf("/") + 1;
            legenda += QString(" @ %1").arg( dir.mid(lio) );
        }

        legendas.append(legenda);

        QString filename = dir+"/Results";
        switch( aop->getFlowOp() ) {
            case AnalysisOptions::AllFlows:
                filename += "/summary";
                break;
            case AnalysisOptions::RT0:
            case AnalysisOptions::RT1:
            case AnalysisOptions::nRT0:
            case AnalysisOptions::nRT1:
                filename += QString("/class_%1").arg(aop->getFlowOp() - 1);
                break;
            case AnalysisOptions::Specified:
                filename += QString("/flow_%1_%2_%3_%4_%5")
                        .arg(aop->getXSrc())
                        .arg(aop->getYSrc())
                        .arg(aop->getXDest())
                        .arg(aop->getYDest())
                        .arg( aop->getTrafficPattern() );
                break;

        }
        if(aop->isLatencyDistribution()) {
            filename += "_latency_histogram";
        }

        QByteArray byteArrayReport = filename.toUtf8();
        const char* strFile = byteArrayReport.constData();
        QList<DataReport*>* data = NULL;
        if( aop->isLatencyDistribution() ) {
            data = Control::readLatencyDistributionReport(strFile);
        } else {
            data = Control::readReport( strFile );
        }

        if( data == NULL ) {
            emit sendMessage(trUtf8("<font color=red>Report file %1 unavailable or this flow is null (no packet was transfered)</font>").arg(filename));
            return;
        }

        // Start define curves
        int countData = data->size();
        QPolygonF points;
        for( int x = 0; x < countData; x++ ) {
            DataReport* dr = data->at(x);
            if( aop->isLatencyDistribution() ) {
            // Desconsiderar os nomes dos atributos utilizados, pois foram os utilizados na leitura da distribuição de latência
                points.append( QPointF( dr->accNbOfPck, dr->fClk ) );
            } else {

                QPointF ponto;
                // Selected Graphic
                switch( aop->getXAxis() ) {
                    case 0 : // FClk
                        ponto.setX( dr->fClk );
                        break;
                    case 1: // Offered traffic
                        ponto.setX( dr->avgRequiredBwNorm );
                        break;
                    case 2: // Accepted Traffic (f/c/n)
                        ponto.setX( dr->acceptedTrafficFlits );
                        break;
                    case 3: // Accepted Traffic (Mbps/n)
                        ponto.setX( dr->acceptedTrafficBps );
                        break;
                }

                switch ( aop->getYAxis() ) {
                    case 0: // Average latency (cycles)
                        ponto.setY(dr->avgLatencyCycles);
                        break;
                    case 1: // Average latency (ns)
                        ponto.setY(dr->avgLatencyNs);
                        break;
                    case 2: // Accepted Traffic (f/c/n)
                        ponto.setY(dr->acceptedTrafficFlits);
                        break;
                    case 3: // Accepted Traffic (Mbps/n)
                        ponto.setY(dr->acceptedTrafficBps);
                        break;
                    case 4: // % Met deadlines RT0
                        ponto.setY(dr->metDeadlinesPer0);
                        break;
                    case 5: // % Met deadlines RT1
                        ponto.setY(dr->metDeadlinesPer1);
                        break;
                    case 6: // % Met deadlines RT2
                        ponto.setY(dr->metDeadlinesPer2);
                        break;
                    case 7: // % Met deadlines RT3
                        ponto.setY(dr->metDeadlinesPer3);
                        break;
                }
                points.append( ponto );

            }
            delete dr;

        }

        // Define point style - 5 options
        // Define line style - 5 options
        QwtSymbol::Style pointStyle;
        Qt::PenStyle lineStyle;
        switch( i%5 ) {
            case 0:
                pointStyle = QwtSymbol::Ellipse;
                lineStyle = Qt::SolidLine;
                break;
            case 1:
                pointStyle = QwtSymbol::Rect;
                lineStyle = Qt::DashLine;
                break;
            case 2:
                pointStyle = QwtSymbol::XCross;
                lineStyle = Qt::DotLine;
                break;
            case 3:
                pointStyle = QwtSymbol::Triangle;
                lineStyle = Qt::DashDotLine;
                break;
            case 4:
                pointStyle = QwtSymbol::Cross;
                lineStyle = Qt::DashDotDotLine;
                break;
            default:
                pointStyle = QwtSymbol::Diamond;
                lineStyle = Qt::SolidLine;

        }

        // Define curve
        QColor cor = QColor( aop->getColor(i) );
        QwtPlotCurve *curve = new QwtPlotCurve;
        curve->setTitle( legenda );
        curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        curve->setSamples( points );

        // Define point
        float pointSize = aop->getPointSize() * 6;
        QwtSymbol *symbol = new QwtSymbol( pointStyle,
            QBrush( Qt::transparent ), QPen( QBrush(cor), 1 ),
            QSize( pointSize,pointSize ) );
        curve->setSymbol( symbol );
        curve->setLegendAttribute(QwtPlotCurve::LegendShowLine);
        curve->setLegendAttribute(QwtPlotCurve::LegendShowSymbol);
        curve->setLegendIconSize( QSize(20,20) );
        curve->setPen( cor , aop->getLineWidth() , lineStyle );
        if( aop->isLatencyDistribution() ) {
           curve->setStyle( QwtPlotCurve::Sticks );
        }

        curve->attach( plotter );
        // end

        data->clear();
        delete data;
    }

    QList<QwtLegendLabel*> objList = plotter->findChildren<QwtLegendLabel*>();
    for( int i = 0; i < objList.size(); i++ ) {
        QwtLegendLabel* legLabel = objList.at(i);
        legLabel->setChecked(true);
    }

    new QwtPlotZoomer( plotter->canvas() );

    window->show();

}

QwtPlotter::~QwtPlotter() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class View/QwtPlotter" << std::endl;
#endif

    window->deleteLater();
    this->plotter->deleteLater();

}

void QwtPlotter::legendChecked(const QVariant &data, bool status, int ) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/QwtPlotter::legendChecked" << std::endl;
#endif

    QwtPlotItem* it =  plotter->infoToItem(data);
    if( it ) {
        it->setVisible( status );
        plotter->replot();
    }
}

void QwtPlotter::exportPlot() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/QwtPlotter::exportPlot" << std::endl;
#endif

    QwtPlotRenderer renderer;
    QMessageBox::information(window,trUtf8("Attention"),trUtf8("When you select the file type (PDF, SVG, PostScript, Image)\nAdd the file type (.pdf, .svg, .ps, .jpg) in the name file"));
    renderer.exportTo(this->plotter,"PerformanceEvaluate.pdf");

}
