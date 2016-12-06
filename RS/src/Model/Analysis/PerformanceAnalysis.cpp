/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* PerformanceAnalysis.cpp
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

#include "include/Model/System/Defines.h"
#include "include/Model/Analysis/PerformanceAnalysis.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <clocale>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

PerformanceAnalysis::PerformanceAnalysis(unsigned int xSize, unsigned int ySize, unsigned int zSize,
         unsigned int dataWidth, float lower, float upper, float fClk, float tClk,
         unsigned long channelBw, unsigned int fifoOutDepth, unsigned int flowControlType,
         const char *workDir, const char *resultDir) :
        TrafficAnalysis(xSize,ySize,zSize,dataWidth,lower,upper,fClk,tClk,channelBw,
        fifoOutDepth,flowControlType,workDir,resultDir)
{
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/Analysis/PerformanceAnalysis" << std::endl;
#endif

    // TODO: Alterar todo o analisador para suporter análise 3D
    std::setlocale(LC_ALL, "en_US.UTF-8");

    this->packets = new std::vector<PacketInfo* >**[xSize];

    flowAccNbOfPck                = new unsigned long int**[xSize];
    flowAccNbOfFlits              = new unsigned long int**[xSize];
    flowAccRequiredBw             = new unsigned long int**[xSize];
    flowAvgRequiredBw             = new float**[xSize];
    flowAvgRequiredBwNorm         = new float**[xSize];
    flowAcceptedTrafficFlits      = new float**[xSize];
    flowAcceptedTrafficBps        = new float**[xSize];
    flowAccPckWithDeadline        = new unsigned long int***[xSize];
    flowAccMissedDeadlines        = new unsigned long int***[xSize];
    flowMissedDeadlinesPer        = new float***[xSize];
    flowMetDeadlinesPer           = new float***[xSize];
    flowAccLatency                = new unsigned long int**[xSize];
    flowAvgLatency                = new float**[xSize];
    flowAccLatencyMinusAvgLatency = new float**[xSize];
    flowStdevLatency              = new float**[xSize];
    flowMinLatency                = new unsigned long int**[xSize];
    flowMaxLatency                = new unsigned long int**[xSize];
    flowLatencyRng                = new unsigned long int***[xSize];
    flowLatencyRngInc             = new unsigned long int**[xSize];
    flowLatencyRngPcks            = new unsigned long int***[xSize];

    // Second dimension
    for(unsigned int i = 0; i < xSize; i++) {
        this->packets[i] = new std::vector<PacketInfo *>*[ySize];

        flowAccNbOfPck[i]                = new unsigned long int*[ySize];
        flowAccNbOfFlits[i]              = new unsigned long int*[ySize];
        flowAccRequiredBw[i]             = new unsigned long int*[ySize];
        flowAvgRequiredBw[i]             = new float*[ySize];
        flowAvgRequiredBwNorm[i]         = new float*[ySize];
        flowAcceptedTrafficFlits[i]      = new float*[ySize];
        flowAcceptedTrafficBps[i]        = new float*[ySize];
        flowAccPckWithDeadline[i]        = new unsigned long int**[ySize];
        flowAccMissedDeadlines[i]        = new unsigned long int**[ySize];
        flowMissedDeadlinesPer[i]        = new float**[ySize];
        flowMetDeadlinesPer[i]           = new float**[ySize];
        flowAccLatency[i]                = new unsigned long int*[ySize];
        flowAvgLatency[i]                = new float*[ySize];
        flowAccLatencyMinusAvgLatency[i] = new float*[ySize];
        flowStdevLatency[i]              = new float*[ySize];
        flowMinLatency[i]                = new unsigned long int*[ySize];
        flowMaxLatency[i]                = new unsigned long int*[ySize];
        flowLatencyRng[i]                = new unsigned long int**[ySize];
        flowLatencyRngInc[i]             = new unsigned long int*[ySize];
        flowLatencyRngPcks[i]            = new unsigned long int**[ySize];
    }

    // Third dimension
    for(unsigned int i = 0; i < xSize; i++) {
        for(unsigned int x = 0; x < ySize; x++) {
            this->packets[i][x] = new std::vector<PacketInfo*>();

            flowAccNbOfPck[i][x]                = new unsigned long int[MAX_FLOW_IDS];
            flowAccNbOfFlits[i][x]              = new unsigned long int[MAX_FLOW_IDS];
            flowAccRequiredBw[i][x]             = new unsigned long int[MAX_FLOW_IDS];
            flowAvgRequiredBw[i][x]             = new float[MAX_FLOW_IDS];
            flowAvgRequiredBwNorm[i][x]         = new float[MAX_FLOW_IDS];
            flowAcceptedTrafficFlits[i][x]      = new float[MAX_FLOW_IDS];
            flowAcceptedTrafficBps[i][x]        = new float[MAX_FLOW_IDS];
            flowAccPckWithDeadline[i][x]        = new unsigned long int*[MAX_FLOW_IDS];
            flowAccMissedDeadlines[i][x]        = new unsigned long int*[MAX_FLOW_IDS];
            flowMissedDeadlinesPer[i][x]        = new float*[MAX_FLOW_IDS];
            flowMetDeadlinesPer[i][x]           = new float*[MAX_FLOW_IDS];
            flowAccLatency[i][x]                = new unsigned long int[MAX_FLOW_IDS];
            flowAvgLatency[i][x]                = new float[MAX_FLOW_IDS];
            flowAccLatencyMinusAvgLatency[i][x] = new float[MAX_FLOW_IDS];
            flowStdevLatency[i][x]              = new float[MAX_FLOW_IDS];
            flowMinLatency[i][x]                = new unsigned long int[MAX_FLOW_IDS];
            flowMaxLatency[i][x]                = new unsigned long int[MAX_FLOW_IDS];
            flowLatencyRng[i][x]                = new unsigned long int*[MAX_FLOW_IDS];
            flowLatencyRngInc[i][x]             = new unsigned long int[MAX_FLOW_IDS];
            flowLatencyRngPcks[i][x]            = new unsigned long int*[MAX_FLOW_IDS];
        }
    }

    // Fourth dimension
    for(unsigned int i = 0; i < xSize; i++) {
        for(unsigned int x = 0; x < ySize; x++) {
            for(unsigned int z = 0; z < MAX_FLOW_IDS; z++) {
                flowAccPckWithDeadline[i][x][z] = new unsigned long int[MAX_CLASSES];
                flowAccMissedDeadlines[i][x][z] = new unsigned long int[MAX_CLASSES];
                flowMissedDeadlinesPer[i][x][z] = new float[MAX_CLASSES];
                flowMetDeadlinesPer[i][x][z]    = new float[MAX_CLASSES];
                flowLatencyRng[i][x][z]         = new unsigned long int[100];
                flowLatencyRngPcks[i][x][z]     = new unsigned long int[100];
            }
        }
    }



}

