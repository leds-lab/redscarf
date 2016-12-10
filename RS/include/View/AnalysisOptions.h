/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* AnalysisOptions.h
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
* 20/11/2016 - 2.0     - Eduardo Alves da Silva      | Back-end refactoring
*
*/

#ifndef __ANALYSISOPTIONS_H__
#define __ANALYSISOPTIONS_H__

#include <QString>
#include <QColor>

class AnalysisOptions {

public:

    enum FlowOptions { AllFlows=0, RT0, RT1, nRT0, nRT1,Specified };

    AnalysisOptions();
    AnalysisOptions(bool topology,bool routingAlgorithm,bool flowControl,
            bool arbiterType,bool vcOp,bool inputBuffers,bool outputBuffers,float lineWidth,
            float pointSize,unsigned short source,unsigned short destination,
            QString xAxisLabel,QString yAxisLabel,int xAxis,int yAxis,QString title,
            FlowOptions flowOp,QColor* color[5],bool latencyDistribution);

    inline void setTopology(bool check) { this->topology = check; }
    inline void setRoutingAlgorithm(bool check) { this->routingAlgorithm = check; }
    inline void setFlowControl(bool check) { this->flowControl = check; }
    inline void setArbiterType(bool check) { this->arbiterType = check; }
    inline void setVcOp(bool check) { this->vcOp = check; }
    inline void setInputBuffers(bool check) { this->inputBuffers = check; }
    inline void setOutputBuffers(bool check) { this->outputBuffers = check; }
    inline void setLatencyDistribution(bool check) { this->latencyDistribution = check; }
    inline void setLineWidth(float width) { this->lineWidth = width; }
    inline void setPointSize(float size) { this->pointSize = size; }
    inline void setSource(int source) { this->source = source; }
    inline void setDestination(int destination) { this->destination = destination; }
    inline void setXAxisLabel(QString xLabel) { this->xAxisLabel = xLabel; }
    inline void setYAxisLabel(QString yLabel) { this->yAxisLabel = yLabel; }
    inline void setFlowOp(FlowOptions op) { this->flowOp = op; }
    inline void setTitle(QString title) { this->title = title; }
    void setColor(int index, QColor* color);
    inline void setXAxis(int x) {this->xAxis = x;}
    inline void setYAxis(int y) {this->yAxis = y;}

    inline bool isTopology() const { return this->topology; }
    inline bool isRoutingAlgorithm() const { return this->routingAlgorithm; }
    inline bool isFlowControl() const { return this->flowControl; }
    inline bool isArbiterType() const { return this->arbiterType; }
    inline bool isVcOp() const { return this->vcOp; }
    inline bool isInputBuffers() const { return this->inputBuffers; }
    inline bool isOutputBuffers() const { return this->outputBuffers; }
    inline bool isLatencyDistribution() const { return this->latencyDistribution; }
    inline float getLineWidth() const { return this->lineWidth; }
    inline float getPointSize() const { return this->pointSize; }
    inline int getSource() const { return this->source; }
    inline int getDestination() const { return this->destination; }
    inline QString getXAxisLabel() const { return this->xAxisLabel; }
    inline QString getYAxisLabel() const { return this->yAxisLabel; }
    inline FlowOptions getFlowOp() const { return this->flowOp; }
    inline QString getTitle() const { return this->title; }
    QColor getColor(int index);
    inline int getXAxis() {return this->xAxis;}
    inline int getYAxis() {return this->yAxis;}

    QString getLegend(QString dir) const;
    static QStringList getVisibleStrings(const QStringList dirs);

private:
    bool topology;
    bool routingAlgorithm;
    bool flowControl;
    bool arbiterType;
    bool vcOp;
    bool inputBuffers;
    bool outputBuffers;
    bool latencyDistribution;

    float lineWidth;
    float pointSize;

    unsigned short source;
    unsigned short destination;

    int xAxis;
    int yAxis;
    QString xAxisLabel;
    QString yAxisLabel;
    QString title;
    FlowOptions flowOp;
    QColor* color[5];

};

#endif
