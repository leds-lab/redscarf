/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* GnuPlotPlotter.cpp
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

#include "include/Control/GnuPlotPlotter.h"
#include "include/View/AnalysisOptions.h"

#include <QDir>

#define GNUPLOT_SCRIPT "work/graphic.txt"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

unsigned int GnuPlotPlotter::XCOL[4] = {1, 3,  4, 11};
unsigned int GnuPlotPlotter::YCOL[8] = {6, 13, 4, 11, 16, 17, 18, 19};


GnuPlotPlotter::GnuPlotPlotter(QObject *parent) : Plotter(parent) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor class Control/GnuPlotPlotter" << std::endl;
#endif

}

void GnuPlotPlotter::viewGraphic(AnalysisOptions *aop, QStringList dirs) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/GnuPlotPlotter::viewGraphic" << std::endl;
#endif
    FILE* fp = fopen(GNUPLOT_SCRIPT,"w");

    if( fp == NULL ) {
        emit sendMessage(trUtf8("<font color=red>Unable to open file to write graphic informations</font>"));
        return;
    }

    QProcess* gnuplot = new QProcess(this);
    connect(gnuplot,SIGNAL(finished(int)),this,SIGNAL(finished(int)));
    connect(gnuplot,SIGNAL(finished(int)),gnuplot,SLOT(kill()));
    connect(gnuplot,SIGNAL(finished(int)),gnuplot,SLOT(deleteLater()));

    // Build the script file

    // Title and axis names
    QByteArray btTitle = aop->getTitle().toUtf8();
    QByteArray btXLabel = aop->getXAxisLabel().toUtf8();
    QByteArray btYLabel = aop->getYAxisLabel().toUtf8();
    const char* title  = btTitle.constData();
    const char* yLabel = btYLabel.constData();
    const char* xLabel = btXLabel.constData();
    fprintf( fp,"set title  '%s' %s", title, " font 'Helvetica, 15' \n" );
    fprintf( fp,"set ylabel '%s' %s", yLabel," font 'Helvetica, 12' \n" );
    fprintf( fp,"set xlabel '%s' %s", xLabel," font 'Helvetica, 12' \n" );

    // Margins
    fprintf( fp,"set tmargin 	3  \n");
    fprintf( fp,"set rmargin 	3  \n");
    fprintf( fp,"set lmargin 	15 \n");
    fprintf( fp,"set bmargin 	3  \n");

    // X axis
    fprintf( fp,"set xrange [0:] \n" );
    // Y axis
    fprintf( fp,"set yrange [0:] \n" );

    // Grid
    fprintf( fp,"set grid  nopolar \n" );

    // Legend
    fprintf( fp,"set key top left \n" );

    // It plots the curves
    int size = dirs.size();
    QStringList legendas;
    for( int i = 0; i < size; i++) {
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
            legenda = QString("Curve#%1").arg(i+1);
        }

        if( legendas.contains( legenda ) ) {
            legenda += QString("#%1").arg(i+1);
        }

        if( aop->isLatencyDistribution() ) {
            int lio = dir.lastIndexOf("/") + 1;
            legenda += QString(" FClk %1").arg( dir.mid(lio) );
        }

        legendas.append(legenda);

        QString arquivo;
        switch( aop->getFlowOp() ) {
            case AnalysisOptions::AllFlows:
                arquivo = dir + "/Results/summary";
                break;
            case AnalysisOptions::RT0:
            case AnalysisOptions::RT1:
            case AnalysisOptions::nRT0:
            case AnalysisOptions::nRT1:
                arquivo = dir + QString("/Results/class_%1").arg(aop->getFlowOp() - 1);
                break;
            case AnalysisOptions::Specified:
                arquivo = dir + QString("/Results/flow_%1_%2_%3_%4_%5")
                        .arg(aop->getXSrc())
                        .arg(aop->getYSrc())
                        .arg(aop->getXDest())
                        .arg(aop->getYDest())
                        .arg( aop->getTrafficPattern() );
                break;
        }
        char plot[20];
        if( aop->isLatencyDistribution() ) {
            arquivo += "_latency_histogram";
            sprintf(plot,"using 1:3");
        } else {
            sprintf(plot,"using %u:%u",XCOL[aop->getXAxis()],YCOL[aop->getYAxis()]);
        }

        QColor cor = aop->getColor(i);

        QByteArray btFile = arquivo.toUtf8();
        QByteArray btColor = cor.name().toUtf8();
        QByteArray btCurve = legenda.toUtf8();

        const char* filename = btFile.constData();

        FILE* fpRead;
        if( (fpRead = fopen(filename,"r")) == NULL ) {
            delete gnuplot;
            emit sendMessage("<font color=red>Unable to open file to read graphic informations</font>");
            return;
        } else {
            fclose(fpRead);
        }

        const char* color = btColor.constData();
        const char* curveName = btCurve.constData();
        if( i == 0 ) {
            // First curve plot
            fprintf(fp,"plot '%s' %s with linespoints lt rgb \"%s\" lw %f pointtype %i ps %f title '%s' \n",
                    filename,plot,color,aop->getLineWidth(),(rand()%28),aop->getPointSize(),curveName);
        } else {
            // Other curves
            fprintf(fp,"replot '%s' %s with linespoints lt rgb \"%s\" lw %f pointtype %i ps %f title '%s' \n",
                    filename,plot,color,aop->getLineWidth(),(rand()%28),aop->getPointSize(),curveName);
        }

    }
    fclose(fp);

    QString command = QString("gnuplot -geometry 380x210+630+0 -background white -persist %1").arg(GNUPLOT_SCRIPT);
    gnuplot->start(command);

}