TrafficAnalysis::StatusAnalysis PerformanceAnalysis::makeAnalysis() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/PerformanceAnalysis::makeAnalysis" << std::endl;
#endif

    // Read simulation logs
    StatusAnalysis status = this->readLogsFiles();
    if( status != TrafficAnalysis::Ok ) {
        return status;
    }

    // Starting System Variables
    this->initSystemVariables();

    // Analyze all flows
    status = this->analyzeAllFlows();
    if( status != TrafficAnalysis::Ok ) {
        return status;
    }

    // Creates the reports for all the flows
    status = this->createReportsForAllFlows();
    if( status != TrafficAnalysis::Ok ) {
        return status;
    }

    // Analyze individual flows
    return this->analyzeIndividualFlows();
}

TrafficAnalysis::StatusAnalysis PerformanceAnalysis::readLogsFiles() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Mode/Analysis/PerformanceAnalysis::readLogsFiles" << std::endl;
#endif

    FILE* fpIn;
    char filenameIn[512];

    unsigned long int receivedPacketId = 0;
    unsigned int      xSource = 0;
    unsigned int      ySource = 0;
    unsigned int      xDestination = 0;
    unsigned int      yDestination = 0;
    unsigned int      idSrc = 0;
    unsigned int      idDest= 0;
    unsigned int      threadId = 0;
    unsigned int      trafficClass = 0;
    unsigned long     deadline = 0;
    unsigned long long cycleCreationPacket = 0;
    unsigned long long cycleReceivedHeader = 0;
    unsigned long long cycleReceivedTrailer = 0;
    unsigned int      packetSize = 0;
    float             requiredBandwidth = 0;

    for( unsigned int x = 0; x < xSize; x++ ) {
        for( unsigned int y = 0; y < ySize; y++ ) {
            unsigned int nodeId = COORDINATE_2D_TO_ID(x,y,xSize);
            xDestination = x;
            yDestination = y;
            // It opens the input file
            sprintf(filenameIn,"%s/ext_%u_out",workDir,nodeId);
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
                    receivedPacketId = (unsigned long int) atoi(str);

                    scanResult = fscanf(fpIn,"%s", str);
                    idSrc = (unsigned int) atoi(str);
                    xSource = ID_TO_COORDINATE_2D_X(idSrc,xSize);
                    ySource = ID_TO_COORDINATE_2D_Y(idSrc,xSize);

                    scanResult = fscanf(fpIn,"%s", str);
                    idDest = (unsigned int) atoi(str);
                    //xDestination = ID_TO_COORDINATE_X(idDest,xSize);
                    //yDestination = ID_TO_COORDINATE_Y(idDest,ySize);

                    scanResult = fscanf(fpIn,"%s", str);
                    threadId = (unsigned int) atoi(str);

                    scanResult = fscanf(fpIn,"%s", str);
                    trafficClass = (unsigned int) atoi(str);

                    scanResult = fscanf(fpIn,"%s", str);
                    deadline = (unsigned long long int) atol(str);

                    scanResult = fscanf(fpIn,"%s", str);
                    cycleCreationPacket = (unsigned long long int) atol(str);

                    scanResult = fscanf(fpIn,"%s", str);
                    cycleReceivedHeader = (unsigned long long int) atol(str);

                    scanResult = fscanf(fpIn,"%s", str);
                    cycleReceivedTrailer = (unsigned long long int) atol(str);

                    scanResult = fscanf(fpIn,"%s", str);
                    packetSize = (unsigned int) atoi(str);

                    scanResult = fscanf(fpIn,"%s", str);
                    requiredBandwidth = (float) atof(str);

                    PacketInfo* pckInfo = new PacketInfo(receivedPacketId,xDestination,yDestination,xSource,ySource,threadId,
                            trafficClass,deadline,cycleCreationPacket,cycleReceivedHeader,
                            cycleReceivedTrailer,packetSize,requiredBandwidth);
                    this->packets[x][y]->push_back(pckInfo);
                    scanResult++;
                }
            } while ((strcmp(str,"#") != 0));

            fclose(fpIn);

        }
    }

    return TrafficAnalysis::Ok;

}

void PerformanceAnalysis::initSystemVariables() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/PerformanceAnalysis::initSystemVariables" << std::endl;
#endif

    // It determines how many cycles are spent by a header to travers a router with
    // none contention
    if (fifoOutDepth) {
        nbOfCyclesPerRouter = 3; // With    output fifos
    } else {
        nbOfCyclesPerRouter = 2; // Without output fifos
    }
    // Calculate time in router depends flow control type - On Handshake have overlay cycles in routing phase
    tRA = ( flowControlType == 0 ? nbOfCyclesPerRouter-1 : nbOfCyclesPerRouter );


    // It determines the number of cycles spent to send a flit
    if (flowControlType) {
        nbOfCyclesPerFlit = 1;  // Credit-based flow control
    } else {
        nbOfCyclesPerFlit = 4;  // Handshake flow control
    }

    // It the determines the available bandwidth
    channelBwAvailable = (float) channelBw/nbOfCyclesPerFlit;

    // It defines the initial values
    accRequiredBw             = 0.0;
    accLatency                = 0;
    idealLatency              = 0;
    idealAccLatency           = 0;

    accNbOfPck   		      = 0;
    accNbOfFlits 		      = 0;
    minLatency     		      = ULONG_MAX;//1000000;
    maxLatency                = 0;
    smallestStartCycle 		  = ULONG_MAX;//1000000;
    smallestEndCycle   		  = ULONG_MAX;
    biggestEndCycle   		  = 0;
    accLatencyMinusAvgLatency = 0;



    for (unsigned int trafficClass = 0; trafficClass < MAX_CLASSES; trafficClass++){
        accMissedDeadlines[trafficClass]			 = 0;
        accPckWithDeadline[trafficClass]             = 0;

        classAccNbOfPck[trafficClass] 				 = 0;
        classAccNbOfFlits[trafficClass] 			 = 0;
        classAccRequiredBw[trafficClass] 			 = 0;

        classAccPckWithDeadline[trafficClass]		 = 0;
        classAccMissedDeadlines[trafficClass]		 = 0;
        classMetDeadlinesPer[trafficClass]	 		 = 0;

        classAccLatency[trafficClass] 				 = 0;
        classAccLatencyMinusAvgLatency[trafficClass] = 0;
        classMinLatency[trafficClass] 				 = ULONG_MAX;//10000000;
        classMaxLatency[trafficClass] 				 = 0;
    }
}

unsigned long int PerformanceAnalysis::calculateIdealLatency(unsigned int nLinks, unsigned int nRouters, unsigned int payload) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/PerformanceAnalysis::calculateIdealLatency" << std::endl;
#endif

    return ( nbOfCyclesPerFlit * nLinks + tRA * nRouters + nbOfCyclesPerFlit * payload );

}

