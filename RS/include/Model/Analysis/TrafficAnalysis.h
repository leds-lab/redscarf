/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* TrafficAnalysis.h
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

#ifndef TRAFFICANALYSIS_H
#define TRAFFICANALYSIS_H

class TrafficAnalysis {
protected:
    unsigned int xSize;
    unsigned int ySize;
    unsigned int zSize;
    unsigned int dataWidth;
    float lower;
    float upper;
    float fClk;
    float tClk;
    unsigned long int channelBw;
    unsigned int fifoOutDepth;
    unsigned int flowControlType;
    const char* workDir;
    const char* resultDir;


public:
    // Versão antiga => 0 = NoInputFile | 1 = Ok | -1 = NoPackets Delivered
    enum StatusAnalysis{Ok=0,NoPacketsDelivered,NoInputFile,NoOutputFile};
    TrafficAnalysis(unsigned int xSize, unsigned int ySize,unsigned int zSize,
                    unsigned int dataWidth,float lower,float upper,
                    float fClk,float tClk,unsigned long int channelBw,
                    unsigned int fifoOutDepth,unsigned int flowControlType,
                    const char* workDir, const char* resultDir);

    virtual StatusAnalysis makeAnalysis() = 0;

    virtual ~TrafficAnalysis();
};

#endif // ANALYSISTRAFFIC_H
