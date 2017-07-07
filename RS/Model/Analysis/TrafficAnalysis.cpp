/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* TrafficAnalysis.cpp
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
* 01/12/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*/

#include "TrafficAnalysis.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <clocale>

#include "PacketInfo.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

TrafficAnalysis::TrafficAnalysis(unsigned short numElements,
                                 unsigned short dataWidth,
                                 float lower, float upper,
                                 float fClk,unsigned short fifoOutDepth,
                                 unsigned short flowControlType,
                                 const char *workDir, const char *resultDir)
{
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/Analysis/TrafficAnalysis" << std::endl;
#endif
    std::setlocale(LC_ALL, "en_US.UTF-8");

    this->packets.resize(numElements);

    this->numberOfElements= numElements;
    this->dataWidth       = dataWidth;
    this->fClk            = fClk;
    this->tClk            = (1.0f / fClk) * 1000.0f;
    this->channelBw       = dataWidth * fClk;
    this->fifoOutDepth    = fifoOutDepth;
    this->flowControlType = flowControlType;
    this->lower           = lower;
    this->resultDir       = resultDir;
    this->upper           = upper;
    this->workDir         = workDir;

}

TrafficAnalysis::~TrafficAnalysis() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Model/Analysis/TrafficAnalysis" << std::endl;
#endif

    for( unsigned int elementId = 0; elementId < numberOfElements; elementId++ ) {
        std::vector<PacketInfo* > pcks = this->packets[elementId];
        for( unsigned int pckIndex = 0; pckIndex < pcks.size(); pckIndex++ ) {
            PacketInfo* pck = pcks[pckIndex];
            delete pck;
        }
    }

}

TrafficAnalysis::StatusAnalysis TrafficAnalysis::readLogsFiles() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/TrafficAnalysis::readLogsFiles" << std::endl;
#endif

    FILE* fpIn;
    char filenameIn[512];
    char str[2048];

    unsigned long long receivedPacketId = 0;
    unsigned int       source = 0;
    unsigned int       destination = 0;
    unsigned int       hops;
    unsigned int       flowId = 0;
    unsigned int       trafficClass = 0;
    unsigned long      deadline = 0;
    unsigned long long cycleCreationPacket = 0;
    unsigned long long cycleReceivedHeader = 0;
    unsigned long long cycleReceivedTrailer = 0;
    unsigned int       packetSize = 0;
    float              requiredBandwidth = 0;

    for( unsigned int elementId = 0; elementId < numberOfElements; elementId++ ) {
        // Open the simulation report file
        sprintf(filenameIn,"%s/ext_%u_out",workDir,elementId);
        if( (fpIn = fopen(filenameIn,"rt")) == NULL ) {
            return TrafficAnalysis::NoInputFile;
        }

        int scanResult = 0;
        // It looks for the marker of begining of table of measures
        do {
            scanResult = fscanf(fpIn,"%s",str);
        } while( strcmp(str,"#") != 0 );


        // It extracts information related to each packet received
        do {
            scanResult = fscanf(fpIn,"%s", str);
            if ((strcmp(str,"#") != 0)) {
                receivedPacketId = (unsigned long int) atoll(str);

                scanResult = fscanf(fpIn,"%s", str);
                source = (unsigned int) atoi(str);

                scanResult = fscanf(fpIn,"%s", str);
                destination = (unsigned int) atoi(str);

                scanResult = fscanf(fpIn,"%s", str);
                hops = (unsigned int) atoi(str);

                scanResult = fscanf(fpIn,"%s", str);
                flowId = (unsigned int) atoi(str);

                scanResult = fscanf(fpIn,"%s", str);
                trafficClass = (unsigned int) atoi(str);

                scanResult = fscanf(fpIn,"%s", str);
                deadline = (unsigned long long int) atol(str);

                scanResult = fscanf(fpIn,"%s", str);
                cycleCreationPacket = (unsigned long long int) atoll(str);

                scanResult = fscanf(fpIn,"%s", str);
                cycleReceivedHeader = (unsigned long long int) atoll(str);

                scanResult = fscanf(fpIn,"%s", str);
                cycleReceivedTrailer = (unsigned long long int) atoll(str);

                scanResult = fscanf(fpIn,"%s", str);
                packetSize = (unsigned int) atoi(str);

                scanResult = fscanf(fpIn,"%s", str);
                requiredBandwidth = (float) atof(str);

                PacketInfo* pckInfo = new PacketInfo(receivedPacketId,source,destination,hops,flowId,
                                                     trafficClass,deadline,cycleCreationPacket,cycleReceivedHeader,
                                                     cycleReceivedTrailer,packetSize,requiredBandwidth);

                this->packets[elementId].push_back(pckInfo);
                scanResult++;
            }
        } while ((strcmp(str,"#") != 0));

        fclose(fpIn);

    }

    return TrafficAnalysis::Ok;

}