TrafficAnalysis::StatusAnalysis PerformanceAnalysis::analyzeAllFlows() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/PerformanceAnalysis::analyzeAllFlows" << std::endl;
#endif

    unsigned int x = 0; // Index of x dimension
    unsigned int y = 0; // Index of y dimension
    unsigned int i = 0; // Index of packets in log files

    //////////////////////////////////////////////////////////////////////////////
    /// 1st SCAN: It determines the number of total packets received by TGs
    //////////////////////////////////////////////////////////////////////////////
    unsigned int totalNbOfPckRcvd = 0;
    for( x = 0; x < xSize; x++ ) {
        for( y = 0; y < ySize; y++) {
            totalNbOfPckRcvd += (unsigned int) this->packets[x][y]->size();
        }
    }

    // If none packet was delivered, it returns with an error
    if( totalNbOfPckRcvd == 0 ) {
        return TrafficAnalysis::NoPacketsDelivered;
    }


    //////////////////////////////////////////////////////////////////////////////
    /// 2nd SCAN: It determines the average latency and the accepted traffic
    //////////////////////////////////////////////////////////////////////////////
    unsigned long int latency = 0;
    for( x = 0; x < xSize; x++ ) {
        for( y = 0; y < ySize; y++ ) {
            unsigned int nbOfPacketsReceived = (unsigned int) this->packets[x][y]->size();
            for( i = 0; i < nbOfPacketsReceived; i++) {
                PacketInfo* pckInf = this->packets[x][y]->at(i);
                unsigned int trafficClass = pckInf->getTrafficClass();

                if( (pckInf->getReceivedPacketId() > ((unsigned long int)(lower * nbOfPacketsReceived)))
                  &&(pckInf->getReceivedPacketId() <= ((unsigned long int)(upper * nbOfPacketsReceived))) ) {

                    // Accumulated required BW
                    accRequiredBw += pckInf->getRequiredBandwidth();
                    classAccRequiredBw[trafficClass] += (unsigned long) pckInf->getRequiredBandwidth();

                    // Packet latency in cycles
                    latency = pckInf->getLatency();

                    // Accumulated latency
                    accLatency += latency;
                    classAccLatency[trafficClass] += latency;

                    // Minimal latency
                    if( latency < minLatency ) { minLatency = latency;}
                    if( latency < classMinLatency[trafficClass] ) {classMinLatency[trafficClass] = latency;}

                    // Maximum latency
                    if(latency > maxLatency){ maxLatency = latency; }
                    if(latency > classMaxLatency[trafficClass]) { classMaxLatency[trafficClass] = latency; }

                    if( pckInf->getDeadline() != 0 ) {
                        accPckWithDeadline[trafficClass]++;
                        classAccPckWithDeadline[trafficClass]++;

                        if( (latency*tClk) > pckInf->getDeadline() ) {
                            accMissedDeadlines[trafficClass]++;
                            classAccMissedDeadlines[trafficClass]++;
                        }
                    }

                    // Number of packets and flits analyzed
                    accNbOfPck++;
                    classAccNbOfPck[trafficClass]++;

                    accNbOfFlits += pckInf->getPacketSize();
                    classAccNbOfFlits[trafficClass] += pckInf->getPacketSize();

                    // It identifies the end cycle of the last packet before the interval of analysis
                    // This time will define the begin of the interval used do calculate the Accepted Traffic
                    if (i != 0) {
                       PacketInfo* pckInf_previous = this->packets[x][y]->at(i-1);
                       endCycle = pckInf_previous->getCycleReceivedTrailer();
                    } else {
                       endCycle = 0;
                    }

                    if( endCycle < smallestEndCycle ) {
                        smallestEndCycle = endCycle;
                    }
                }


                // It get information to determine the time of simulation (in simulation cycles)
                if( pckInf->getReceivedPacketId() == ((unsigned long int)(lower * nbOfPacketsReceived + 1)) ) {
                    startCycle = pckInf->getCycleCreationPacket();
                    if( startCycle < smallestStartCycle ) {
                        smallestStartCycle = startCycle;
                    }
                }
                if( pckInf->getReceivedPacketId() == ((unsigned long int)(upper * nbOfPacketsReceived)) ) {
                    endCycle = pckInf->getCycleReceivedTrailer();
                    if( endCycle > biggestEndCycle ) {
                        biggestEndCycle = endCycle;
                    }
                }
            }
        }
    }

    // It calculates the simulated time

    if (lower == 0)
        simulatedTimeCycles = biggestEndCycle;// + 1;// - smallestStartCycle + 1;
    else
        simulatedTimeCycles = biggestEndCycle - smallestEndCycle;// + 1;// - smallestStartCycle + 1;

    simulatedTimeNs     = simulatedTimeCycles * (unsigned long) tClk;

    // It calculates the accepted traffic
    acceptedTrafficFlits = (float) accNbOfFlits/(float)(xSize*ySize)/(float)simulatedTimeCycles;
