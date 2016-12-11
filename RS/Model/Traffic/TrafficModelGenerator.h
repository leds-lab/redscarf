/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* TrafficModelGenerator.h
* Copyright (C) 2014 - 2017 LEDS - Univali <zeferino@univali.br>
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
* 10/12/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*
*/

#ifndef __TRAFFICGENERATOR_H__
#define __TRAFFICGENERATOR_H__

#include <string>

class SystemParameters;
class SystemOperation;
class Experiment;
class TrafficParameters;

class TrafficModelGenerator {
protected:

    SystemParameters* sp;
    Experiment* exp;
    SystemOperation* sop;

    unsigned int numberCyclesPerFlit;
    unsigned int lastPayloadLength;

    float requiredBwArray[100];
    float requiredBw;

    unsigned long int pck2Send;
    unsigned long int pck2SendArray[100];

    unsigned int numTrafficPatter;

    void writeFlow(TrafficParameters* tp, unsigned int type = 0, unsigned int burstSize=0);

    void createFileTrafficConfiguration(const char* diretorio) throw (const char*);

    // gtr_functions
    virtual int functionProbability(TrafficParameters* tp);
    virtual void removeZeroPayloadPacket(TrafficParameters* tp);
    virtual unsigned int calculateIdleBasedOnPayloadLength(unsigned int payloadLength,
                                                   float channelBw, float requiredBw);
    virtual unsigned int calculatePayloadLengthBasedOnIdle(unsigned int idle,
                                                           float channelBw, float requiredBw);
    virtual unsigned int calculatePayloadLengthBasedOnInterArrival(unsigned int iat,
                                                                    float channelBw, float requiredBw);
    virtual unsigned int calculateInterArrivalBasedOnPayloadLength(unsigned int payloadLength,
                                                                   float channelBw, float requiredBw);

    // gtr2
    virtual void validateTrafficAddresses();
    virtual void adjustParameters();
    virtual void determineFlowControl();
    virtual void generateVariableRate() throw (const char*);
    virtual void generateTrafficModel();

    virtual void generateSingleDestinationDistributions(TrafficParameters* tp);
    virtual void generateMultipleDestinationDistributions(TrafficParameters* tp);
public:
    TrafficModelGenerator(SystemParameters* sp, Experiment* exp);

    virtual void generateTraffic(const char* diretorio) throw (const char* );

    virtual ~TrafficModelGenerator();

};

#endif // TRAFFICGENERATOR_H
