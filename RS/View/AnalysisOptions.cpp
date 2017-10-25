/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* AnalysisOptions.cpp
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
* ----------------------------------------------------------------------------
* 20/11/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*
*/

#include "AnalysisOptions.h"

#include <cstdlib>
#include <ctime>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

AnalysisOptions::AnalysisOptions() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor class View/GraphicOptions" << std::endl;
#endif
    routingAlgorithm = false;
    flowControl = false;
    arbiterType = false;
    vcOp = false;
    useCryptography = false;
    inputBuffers = false;
    outputBuffers = false;

    latencyDistribution = false;
    lineWidth = 0.0f;
    pointSize = 0.0f;

    source = 0;
    destination = 0;

    color[0] = NULL;
    color[1] = NULL;
    color[2] = NULL;
    color[3] = NULL;
    color[4] = NULL;


    srand( time(NULL) );
}

AnalysisOptions::AnalysisOptions(bool topology, bool routingAlgorithm,
        bool flowControl, bool arbiterType, bool vcOp,bool useCryptography, bool inputBuffers, bool outputBuffers,
        float lineWidth, float pointSize, unsigned short source, unsigned short destination,
                                 QString xAxisLabel, QString yAxisLabel, int xAxis, int yAxis,
        QString title, FlowOptions flowOp, QColor *color[5], bool latencyDistribution) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor class View/AnalysisOptions" << std::endl;
#endif

    this->topology = topology;
    this->routingAlgorithm = routingAlgorithm;
    this->flowControl = flowControl;
    this->arbiterType = arbiterType;
    this->vcOp = vcOp;
    this->useCryptography = useCryptography;
    this->inputBuffers = inputBuffers;
    this->outputBuffers = outputBuffers;

    this->latencyDistribution = latencyDistribution;
    this->lineWidth = lineWidth;
    this->pointSize = pointSize;

    this->source = source;
    this->destination = destination;

    this->xAxis = xAxis;
    this->yAxis = yAxis;
    this->xAxisLabel = xAxisLabel;
    this->yAxisLabel = yAxisLabel;
    this->title = title;
    this->flowOp = flowOp;

    this->color[0] = color[0];
    this->color[1] = color[1];
    this->color[2] = color[2];
    this->color[3] = color[3];
    this->color[4] = color[4];
}

void AnalysisOptions::setColor(int index, QColor *color) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/AnalysisOptions::setColor" << std::endl;
#endif
    if( index < 0 || index > 4 ) {
        return;
    }

    this->color[index] = color;

}

QColor AnalysisOptions::getColor(int index) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/AnalysisOptions::getColor" << std::endl;
#endif

    if(index < 0 || index > 4) {
        return QColor( rand() % 256,rand() % 256,rand() % 256 );
    }

    return *color[index];

}

QString AnalysisOptions::getLegend(QString dir) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/AnalysisOptions::getLegend" << std::endl;
#endif

    QString legend = QString();
    int lio = dir.lastIndexOf("/");
    QString diretorioExperimento = dir.mid( lio+1 );
    QStringList config = diretorioExperimento.split("_");

    legend += ( this->topology           ? config.at(1)     : "" );
    legend += ( this->routingAlgorithm   ? " "+config.at(2) : "" );
    legend += ( this->flowControl        ? " "+config.at(3) : "" );
    legend += ( this->arbiterType        ? " "+config.at(4) : "" );
    legend += ( this->vcOp               ? " "+config.at(5) : "" );
    legend += ( this->inputBuffers       ? " "+config.at(6) : "" );
    legend += ( this->outputBuffers      ? " "+config.at(7) : "" );
    legend += ( this->useCryptography      ? " "+config.at(8) : "" );

    if (legend.startsWith(" ")) {
        legend.remove(0,1);
    }
    return legend;

}

QStringList AnalysisOptions::getVisibleStrings(const QStringList dirs) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/AnalysisOptions::getVisibleStrings" << std::endl;
#endif

    QStringList items;
    QStringList experiments;
    QStringList fsClk;
    for( int i = 0; i < dirs.size(); i++ ) {
        QString dir = dirs.at(i);
        QString tmpDir = dir.left( dir.lastIndexOf("/") );
        QString item = dir.mid(tmpDir.lastIndexOf("/") + 5);
        QStringList sep = item.split("/");
        experiments.append(sep[0]);
        fsClk.append(sep[1]);
    }

    QStringList temp = experiments;
    temp.removeDuplicates();

    if(temp.size() == 1) {
        for(int i = 0; i < dirs.size(); i++) {
            items.append( QString("@ %1").arg(fsClk.at(i)) );
        }
    } else {
        int numExps =  experiments.size() / temp.size();
        for(int i = 0; i < dirs.size(); i++) {
            if( i % numExps == 0 ) {
                items.append( QString("%1 @ %2").arg(experiments.at(i)).arg(fsClk.at(i)) );
            } else {
                items.append( QString(" @ %1").arg(fsClk.at(i)).rightJustified(64,' ') );
            }
        }
    }

    return items;
}