//    qDebug() << QString("Simulated time cycles: %1").arg(simulatedTimeCycles);
    sprintf(str,"%.4f",acceptedTrafficFlits);
    float tmpFloat = (float) atof(str);
    acceptedTrafficBps = (float) (tmpFloat*(dataWidth*fClk));

    // It calculates the average latency
    avgLatency    = ((float)accLatency) / (float) accNbOfPck;
    avgRequiredBw = accRequiredBw / (float) accNbOfPck;// / (float) (xSize*ySize);

    // It do the same for the traffic classes
    for( unsigned int trafficClass = 0; trafficClass < MAX_CLASSES; trafficClass++ ) {
        // It calculates the accepted traffic for the current class
        classAcceptedTrafficFlits[trafficClass] = (float) classAccNbOfFlits[trafficClass]/(float)(xSize*ySize)/(float)simulatedTimeCycles;
        sprintf(str,"%.4f",classAcceptedTrafficFlits[trafficClass]);
        tmpFloat = (float) atof(str);
        classAcceptedTrafficBps[trafficClass] = (float) (tmpFloat*(dataWidth*fClk));

        // It calculates the average latency for the current class
        classAvgLatency[trafficClass]    = ((float) classAccLatency[trafficClass]) / (float) classAccNbOfPck[trafficClass];
        classAvgRequiredBw[trafficClass] = (float) classAccRequiredBw[trafficClass] / (float) classAccNbOfPck[trafficClass];
    }

    //////////////////////////////////////////////////////////////////////////////
    /// 3rd SCAN: It determines the standard deviation of the latency
    //////////////////////////////////////////////////////////////////////////////
    for( x = 0; x < xSize; x++ ) {
        for( y = 0; y < ySize; y++ ) {
            unsigned int nbOfPacketsReceived = (unsigned int) this->packets[x][y]->size();
            for( i = 0; i < nbOfPacketsReceived; i++) {
                PacketInfo* pckInf = this->packets[x][y]->at(i);
                if( (pckInf->getReceivedPacketId() > ((unsigned long int)(lower * nbOfPacketsReceived)))
                  &&(pckInf->getReceivedPacketId() <= ((unsigned long int)(upper * nbOfPacketsReceived))) ) {
                    unsigned long int latency = pckInf->getLatency();
                    accLatencyMinusAvgLatency += pow( ((float)(latency - avgLatency)),2 );
                    classAccLatencyMinusAvgLatency[pckInf->getTrafficClass()] += pow( ((float)(latency - classAvgLatency[pckInf->getTrafficClass()])),2 );
                }
            }
        }
    }

    // It calculates the standard deviation
    stdevLatency = sqrt( accLatencyMinusAvgLatency / (float)accNbOfPck );
    for(unsigned int trafficClass = 0; trafficClass < MAX_CLASSES; trafficClass++) {
        if( classAccNbOfPck[trafficClass] != 0.0 ) {
            classStdevLatency[trafficClass] = sqrt( classAccLatencyMinusAvgLatency[trafficClass]/(float)classAccNbOfPck[trafficClass] );
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    /// 4th SCAN: It determines the frequency of latencies and the ideal average latency
    //////////////////////////////////////////////////////////////////////////////

    // It initializes the counters
    // Global latency range counters
    latencyRngInc = (unsigned long int) roundf( ((float) maxLatency - minLatency ) / 100.0f );

    if( latencyRngInc < 1 ) {
        latencyRngInc = 1;
    }

    latencyRng[0] = minLatency;
    latencyRngPcks[0] = 0;

    for( i = 1; i < 100; i++ ) {
        latencyRng[i] = latencyRng[i-1] + latencyRngInc;
        latencyRngPcks[i] = 0;
    }

    for( i = 0; i < IDEAL_LATENCY_RNG_SIZE; i++ ) {
        idealLatencyRng[i] = 0;
    }

    // Global latency range counters for each traffic class
    for( unsigned int trafficClass = 0; trafficClass < MAX_CLASSES; trafficClass++ ) {
        classLatencyRngInc[trafficClass] = (unsigned long int) roundf( ((float) classMaxLatency[trafficClass]
                                                                  - classMinLatency[trafficClass]) / 100.0f );

        if( classLatencyRngInc[trafficClass] < 1 ) {
            classLatencyRngInc[trafficClass] = 1;
        }
        classLatencyRng[trafficClass][0] = classMinLatency[trafficClass];
        classLatencyRngPcks[trafficClass][0] = 0;

        for( i = 1; i < 100; i++ ) {
            classLatencyRng[trafficClass][i] = classLatencyRng[trafficClass][i-1]
                    + classLatencyRngInc[trafficClass];
            classLatencyRngPcks[trafficClass][i] = 0;
        }
    }

    for( x = 0; x < xSize; x++ ) {
        for( y = 0; y < ySize; y++ ) {
            unsigned int nbOfPacketsReceived = (unsigned int) this->packets[x][y]->size();
            for( i = 0; i < nbOfPacketsReceived; i++) {
                PacketInfo* pckInf = this->packets[x][y]->at(i);
                unsigned int trafficClass = pckInf->getTrafficClass();
                if ( (pckInf->getReceivedPacketId() >  ((unsigned long int) (lower * nbOfPacketsReceived)))
                  && (pckInf->getReceivedPacketId() <= ((unsigned long int) (upper * nbOfPacketsReceived)))) {
                    // It calculates the latency
                    latency = pckInf->getLatency();

                    // It increments the range latency counters
                    for( unsigned int z = 0; z < 99; z++ ) {
                        if( (latency >= latencyRng[z]) && (latency < latencyRng[z+1]) ) {
                            latencyRngPcks[z]++;
                        }
                        if( (latency >= classLatencyRng[trafficClass][z]) && (latency < classLatencyRng[trafficClass][z+1]) ) {
                            classLatencyRngPcks[trafficClass][z]++;
                        }
                    }

                    unsigned int nbOfRouters = pckInf->getNumberOfRoutersInPath();
                    unsigned int nbOfLinks   = pckInf->getNumberOfLinksInPath();

                    idealLatency = calculateIdealLatency(nbOfLinks,nbOfRouters,pckInf->getPacketSize());

                    idealAccLatency += idealLatency;

                    // It increments the latency counters which determines how many
                    // packets are equal or greater that the ideal latency
                    idealLatencyIndex = (unsigned long int) roundl( (((float)latency/(float)idealLatency)-1)*10.0 );

                    if( idealLatencyIndex <= IDEAL_LATENCY_RNG_SIZE - 2 ) {
                        idealLatencyRng[idealLatencyIndex]++;
                    } else {
                        idealLatencyRng[IDEAL_LATENCY_RNG_SIZE-1]++;
                    }
                }
            }
        }
    }

    idealAverageLatency = (float) idealAccLatency / (float) accNbOfPck;

    for( unsigned int i = 0; i < MAX_CLASSES; i++ ) {
        if( accPckWithDeadline[i] > 0 ) {
            metDeadlinesPer[i] = 100 - ((float) ( (accMissedDeadlines[i]*100)/accPckWithDeadline[i]) );
        } else {
            metDeadlinesPer[i] = -1;
        }

        if( classAccPckWithDeadline[i] > 0 ) {
            classMetDeadlinesPer[i] = 100 - ((float) ((classAccMissedDeadlines[i]*100)/classAccPckWithDeadline[i]) );
        } else {
            classMetDeadlinesPer[i] = -1;
        }
    }

    return TrafficAnalysis::Ok;
}

TrafficAnalysis::StatusAnalysis PerformanceAnalysis::analyzeIndividualFlows() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/PerformanceAnalysis::analyzeIndividualFlows" << std::endl;
#endif

    unsigned int x,y,xSrc,ySrc,i;

    for( x = 0; x < xSize; x++ ) {
        for( y = 0; y < ySize; y++ ) {
            //////////////////////////////////////////////////////////////////////////
            ///
            /// It initializes the variables for flow analysis
            ///
            ///////////////////////////////////////////////////////////////////////////
            // It initializes the variables used to detemine the metrics for individual flows
            for (xSrc = 0; xSrc < xSize; xSrc++) {
                for (ySrc = 0; ySrc < ySize; ySrc++) {
                    for (unsigned int flowId = 0; flowId < MAX_FLOW_IDS; flowId++) {
                        flowAccNbOfPck[xSrc][ySrc][flowId]        = 0;
                        flowAccNbOfFlits[xSrc][ySrc][flowId]      = 0;
                        flowAccRequiredBw[xSrc][ySrc][flowId]     = 0;
                        flowAvgRequiredBwNorm[xSrc][ySrc][flowId] = 0;
                        flowAvgRequiredBw[xSrc][ySrc][flowId]     = 0;

                        flowAcceptedTrafficFlits[xSrc][ySrc][flowId] = 0;
                        flowAcceptedTrafficBps[xSrc][ySrc][flowId]   = 0;

                        for (i=0;i<MAX_CLASSES;i++){
                            flowAccPckWithDeadline[xSrc][ySrc][flowId][i] = 0;
                            flowAccMissedDeadlines[xSrc][ySrc][flowId][i] = 0;
                            flowMissedDeadlinesPer[xSrc][ySrc][flowId][i] = 0;
                            flowMetDeadlinesPer[xSrc][ySrc][flowId][i]    = 0;
                        }

                        flowAccLatency[xSrc][ySrc][flowId] 				  = 0;
                        flowAvgLatency[xSrc][ySrc][flowId] 				  = 0;
                        flowAccLatencyMinusAvgLatency[xSrc][ySrc][flowId] = 0;
                        flowStdevLatency[xSrc][ySrc][flowId] 			  = 0;
                        flowMinLatency[xSrc][ySrc][flowId] 				  = ULONG_MAX;
                        flowMaxLatency[xSrc][ySrc][flowId] 				  = 0;
                    }
                }
            }
            //////////////////////////////////////////////////////////////////////////////
            ///
            /// FLOW's 1st SCAN: It determines the average latency and the accepted traffic
            ///
            //////////////////////////////////////////////////////////////////////////////
            unsigned int nbOfPacketsReceived = (unsigned int) this->packets[x][y]->size();
            unsigned long int latency = 0;
            for( i = 0; i < nbOfPacketsReceived; i++ ) {
                PacketInfo* pckInf = this->packets[x][y]->at(i);
                xSrc = pckInf->getXSource();
                ySrc = pckInf->getYSource();
                unsigned int flowId = pckInf->getFlowId();
                unsigned int trafficClass = pckInf->getTrafficClass();
                // It determines the accumulated latency and the accumulated number of flits received
                if( (pckInf->getReceivedPacketId() > ((unsigned long int)(lower*nbOfPacketsReceived)))
                  &&(pckInf->getReceivedPacketId() <= ((unsigned long int)(upper*nbOfPacketsReceived))) ) {

                    flowAccRequiredBw[xSrc][ySrc][flowId] += (unsigned long) pckInf->getRequiredBandwidth();

                    // Packet latency in cycles
                    latency = pckInf->getLatency();

                    // Accumulated latency
                    flowAccLatency[xSrc][ySrc][flowId] += latency;

                    // Minimal latency
                    if( latency < flowMinLatency[xSrc][ySrc][flowId] ) {
                        flowMinLatency[xSrc][ySrc][flowId] = latency;
                    }

                    // Maximum latency
                    if(latency > flowMaxLatency[xSrc][ySrc][flowId]) {
                        flowMaxLatency[xSrc][ySrc][flowId] = latency;
                    }

                    // Missed deadlines
                    if( pckInf->getDeadline() != 0 ) {
                        flowAccPckWithDeadline[xSrc][ySrc][flowId][trafficClass]++;
                        if( (latency * tClk) > pckInf->getDeadline() ) {
                            flowAccMissedDeadlines[xSrc][ySrc][flowId][trafficClass]++;
                        }
                    }

                    // Number of flits received
                    flowAccNbOfFlits[xSrc][ySrc][flowId] += pckInf->getPacketSize();
                    flowAccNbOfPck[xSrc][ySrc][flowId]++;
                }

                // It get information to determine the time of simulation (in simulation cycles)
                if( pckInf->getReceivedPacketId() == ((unsigned long int) (lower * nbOfPacketsReceived + 1)) ) {
                    startCycle = pckInf->getCycleCreationPacket();
                    if( startCycle < smallestStartCycle ) {
                        smallestStartCycle = startCycle;
                    }
                }
                if( pckInf->getReceivedPacketId() == ((unsigned long int) (upper * nbOfPacketsReceived)) ) {
                    endCycle = pckInf->getCycleReceivedTrailer();
                    if( endCycle > biggestEndCycle ) {
                        biggestEndCycle = endCycle;
                    }
                }
            }

            // It computes only the number of packets received between the lower and upper bounds
            // Examples: Considering that 100 packets were received and bounds equal 0.1 and 0.9,
            // it will consider 80% of the received packets (that is, 80 packets)
            // acc_nb_of_pck +=  (unsigned long int)roundf(((upper - lower) * nb_of_pck_rcvd[x][y]));
            // fscanf(fpIn,"%s", str);
            // simulated_time_cycles = (unsigned long int) atol(str); // - warmup

            // It calculates the target metrics of this scan
            simulatedTimeCycles = biggestEndCycle - smallestStartCycle + 1;
            simulatedTimeNs     = simulatedTimeCycles * (unsigned long) tClk;

            for (xSrc = 0; xSrc < xSize; xSrc++) {
                for (ySrc = 0; ySrc < ySize; ySrc++) {
                    for (unsigned int flowId = 0; flowId < MAX_FLOW_IDS; flowId++) {
                        if( flowAccNbOfPck[xSrc][ySrc][flowId] ) {
                            flowAvgRequiredBw[xSrc][ySrc][flowId] = ((float) flowAccRequiredBw[xSrc][ySrc][flowId])
                                    / (float) (flowAccNbOfPck[xSrc][ySrc][flowId]);

                            if (flowAvgRequiredBw[xSrc][ySrc][flowId] <= channelBwAvailable) {
                                flowAvgRequiredBwNorm[xSrc][ySrc][flowId] = flowAvgRequiredBw[xSrc][ySrc][flowId]
                                        / channelBwAvailable;
                            } else {
                                flowAvgRequiredBwNorm[xSrc][ySrc][flowId] = 1.0;
                            }

                            flowAvgLatency[xSrc][ySrc][flowId] = ((float) flowAccLatency[xSrc][ySrc][flowId])
                                    / (float) (flowAccNbOfPck[xSrc][ySrc][flowId]);
                        } else {
                            flowAvgRequiredBw[xSrc][ySrc][flowId]     = 0.0f;
                            flowAvgRequiredBwNorm[xSrc][ySrc][flowId] = 0.0f;
                            flowAvgLatency[xSrc][ySrc][flowId]        = 0.0f;
                        } // if ((flow_acc_nb_of_pck[x_src][y_src][flow_id]))

                        flowAcceptedTrafficFlits[xSrc][ySrc][flowId] = (float) flowAccNbOfFlits[xSrc][ySrc][flowId]
                                / simulatedTimeCycles;

//                        qDebug()<<flowAccNbOfFlits[xSrc][ySrc][flowId];
//                        qDebug()<<simulatedTimeCycles;
//                        qDebug()<<flowAcceptedTrafficFlits[xSrc][ySrc][flowId];

//                        qDebug() << QString("%1 , %2, %3")
//                                    .arg(flowAccNbOfFlits[xSrc][ySrc][flowId])
//                                    .arg(simulatedTimeCycles)
//                                    .arg(flowAcceptedTrafficFlits[xSrc][ySrc][flowId]);

                        flowAcceptedTrafficBps[xSrc][ySrc][flowId] = (float) flowAcceptedTrafficFlits[xSrc][ySrc][flowId] * dataWidth/(tClk/1000);
                    } // for (flow_id = 0; flow_id < MAX_FLOW_IDS; flow_id++)
                } // for (y_src=0; y_src<y_size; y_src++)
            } //for (x_src=0; x_src<x_size; x_src++)
            //////////////////////////////////////////////////////////////////////////////
            ///
            /// FLOW's 2nd SCAN: it determines the standard deviation of the latency
            ///
            //////////////////////////////////////////////////////////////////////////////
            // It opens the input file
            for( i = 0; i < nbOfPacketsReceived; i++ ) {
                PacketInfo* pckInf = this->packets[x][y]->at(i);
                xSrc = pckInf->getXSource();
                ySrc = pckInf->getYSource();
                unsigned int flowId = pckInf->getFlowId();
                if( (pckInf->getReceivedPacketId() > ((unsigned long int)(lower*nbOfPacketsReceived)))
                  &&(pckInf->getReceivedPacketId() <= ((unsigned long int)(upper*nbOfPacketsReceived))) ) {
                    latency = pckInf->getLatency();
                    flowAccLatencyMinusAvgLatency[xSrc][ySrc][flowId] += pow( (((float)latency) - flowAvgLatency[xSrc][ySrc][flowId]),2 );
                }
            }

            /// CHANGE: Diferente do ATR -> Linha: 1114
            for(xSrc = 0; xSrc < xSize; xSrc++) {
                for( ySrc = 0; ySrc < ySize; ySrc++ ) {
                    for( unsigned int flowId = 0; flowId < MAX_FLOW_IDS; flowId++ ) {
                        // If requiredBw is greather than 0, it calculates the standard deviation, if not, it sets it to 0
                        if(flowAvgRequiredBw[xSrc][ySrc][flowId]) {
                            flowStdevLatency[xSrc][ySrc][flowId] = sqrt(flowAccLatencyMinusAvgLatency[xSrc][ySrc][flowId]/(float)flowAccNbOfPck[xSrc][ySrc][flowId]);
                        } else {
                            flowStdevLatency[xSrc][ySrc][flowId] = 0.0f;
                        }
                    }
                }
            }

            //////////////////////////////////////////////////////////////////////////////
            ///
            /// FLOW's 3rd SCAN: It determines the frequency of latencies and the ideal avg latency
            ///
            //////////////////////////////////////////////////////////////////////////////
            // It initializes the counters

            // Flow latency range counters
            for(xSrc = 0; xSrc < xSize; xSrc++) {
                for(ySrc = 0; ySrc < ySize; ySrc++) {
                    for(unsigned int flowId = 0; flowId < MAX_FLOW_IDS; flowId++) {
                        flowLatencyRngInc[xSrc][ySrc][flowId] = (unsigned long int) roundf(((float)flowMaxLatency[xSrc][ySrc][flowId] -
                                                                    (float)flowMinLatency[xSrc][ySrc][flowId])/100.0f);

                        if(flowLatencyRngInc[xSrc][ySrc][flowId] < 1) {
                            flowLatencyRngInc[xSrc][ySrc][flowId] = 1;
                        }

                        flowLatencyRng[xSrc][ySrc][flowId][0] = flowMinLatency[xSrc][ySrc][flowId];
                        flowLatencyRngPcks[xSrc][ySrc][flowId][0] = 0;

                        for ( i = 1; i < 100; i++) {
                            flowLatencyRng[xSrc][ySrc][flowId][i] = flowLatencyRng[xSrc][ySrc][flowId][i-1] +
                                                                    flowLatencyRngInc[xSrc][ySrc][flowId];
                            flowLatencyRngPcks[xSrc][ySrc][flowId][i] = 0;
                        }
                    }
                }
            }

            for( i = 0; i < IDEAL_LATENCY_RNG_SIZE; i++ ) {
                idealLatencyRng[i] = 0;
            }

            /// CHANGE: Não era zerado o acumulador da latencia ideal
            idealAccLatency = 0;
            for( unsigned int pckIndex = 0; pckIndex < nbOfPacketsReceived; pckIndex++ ) {
                PacketInfo* pckInf = this->packets[x][y]->at(pckIndex);
                xSrc = pckInf->getXSource();
                ySrc = pckInf->getYSource();
                // It updates the latency counters
                if( (pckInf->getReceivedPacketId() > ((unsigned long int) (lower * nbOfPacketsReceived)))
                  &&(pckInf->getReceivedPacketId() <= ((unsigned long int) (upper * nbOfPacketsReceived))) ) {

                    // It calculates the latency
                    latency = pckInf->getLatency();

                    // It increments the range latency counters
                    for( i = 0; i < 99; i++ ) {
                        if( (latency >= latencyRng[i]) && (latency < latencyRng[i+1]) ) {
                            latencyRngPcks[i]++;
                        }
                    }

                    for( unsigned int flowId = 0; flowId < MAX_FLOW_IDS; flowId++ ) {
                        for( i = 0; i < 99; i++ ) {
                            if( (latency >= flowLatencyRng[xSrc][ySrc][flowId][i])
                              &&(latency < flowLatencyRng[xSrc][ySrc][flowId][i+1]) ) {

                                flowLatencyRngPcks[xSrc][ySrc][flowId][i]++;
                            }
                        }
                    }

                    /// CHANGE: Linha 1227 no ATR
                    unsigned int nbOfRouters = pckInf->getNumberOfRoutersInPath();
                    unsigned int nbOfLinks   = pckInf->getNumberOfLinksInPath();

                    // It determines the ideal latency for the flow
                    idealLatency = this->calculateIdealLatency(nbOfLinks,nbOfRouters,pckInf->getPacketSize());

                    idealAccLatency += idealLatency;

                    // It increments the latency counters which determines how many
                    // packets are equal or greater that the ideal latency
                    idealLatencyIndex = (unsigned long int) roundl( (((float) latency / (float)idealLatency)-1)*10.0f );

                    if( idealLatencyIndex <= IDEAL_LATENCY_RNG_SIZE-2 ) {
                        idealLatencyRng[idealLatencyIndex]++;
                    } else {
                        idealLatencyRng[IDEAL_LATENCY_RNG_SIZE-1]++;
                    }
                }
            }

            // It determines the flow's missed deadlines
            for (xSrc = 0; xSrc < xSize; xSrc++) {
                for (ySrc = 0; ySrc < ySize; ySrc++) {
                    for (unsigned int flowId = 0; flowId < MAX_FLOW_IDS; flowId++) {
                        for (i = 0; i < MAX_CLASSES; i++){
                            if (flowAccPckWithDeadline[xSrc][ySrc][flowId][i] > 0) {
                                flowMissedDeadlinesPer[xSrc][ySrc][flowId][i] =
                                        ( ((float)flowAccMissedDeadlines[xSrc][ySrc][flowId][i]*100.0f)/
                                                  (float) flowAccPckWithDeadline[xSrc][ySrc][flowId][i] );
                                flowMetDeadlinesPer[xSrc][ySrc][flowId][i] = 100 -
                                        ( ((float) flowAccMissedDeadlines[xSrc][ySrc][flowId][i]*100.0f)/
                                                  (float) flowAccPckWithDeadline[xSrc][ySrc][flowId][i] );
                            } else {
                                flowMissedDeadlinesPer[xSrc][ySrc][flowId][i] = -1.0f;
                                flowMetDeadlinesPer[xSrc][ySrc][flowId][i] = -1.0f;
                            }
                        }
                    }
                }
            }

            TrafficAnalysis::StatusAnalysis status = this->createReportsForIndividualFlows(x,y);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
        } // for(y=0;y<ySize;y++)
    } // for(x=0;x<xSize;x++)

    return TrafficAnalysis::Ok;
}

TrafficAnalysis::StatusAnalysis PerformanceAnalysis::createReportsForAllFlows() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/PerformanceAnalysis::createReportsForAllFlows" << std::endl;
#endif

    FILE* fpOut;

    // It creates the summary report
    sprintf(str,"%s/summary",this->resultDir);
    if( (fpOut=fopen(str,"a+t")) == NULL ) {
        printf("Error: Impossible to create file %s",str);
        return TrafficAnalysis::NoOutputFile;
    } else {
        sprintf(str,"%.1f\t%lu\t%.4f\t%.4f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f",
                fClk,
                accNbOfPck,
                (avgRequiredBw <= channelBwAvailable ? avgRequiredBw/channelBwAvailable : 1.0f ),
                acceptedTrafficFlits,
                idealAverageLatency,
                avgLatency,
                stdevLatency,
                (float)minLatency,
                (float)maxLatency,
                avgRequiredBw,					// bps
                acceptedTrafficBps,				// bps
                idealAverageLatency * tClk, // ns
                avgLatency * tClk, // ns
//                stdevLatency, //,* tclk, // ns
                (float)minLatency * tClk, // ns
                (float)maxLatency * tClk, // ns
                metDeadlinesPer[0],
                metDeadlinesPer[1],
                metDeadlinesPer[2],
                metDeadlinesPer[3]
                );
        // It replaces ',' by '.'
        unsigned int i = 0;
        do {
            if(str[i]==',') str[i] = '.';
            i++;
        } while( str[i] != '\0' );

        fprintf(fpOut,"%s\n",str);
        fclose(fpOut);
    }

    // It creates the summary histogram report
    sprintf(str,"%s/Results/summary_latency_histogram",workDir);
    if( (fpOut=fopen(str,"wt")) == NULL ) {
        printf("ERROR: Impossible to create file %s",str);
        return TrafficAnalysis::NoOutputFile;
    } else {
        for(unsigned int x = 0; x < 100; x++) {
            if( latencyRngPcks[x] != 0 ) {
                float tmpFloat = 100.0f * ((float)latencyRngPcks[x]/(float)accNbOfPck);
                sprintf(str,"%lu\t%lu\t%.1f",latencyRng[x],latencyRngPcks[x],tmpFloat);
                fprintf(fpOut,"%s\n",str);
            }
        }
        fclose(fpOut);
    }

    // It creates the summary report for the traffic classes
    for( unsigned int trafficClass = 0; trafficClass < MAX_CLASSES; trafficClass++ ) {
        if( classAccNbOfPck[trafficClass] > 0 ) {
            sprintf(str,"%s/class_%u",resultDir,trafficClass);
            if( (fpOut=fopen(str,"a+t")) == NULL ) {
                printf("ERROR: Impossible to create file %s",str);
                return TrafficAnalysis::NoOutputFile;
            } else {
                // It determines the average normalized required BW for the current class
                if( classAvgRequiredBw[trafficClass] <= channelBwAvailable ) {
                    classAvgRequiredBwNorm[trafficClass] = classAvgRequiredBw[trafficClass] / channelBwAvailable;
                } else {
                    classAvgRequiredBwNorm[trafficClass] = 1.0f;
                }

                // It determines the %Met deadlines for the current class
                for(unsigned int i = 0; i < MAX_CLASSES; i++) {
                    metDeadlinesPer[i] = -1;
                }
                metDeadlinesPer[trafficClass] = classMetDeadlinesPer[trafficClass];

                // It writes into report
                sprintf(str,"%.1f\t%lu\t%.4f\t%.4f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f",
                        fClk,
                        classAccNbOfPck[trafficClass],

                        classAvgRequiredBwNorm[trafficClass],
                        classAcceptedTrafficFlits[trafficClass],
                        0.0f,//class_ideal_avg_latency[traffic_class],
                        classAvgLatency[trafficClass],
                        classStdevLatency[trafficClass],
                        (float) classMinLatency[trafficClass],
                        (float) classMaxLatency[trafficClass],

                        classAvgRequiredBw[trafficClass],									// bps
                        classAcceptedTrafficBps[trafficClass],						// bps
                        0.0f,//(class_ideal_avg_latency[traffic_class] ) * tclk, 		// ns
                        0.0f,//(class_avg_latency_cycles[traffic_class]) * tclk, // ns
//                        classStdevLatency[trafficClass] , //,* tclk, // ns
                        (float)classMinLatency[trafficClass] * tClk, 		// ns
                        (float)classMaxLatency[trafficClass] * tClk, 		// ns

                        metDeadlinesPer[0],
                        metDeadlinesPer[1],
                        metDeadlinesPer[2],
                        metDeadlinesPer[3]
                        );
                unsigned int i = 0;
                do {
                    if(str[i] == ',') str[i] = '.';
                    i++;
                } while(str[i] != '\0');
                fprintf(fpOut,"%s\n",str);
                fclose(fpOut);
            }
        }
    }


    // It creates the summary histogram report for the traffic classes
    for( unsigned int trafficClass = 0; trafficClass < MAX_CLASSES; trafficClass++ ) {
        if( classAccNbOfPck[trafficClass] > 0 ) {
            sprintf(str,"%s/Results/class_%u_latency_histogram",workDir,trafficClass);
            if( (fpOut=fopen(str,"wt")) == NULL ) {
                printf("ERROR: Impossible to create file %s",str);
                return TrafficAnalysis::NoOutputFile;
            } else {
                for( unsigned int i = 0; i < 100; i++ ) {
                    if( classLatencyRngPcks[trafficClass][i] != 0 ) {
                        float tmpFloat = 100.0f * (((float)classLatencyRngPcks[trafficClass][i]/(float)classAccNbOfPck[trafficClass]));
                        sprintf(str,"%lu\t%lu\t%.1f",classLatencyRng[trafficClass][i],classLatencyRngPcks[trafficClass][i],tmpFloat);
                        fprintf(fpOut,"%s\n",str);
                    }
                }
                fclose(fpOut);
            }
        }
    }

    return TrafficAnalysis::Ok;
}

TrafficAnalysis::StatusAnalysis PerformanceAnalysis::createReportsForIndividualFlows(unsigned int xDest, unsigned int yDest) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/PerformanceAnalysis::createReportsForIndividualFlows" << std::endl;
#endif

    FILE* fpOut;

    // It creates the flow report
    for( unsigned int xSrc = 0; xSrc < xSize; xSrc++ ) {
        for( unsigned int ySrc = 0; ySrc < ySize; ySrc++ ) {
            for( unsigned int flowId = 0; flowId < MAX_FLOW_IDS; flowId++ ) {
                if( flowAccNbOfPck[xSrc][ySrc][flowId] > 0 ) {
                    sprintf(str,"%s/flow_%u_%u_%u_%u_%u",resultDir,xSrc,ySrc,xDest,yDest,flowId);
                    if( (fpOut=fopen(str,"a+t")) == NULL ) {
                        printf("ERROR: Impossible to create file %s",str);
                        return TrafficAnalysis::NoOutputFile;
                    } else {
                        sprintf(str,"%.1f\t%lu\t%.4f\t%.4f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f",
                                fClk,
                                flowAccNbOfPck[xSrc][ySrc][flowId],
                                flowAvgRequiredBwNorm[xSrc][ySrc][flowId],
                                flowAcceptedTrafficFlits[xSrc][ySrc][flowId],
                                0.0f,//*ret_ideal_avg_latency,//??
                                flowAvgLatency[xSrc][ySrc][flowId],
                                flowStdevLatency[xSrc][ySrc][flowId],
                                (float) flowMinLatency[xSrc][ySrc][flowId],
                                (float) flowMaxLatency[xSrc][ySrc][flowId],

                                flowAvgRequiredBw[xSrc][ySrc][flowId],
                                flowAcceptedTrafficBps[xSrc][ySrc][flowId],
                                0.0f,//(*ret_ideal_avg_latency ) * tclk, // ns
                                flowAvgLatency[xSrc][ySrc][flowId] * tClk,
//                                flowStdevLatency[xSrc][ySrc][flowId],//* tclk,
                                (float) flowMinLatency[xSrc][ySrc][flowId] * tClk,
                                (float) flowMaxLatency[xSrc][ySrc][flowId] * tClk,

                                flowMetDeadlinesPer[xSrc][ySrc][flowId][0],
                                flowMetDeadlinesPer[xSrc][ySrc][flowId][1],
                                flowMetDeadlinesPer[xSrc][ySrc][flowId][2],
                                flowMetDeadlinesPer[xSrc][ySrc][flowId][3]
                                );
                        // It replaces ',' by '.'
                        unsigned int i = 0;
                        do {
                            if(str[i] == ',') str[i] = '.';
                            i++;
                        }while(str[i] != '\0');
                        fprintf(fpOut,"%s\n",str);
                        fclose(fpOut);
                    }
                }
            }
        }
    }

    // It creates latency histogram report
    for( unsigned int xSrc = 0; xSrc < xSize; xSrc++ ) {
        for( unsigned int ySrc = 0; ySrc < ySize; ySrc++ ) {
            for( unsigned int flowId = 0; flowId < MAX_FLOW_IDS; flowId++ ) {
                if( flowAccNbOfPck[xSrc][ySrc][flowId] > 0 ) {
                    sprintf(str,"%s/Results/flow_%u_%u_%u_%u_%u_latency_histogram",workDir,xSrc,ySrc,xDest,yDest,flowId);
                    if( (fpOut=fopen(str,"wt")) == NULL ) {
                        printf("ERROR: Impossible to create file %s",str);
                        return TrafficAnalysis::NoOutputFile;
                    } else {
                        for( unsigned int i = 0; i < 100; i++ ) {
                            if( flowLatencyRngPcks[xSrc][ySrc][flowId][i] != 0 ) {
                                float tmpFloat = 100.0f * ((float) flowLatencyRngPcks[xSrc][ySrc][flowId][i]
                                                           / (float) flowAccNbOfPck[xSrc][ySrc][flowId] );
                                sprintf(str,"%lu\t%lu\t%.1f",flowLatencyRng[xSrc][ySrc][flowId][i],
                                        flowLatencyRngPcks[xSrc][ySrc][flowId][i],tmpFloat);
                                fprintf(fpOut,"%s\n",str);
                            }
                        }
                        fclose(fpOut);
                    }
                }
            }
        }
    }

    return TrafficAnalysis::Ok;

}

PerformanceAnalysis::~PerformanceAnalysis() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Model/Analysis/PerformanceAnalysis" << std::endl;
#endif

    // Deallocating memory
    // Fourth dimension
    for(unsigned int i = 0; i < xSize; i++) {
        for(unsigned int x = 0; x < ySize; x++) {
            for(unsigned int z = 0; z < MAX_FLOW_IDS; z++) {
                delete[] flowAccPckWithDeadline[i][x][z];
                delete[] flowAccMissedDeadlines[i][x][z];
                delete[] flowMissedDeadlinesPer[i][x][z];
                delete[] flowMetDeadlinesPer[i][x][z];
                delete[] flowLatencyRng[i][x][z];
                delete[] flowLatencyRngPcks[i][x][z];
            }
        }
    }

    // Third dimension
    for(unsigned int i = 0; i < xSize; i++) {
        for(unsigned int x = 0; x < ySize; x++) {
            for(unsigned int z = 0; z < this->packets[i][x]->size(); z++) {
                PacketInfo* pck = this->packets[i][x]->at(z);
                delete pck;
            }
            this->packets[i][x]->clear();
            delete this->packets[i][x];

            delete[] flowAccNbOfPck[i][x];
            delete[] flowAccNbOfFlits[i][x];
            delete[] flowAccRequiredBw[i][x];
            delete[] flowAvgRequiredBw[i][x];
            delete[] flowAvgRequiredBwNorm[i][x];
            delete[] flowAcceptedTrafficFlits[i][x];
            delete[] flowAcceptedTrafficBps[i][x];
            delete[] flowAccPckWithDeadline[i][x];
            delete[] flowAccMissedDeadlines[i][x];
            delete[] flowMissedDeadlinesPer[i][x];
            delete[] flowMetDeadlinesPer[i][x];
            delete[] flowAccLatency[i][x];
            delete[] flowAvgLatency[i][x];
            delete[] flowAccLatencyMinusAvgLatency[i][x];
            delete[] flowStdevLatency[i][x];
            delete[] flowMinLatency[i][x];
            delete[] flowMaxLatency[i][x];
            delete[] flowLatencyRng[i][x];
            delete[] flowLatencyRngInc[i][x];
            delete[] flowLatencyRngPcks[i][x];
        }
    }

    // Second dimension
    for(unsigned int i = 0; i < xSize; i++) {
        delete[] this->packets[i];

        delete[] flowAccNbOfPck[i];
        delete[] flowAccNbOfFlits[i];
        delete[] flowAccRequiredBw[i];
        delete[] flowAvgRequiredBw[i];
        delete[] flowAvgRequiredBwNorm[i];
        delete[] flowAcceptedTrafficFlits[i];
        delete[] flowAcceptedTrafficBps[i];
        delete[] flowAccPckWithDeadline[i];
        delete[] flowAccMissedDeadlines[i];
        delete[] flowMissedDeadlinesPer[i];
        delete[] flowMetDeadlinesPer[i];
        delete[] flowAccLatency[i];
        delete[] flowAvgLatency[i];
        delete[] flowAccLatencyMinusAvgLatency[i];
        delete[] flowStdevLatency[i];
        delete[] flowMinLatency[i];
        delete[] flowMaxLatency[i];
        delete[] flowLatencyRng[i];
        delete[] flowLatencyRngInc[i];
        delete[] flowLatencyRngPcks[i];
    }

    // First dimension
    delete[] this->packets;

    delete[] flowAccNbOfPck;
    delete[] flowAccNbOfFlits;
    delete[] flowAccRequiredBw;
    delete[] flowAvgRequiredBw;
    delete[] flowAvgRequiredBwNorm;
    delete[] flowAcceptedTrafficFlits;
    delete[] flowAcceptedTrafficBps;
    delete[] flowAccPckWithDeadline;
    delete[] flowAccMissedDeadlines;
    delete[] flowMissedDeadlinesPer;
    delete[] flowMetDeadlinesPer;
    delete[] flowAccLatency;
    delete[] flowAvgLatency;
    delete[] flowAccLatencyMinusAvgLatency;
    delete[] flowStdevLatency;
    delete[] flowMinLatency;
    delete[] flowMaxLatency;
    delete[] flowLatencyRng;
    delete[] flowLatencyRngInc;
    delete[] flowLatencyRngPcks;

}
