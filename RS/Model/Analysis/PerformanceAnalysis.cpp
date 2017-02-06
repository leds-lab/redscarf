/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* PerformanceAnalysis.cpp
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
*
*/

#include "Model/Analysis/PerformanceAnalysis.h"

#include <cstdio>
#include <math.h>
#include <limits.h>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

PerformanceAnalysis::PerformanceAnalysis(unsigned short numElements, unsigned short dataWidth,
                                         float lower, float upper, float fClk,
                                         unsigned short fifoOutDepth,
                                         unsigned short flowControlType,
                                         const char* workDir, const char* resultDir) :
    TrafficAnalysis(numElements,dataWidth,lower,upper,fClk,fifoOutDepth,flowControlType,workDir,resultDir)
{
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/Analysis/PerformanceAnalysis" << std::endl;
#endif
    this->packets.resize(numElements);

    flowAccNbOfPck.resize(numElements);
    flowAccNbOfFlits.resize(numElements);
    flowAccRequiredBw.resize(numElements);
    flowAvgRequiredBw.resize(numElements);
    flowAvgRequiredBwNorm.resize(numElements);
    flowAcceptedTrafficFlits.resize(numElements);
    flowAcceptedTrafficBps.resize(numElements);
    flowAccPckWithDeadline.resize(numElements);
    flowAccMissedDeadlines.resize(numElements);
    flowMissedDeadlinesPer.resize(numElements);
    flowMetDeadlinesPer.resize(numElements);
    flowAccLatency.resize(numElements);
    flowAvgLatency.resize(numElements);
    flowIdealAvgLatency.resize(numElements);
    flowAccLatencyMinusAvgLatency.resize(numElements);
    flowStdevLatency.resize(numElements);
    flowMinLatency.resize(numElements);
    flowMaxLatency.resize(numElements);
    flowLatencyRng.resize(numElements);
    flowLatencyRngInc.resize(numElements);
    flowLatencyRngPcks.resize(numElements);


    for( unsigned int id = 0; id < numElements; id++ ) {
        flowAccNbOfPck[id].resize(MAX_FLOW_IDS,0);
        flowAccNbOfFlits[id].resize(MAX_FLOW_IDS,0);
        flowAccRequiredBw[id].resize(MAX_FLOW_IDS,0);
        flowAvgRequiredBw[id].resize(MAX_FLOW_IDS,0);
        flowAvgRequiredBwNorm[id].resize(MAX_FLOW_IDS,0);
        flowAcceptedTrafficFlits[id].resize(MAX_FLOW_IDS,0);
        flowAcceptedTrafficBps[id].resize(MAX_FLOW_IDS,0);
        flowAccPckWithDeadline[id].resize(MAX_FLOW_IDS);
        flowAccMissedDeadlines[id].resize(MAX_FLOW_IDS);
        flowMissedDeadlinesPer[id].resize(MAX_FLOW_IDS);
        flowMetDeadlinesPer[id].resize(MAX_FLOW_IDS);
        flowAccLatency[id].resize(MAX_FLOW_IDS,0);
        flowAvgLatency[id].resize(MAX_FLOW_IDS,0);
        flowIdealAvgLatency[id].resize(MAX_FLOW_IDS,0);
        flowAccLatencyMinusAvgLatency[id].resize(MAX_FLOW_IDS,0);
        flowStdevLatency[id].resize(MAX_FLOW_IDS,0);
        flowMinLatency[id].resize(MAX_FLOW_IDS,0);
        flowMaxLatency[id].resize(MAX_FLOW_IDS,0);
        flowLatencyRng[id].resize(MAX_FLOW_IDS);
        flowLatencyRngInc[id].resize(MAX_FLOW_IDS,0);
        flowLatencyRngPcks[id].resize(MAX_FLOW_IDS);

        for(unsigned int flowId = 0; flowId < MAX_FLOW_IDS; flowId++) {
            flowAccPckWithDeadline[id][flowId].resize(MAX_CLASSES,0);
            flowAccMissedDeadlines[id][flowId].resize(MAX_CLASSES,0);
            flowMissedDeadlinesPer[id][flowId].resize(MAX_CLASSES,0);
            flowMetDeadlinesPer[id][flowId].resize(MAX_CLASSES,0);
            flowLatencyRng[id][flowId].resize(100,0);
            flowLatencyRngPcks[id][flowId].resize(100,0);
        }
    }

    classAccNbOfPck.resize(MAX_CLASSES,0);
    classAccNbOfFlits.resize(MAX_CLASSES,0);
    classAccRequiredBw.resize(MAX_CLASSES,0);
    classAvgRequiredBw.resize(MAX_CLASSES,0);
    classAvgRequiredBwNorm.resize(MAX_CLASSES,0);
    classAcceptedTrafficFlits.resize(MAX_CLASSES,0);
    classAcceptedTrafficBps.resize(MAX_CLASSES,0);
    classAccPckWithDeadline.resize(MAX_CLASSES,0);
    classAccMissedDeadlines.resize(MAX_CLASSES,0);
    classMetDeadlinesPer.resize(MAX_CLASSES,0);
    classAccLatency.resize(MAX_CLASSES,0);
    classAvgLatency.resize(MAX_CLASSES,0);
    classIdealAvgLatency.resize(MAX_CLASSES,0);
    classAccLatencyMinusAvgLatency.resize(MAX_CLASSES,0);
    classStdevLatency.resize(MAX_CLASSES,0);
    classMinLatency.resize(MAX_CLASSES,0);
    classMaxLatency.resize(MAX_CLASSES,0);
    classLatencyRng.resize(MAX_CLASSES);
    classLatencyRngInc.resize(MAX_CLASSES,0);
    classLatencyRngPcks.resize(MAX_CLASSES);
    for(unsigned int cls = 0; cls < MAX_CLASSES; cls++) {
        classLatencyRng[cls].resize(100,0);
        classLatencyRngPcks[cls].resize(100,0);
    }

    latencyRng.resize(100,0);
    accPckWithDeadline.resize(MAX_CLASSES,0);
    accMissedDeadlines.resize(MAX_CLASSES,0);
    metDeadlinesPer.resize(MAX_CLASSES,0);
    latencyRngPcks.resize(100,0);;
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

    unsigned int i = 0; // Index of packets in log files
    unsigned int elementId = 0; // Index of TG element

    //////////////////////////////////////////////////////////////////////////////
    /// 1st SCAN: It determines the number of total packets received by TGs
    //////////////////////////////////////////////////////////////////////////////
    unsigned int totalNbOfPckRcvd = 0;
    for( elementId = 0; elementId < numberOfElements; elementId++ ) {
        totalNbOfPckRcvd += this->packets[elementId].size();
    }

    // If none packet was delivered, it returns with an error
    if( totalNbOfPckRcvd == 0 ) {
        return TrafficAnalysis::NoPacketsDelivered;
    }


    //////////////////////////////////////////////////////////////////////////////
    /// 2nd SCAN: It determines the average latency and the accepted traffic
    //////////////////////////////////////////////////////////////////////////////
    unsigned long int latency = 0;
    for( elementId = 0; elementId < numberOfElements; elementId++ ) {
        unsigned int nbOfPacketsReceived = (unsigned int) this->packets[elementId].size();
        for( i = 0; i < nbOfPacketsReceived; i++) {
            PacketInfo* pckInf = this->packets[elementId][i];
            unsigned int trafficClass = pckInf->getTrafficClass();

            if( (i+1 > ((unsigned long)(lower * nbOfPacketsReceived)))
                    && (i+1 <= ((unsigned long int)(upper * nbOfPacketsReceived))) )
            {
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
                    PacketInfo* pckInf_previous = this->packets[elementId][i-1];
                    endCycle = pckInf_previous->getCycleReceivedTrailer();
                } else {
                    endCycle = 0;
                }

                if( endCycle < smallestEndCycle ) {
                    smallestEndCycle = endCycle;
                }
            }


            // It get information to determine the time of simulation (in simulation cycles)
            if( i+1 > ((unsigned long int)(lower * nbOfPacketsReceived)) ) {
                startCycle = pckInf->getCycleCreationPacket();
                if( startCycle < smallestStartCycle ) {
                    smallestStartCycle = startCycle;
                }
            }

            if( i+1 <= ((unsigned long int)(upper * nbOfPacketsReceived)) ) {
                endCycle = pckInf->getCycleReceivedTrailer();
                if( endCycle > biggestEndCycle ) {
                    biggestEndCycle = endCycle;
                }
            }
        }
    }

    // It calculates the simulated time
    simulatedTimeCycles = biggestEndCycle - smallestEndCycle + 1;
    simulatedTimeNs = simulatedTimeCycles * (unsigned long) tClk;

    // It calculates the accepted traffic
    acceptedTrafficFlits = (float) accNbOfFlits/(float)numberOfElements/(float)simulatedTimeCycles;
    // Changed from
    //    sprintf(str,"%.4f",acceptedTrafficFlits);
    //    float tmpFloat = (float) atof(str);
    //    acceptedTrafficBps = (float) (tmpFloat*(dataWidth*fClk));
    // To
    acceptedTrafficBps = (float) (acceptedTrafficFlits*(dataWidth*fClk));

    // It calculates the average latency
    avgLatency    = ((float)accLatency) / (float) accNbOfPck;
    avgRequiredBw = accRequiredBw / (float) accNbOfPck;//

    // It do the same for the traffic classes
    for( unsigned int trafficClass = 0; trafficClass < MAX_CLASSES; trafficClass++ ) {
        // It calculates the accepted traffic for the current class
        classAcceptedTrafficFlits[trafficClass] = (float) classAccNbOfFlits[trafficClass]/(float)numberOfElements/(float)simulatedTimeCycles;
        // Changed from
        //        sprintf(str,"%.4f",classAcceptedTrafficFlits[trafficClass]);
        //        tmpFloat = (float) atof(str);
        //        classAcceptedTrafficBps[trafficClass] = (float) (tmpFloat*(dataWidth*fClk));
        // To
        classAcceptedTrafficBps[trafficClass] = (float) (classAcceptedTrafficFlits[trafficClass]*(dataWidth*fClk));

        // It calculates the average latency for the current class
        classAvgLatency[trafficClass]    = ((float) classAccLatency[trafficClass])  / (float) classAccNbOfPck[trafficClass];
        classAvgRequiredBw[trafficClass] = (float) classAccRequiredBw[trafficClass] / (float) classAccNbOfPck[trafficClass];
    }

    //////////////////////////////////////////////////////////////////////////////
    /// 3rd SCAN: It determines the standard deviation of the latency
    //////////////////////////////////////////////////////////////////////////////
    for( elementId = 0; elementId < numberOfElements; elementId++ ) {
        unsigned int nbOfPacketsReceived = (unsigned int) this->packets[elementId].size();
        for( i = 0; i < nbOfPacketsReceived; i++) {
            PacketInfo* pckInf = this->packets[elementId][i];
            if( (i+1 > ((unsigned long)(lower * nbOfPacketsReceived)))
                    && (i+1 <= ((unsigned long int)(upper * nbOfPacketsReceived))) ) {
                unsigned long int latency = pckInf->getLatency();
                accLatencyMinusAvgLatency += pow( ((float)latency - avgLatency),2 );
                classAccLatencyMinusAvgLatency[pckInf->getTrafficClass()] += pow( ((float)(latency - classAvgLatency[pckInf->getTrafficClass()])),2 );
            }
        }
    }

    // It calculates the standard deviation
    // Changed from
    //    stdevLatency = sqrt( accLatencyMinusAvgLatency / (float)accNbOfPck );
    // To - ver Dally and Towles 2004 - pag. 481
    stdevLatency = sqrt( accLatencyMinusAvgLatency / (float)(accNbOfPck-1) );
    for(unsigned int trafficClass = 0; trafficClass < MAX_CLASSES; trafficClass++) {
        if( classAccNbOfPck[trafficClass] != 0.0 ) {
            // Changed from
            //            classStdevLatency[trafficClass] = sqrt( classAccLatencyMinusAvgLatency[trafficClass]/(float)classAccNbOfPck[trafficClass] );
            // To
            classStdevLatency[trafficClass] = sqrt( classAccLatencyMinusAvgLatency[trafficClass]/(float)(classAccNbOfPck[trafficClass]-1) );
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

    std::vector<unsigned long> classAccIdealLatency;
    classAccIdealLatency.resize(MAX_CLASSES,0);
    for( elementId = 0; elementId < numberOfElements; elementId++ ) {
        unsigned int nbOfPacketsReceived = (unsigned int) this->packets[elementId].size();
        for( i = 0; i < nbOfPacketsReceived; i++) {
            PacketInfo* pckInf = this->packets[elementId][i];
            unsigned int trafficClass = pckInf->getTrafficClass();
            if ( (i+1 >  ((unsigned long int) (lower * nbOfPacketsReceived)))
                 && (i+1 <= ((unsigned long int) (upper * nbOfPacketsReceived))))
            {
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
                classAccIdealLatency[trafficClass] += idealLatency;

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

    idealAverageLatency = (float) idealAccLatency / (float) accNbOfPck;

    for( unsigned int i = 0; i < MAX_CLASSES; i++ ) {
        if( classAccNbOfPck[i] > 0 ) {
            classIdealAvgLatency[i] = (float) classAccIdealLatency[i] / (float) classAccNbOfPck[i];
        }
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

    unsigned int elementId, source,i;

    std::vector<std::vector<unsigned long> > flowAccIdealLatency;
    flowAccIdealLatency.resize(numberOfElements);
    for( elementId = 0; elementId < numberOfElements; elementId++ ) {
        flowAccIdealLatency[elementId].resize(MAX_FLOW_IDS,0);
    }


    for( elementId = 0; elementId < numberOfElements; elementId++ ) {
        //////////////////////////////////////////////////////////////////////////
        ///
        /// It initializes the variables for flow analysis
        ///
        ///////////////////////////////////////////////////////////////////////////
        // It initializes the variables used to detemine the metrics for individual flows
        for (source = 0; source < numberOfElements; source++) {
            for (unsigned int flowId = 0; flowId < MAX_FLOW_IDS; flowId++) {
                flowAccNbOfPck[source][flowId]        = 0;
                flowAccNbOfFlits[source][flowId]      = 0;
                flowAccRequiredBw[source][flowId]     = 0;
                flowAvgRequiredBwNorm[source][flowId] = 0;
                flowAvgRequiredBw[source][flowId]     = 0;

                flowAcceptedTrafficFlits[source][flowId] = 0;
                flowAcceptedTrafficBps[source][flowId]   = 0;

                for ( i= 0; i < MAX_CLASSES; i++ ) {
                    flowAccPckWithDeadline[source][flowId][i] = 0;
                    flowAccMissedDeadlines[source][flowId][i] = 0;
                    flowMissedDeadlinesPer[source][flowId][i] = 0;
                    flowMetDeadlinesPer[source][flowId][i]    = 0;
                }

                flowAccLatency[source][flowId] 				  = 0;
                flowAvgLatency[source][flowId] 				  = 0;
                flowIdealAvgLatency[source][flowId]           = 0;
                flowAccIdealLatency[source][flowId]      = 0;
                flowAccLatencyMinusAvgLatency[source][flowId] = 0;
                flowStdevLatency[source][flowId] 			  = 0;
                flowMinLatency[source][flowId] 				  = ULONG_MAX;
                flowMaxLatency[source][flowId] 				  = 0;
            }
        }
        //////////////////////////////////////////////////////////////////////////////
        ///
        /// FLOW's 1st SCAN: It determines the average latency and the accepted traffic
        ///
        //////////////////////////////////////////////////////////////////////////////
        unsigned int nbOfPacketsReceived = (unsigned int) this->packets[elementId].size();
        unsigned long int latency = 0;
        for( i = 0; i < nbOfPacketsReceived; i++ ) {
            PacketInfo* pckInf = this->packets[elementId][i];
            source = pckInf->getSource();
            unsigned int flowId = pckInf->getFlowId();
            unsigned int trafficClass = pckInf->getTrafficClass();
            // It determines the accumulated latency and the accumulated number of flits received
            if( (i+1 > ((unsigned long int)(lower*nbOfPacketsReceived)))
                    &&(i+1 <= ((unsigned long int)(upper*nbOfPacketsReceived))) ) {

                flowAccRequiredBw[source][flowId] += (unsigned long) pckInf->getRequiredBandwidth();

                // Packet latency in cycles
                latency = pckInf->getLatency();

                // Accumulated latency
                flowAccLatency[source][flowId] += latency;

                // Minimal latency
                if( latency < flowMinLatency[source][flowId] ) {
                    flowMinLatency[source][flowId] = latency;
                }

                // Maximum latency
                if(latency > flowMaxLatency[source][flowId]) {
                    flowMaxLatency[source][flowId] = latency;
                }

                // Missed deadlines
                if( pckInf->getDeadline() != 0 ) {
                    flowAccPckWithDeadline[source][flowId][trafficClass]++;
                    if( (latency * tClk) > pckInf->getDeadline() ) {
                        flowAccMissedDeadlines[source][flowId][trafficClass]++;
                    }
                }

                // Number of flits received
                flowAccNbOfFlits[source][flowId] += pckInf->getPacketSize();
                flowAccNbOfPck[source][flowId]++;
            }

            // It get information to determine the time of simulation (in simulation cycles)
            if( i+1 > ((unsigned long int) (lower * nbOfPacketsReceived)) ) {
                startCycle = pckInf->getCycleCreationPacket();
                if( startCycle < smallestStartCycle ) {
                    smallestStartCycle = startCycle;
                }
            }
            if( i+1 <= ((unsigned long int) (upper * nbOfPacketsReceived)) ) {
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
        // It calculates the simulated time

        // It calculates the target metrics of this scan
        simulatedTimeCycles = biggestEndCycle - smallestStartCycle + 1;
        simulatedTimeNs     = simulatedTimeCycles * (unsigned long) tClk;

        for(source = 0; source < numberOfElements; source++) {
            for(unsigned int flowId = 0; flowId < MAX_FLOW_IDS; flowId++) {
                if( flowAccNbOfPck[source][flowId] ) {
                    flowAvgRequiredBw[source][flowId] = ((float) flowAccRequiredBw[source][flowId])
                            / (float) (flowAccNbOfPck[source][flowId]);

                    if (flowAvgRequiredBw[source][flowId] <= channelBwAvailable) {
                        flowAvgRequiredBwNorm[source][flowId] = flowAvgRequiredBw[source][flowId]
                                / channelBwAvailable;
                    } else {
                        flowAvgRequiredBwNorm[source][flowId] = 1.0;
                    }

                    flowAvgLatency[source][flowId] = ((float) flowAccLatency[source][flowId])
                            / (float) (flowAccNbOfPck[source][flowId]);
                } else {
                    flowAvgRequiredBw[source][flowId]     = 0.0f;
                    flowAvgRequiredBwNorm[source][flowId] = 0.0f;
                    flowAvgLatency[source][flowId]        = 0.0f;
                } // if (flowAccNbOfPck[source][flowId])

                flowAcceptedTrafficFlits[source][flowId] = (float) flowAccNbOfFlits[source][flowId]
                        / (float) simulatedTimeCycles;


                flowAcceptedTrafficBps[source][flowId] = (float) flowAcceptedTrafficFlits[source][flowId] *
                        (float)dataWidth/(tClk/1000.f);
            } // for (flowId = 0; flowId < MAX_FLOW_IDS; flowId++)
        } //for (source=0; source<numberOfElements; source++)
        //////////////////////////////////////////////////////////////////////////////
        ///
        /// FLOW's 2nd SCAN: it determines the standard deviation of the latency
        ///
        //////////////////////////////////////////////////////////////////////////////
        // It opens the input file
        for( i = 0; i < nbOfPacketsReceived; i++ ) {
            PacketInfo* pckInf = this->packets[elementId][i];
            source = pckInf->getSource();
            unsigned int flowId = pckInf->getFlowId();
            if( (i+1 > ((unsigned long int)(lower*nbOfPacketsReceived)))
                    &&(i+1 <= ((unsigned long int)(upper*nbOfPacketsReceived))) ) {
                latency = pckInf->getLatency();
                flowAccLatencyMinusAvgLatency[source][flowId] += pow( (((float)latency) - flowAvgLatency[source][flowId]),2 );
            }
        }

        for(source = 0; source < numberOfElements; source++) {
            for( unsigned int flowId = 0; flowId < MAX_FLOW_IDS; flowId++ ) {
                // If requiredBw is greather than 0, it calculates the standard deviation, if not, it sets it to 0
                if(flowAvgRequiredBw[source][flowId]) {
                    flowStdevLatency[source][flowId] = sqrt(flowAccLatencyMinusAvgLatency[source][flowId] /
                                                            (float)(flowAccNbOfPck[source][flowId] - 1));
                } else {
                    flowStdevLatency[source][flowId] = 0.0f;
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
        for(source = 0; source < numberOfElements; source++) {
            for(unsigned int flowId = 0; flowId < MAX_FLOW_IDS; flowId++) {
                flowLatencyRngInc[source][flowId] = (unsigned long int) roundf(((float)flowMaxLatency[source][flowId] -
                                                                                (float)flowMinLatency[source][flowId])/100.0f);

                if(flowLatencyRngInc[source][flowId] < 1) {
                    flowLatencyRngInc[source][flowId] = 1;
                }

                flowLatencyRng[source][flowId][0] = flowMinLatency[source][flowId];
                flowLatencyRngPcks[source][flowId][0] = 0;

                for ( i = 1; i < 100; i++) {
                    flowLatencyRng[source][flowId][i] = flowLatencyRng[source][flowId][i-1] +
                            flowLatencyRngInc[source][flowId];
                    flowLatencyRngPcks[source][flowId][i] = 0;
                }
            }
        }

        for( i = 0; i < IDEAL_LATENCY_RNG_SIZE; i++ ) {
            idealLatencyRng[i] = 0;
        }

        idealAccLatency = 0;
        for( unsigned int pckIndex = 0; pckIndex < nbOfPacketsReceived; pckIndex++ ) {
            PacketInfo* pckInf = this->packets[elementId][pckIndex];
            source = pckInf->getSource();
            // It updates the latency counters
            if( (pckIndex+1 > ((unsigned long int) (lower * nbOfPacketsReceived)))
                    &&(pckIndex+1 <= ((unsigned long int) (upper * nbOfPacketsReceived))) ) {

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
                        if( (latency >= flowLatencyRng[source][flowId][i])
                                &&(latency < flowLatencyRng[source][flowId][i+1]) ) {

                            flowLatencyRngPcks[source][flowId][i]++;
                        }
                    }
                }

                unsigned int nbOfRouters = pckInf->getNumberOfRoutersInPath();
                unsigned int nbOfLinks   = pckInf->getNumberOfLinksInPath();

                // It determines the ideal latency for the flow
                idealLatency = this->calculateIdealLatency(nbOfLinks,nbOfRouters,pckInf->getPacketSize());
                idealAccLatency += idealLatency;

                flowAccIdealLatency[source][pckInf->getFlowId()] += idealLatency;

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

        // It determines the flow's missed deadlines and ideal flow latency
        for (source = 0; source < numberOfElements; source++) {
            for (unsigned int flowId = 0; flowId < MAX_FLOW_IDS; flowId++) {
                if( flowAccNbOfPck[source][flowId] > 0 ) {
                    flowIdealAvgLatency[source][flowId] = (float) flowAccIdealLatency[source][flowId] /
                            (float) flowAccNbOfPck[source][flowId];
                }


                for (i = 0; i < MAX_CLASSES; i++){
                    if (flowAccPckWithDeadline[source][flowId][i] > 0) {
                        flowMissedDeadlinesPer[source][flowId][i] =
                                ( ((float)flowAccMissedDeadlines[source][flowId][i]*100.0f)/
                                  (float) flowAccPckWithDeadline[source][flowId][i] );
                        flowMetDeadlinesPer[source][flowId][i] = 100 -
                                ( ((float) flowAccMissedDeadlines[source][flowId][i]*100.0f)/
                                  (float) flowAccPckWithDeadline[source][flowId][i] );
                    } else {
                        flowMissedDeadlinesPer[source][flowId][i] = -1.0f;
                        flowMetDeadlinesPer[source][flowId][i] = -1.0f;
                    }
                }
            }
        }

        TrafficAnalysis::StatusAnalysis status = this->createReportsForIndividualFlows(elementId);
        if( status != TrafficAnalysis::Ok ) {
            return status;
        }
    } // for(elementId=0;elementId<numberOfElements;elementId++)

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
                avgRequiredBw,          	// bps
                acceptedTrafficBps,     	// bps
                idealAverageLatency * tClk, // ns
                avgLatency * tClk,          // ns
                (float)minLatency * tClk,   // ns
                (float)maxLatency * tClk,   // ns
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
                        classIdealAvgLatency[trafficClass],            // Must be tested
                        classAvgLatency[trafficClass],
                        classStdevLatency[trafficClass],
                        (float) classMinLatency[trafficClass],
                        (float) classMaxLatency[trafficClass],
                        classAvgRequiredBw[trafficClass],       	// bps
                        classAcceptedTrafficBps[trafficClass],		// bps
                        classIdealAvgLatency[trafficClass] * tClk,     // ns
                        classAvgLatency[trafficClass] * tClk,       // ns
                        (float)classMinLatency[trafficClass] * tClk,// ns
                        (float)classMaxLatency[trafficClass] * tClk,// ns
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

TrafficAnalysis::StatusAnalysis PerformanceAnalysis::createReportsForIndividualFlows(unsigned int destination) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/PerformanceAnalysis::createReportsForIndividualFlows" << std::endl;
#endif

    FILE* fpOut;

    unsigned int source;

    // It creates the flow report
    for( source = 0; source < numberOfElements; source++ ) {
        for( unsigned int flowId = 0; flowId < MAX_FLOW_IDS; flowId++ ) {
            if( flowAccNbOfPck[source][flowId] > 0 ) {
                sprintf(str,"%s/flow_%u_%u_%u",resultDir,source,destination,flowId);
                if( (fpOut=fopen(str,"a+t")) == NULL ) {
                    printf("ERROR: Impossible to create file %s",str);
                    return TrafficAnalysis::NoOutputFile;
                } else {
                    sprintf(str,"%.1f\t%lu\t%.4f\t%.4f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f",
                            fClk,
                            flowAccNbOfPck[source][flowId],
                            flowAvgRequiredBwNorm[source][flowId],
                            flowAcceptedTrafficFlits[source][flowId],
                            flowIdealAvgLatency[source][flowId],    // Must be tested
                            flowAvgLatency[source][flowId],
                            flowStdevLatency[source][flowId],
                            (float) flowMinLatency[source][flowId],
                            (float) flowMaxLatency[source][flowId],

                            flowAvgRequiredBw[source][flowId],
                            flowAcceptedTrafficBps[source][flowId],
                            flowIdealAvgLatency[source][flowId] * tClk,//(*ret_ideal_avg_latency ) * tclk, // ns // Must be tested
                            flowAvgLatency[source][flowId] * tClk,

                            (float) flowMinLatency[source][flowId] * tClk,
                            (float) flowMaxLatency[source][flowId] * tClk,

                            flowMetDeadlinesPer[source][flowId][0],
                            flowMetDeadlinesPer[source][flowId][1],
                            flowMetDeadlinesPer[source][flowId][2],
                            flowMetDeadlinesPer[source][flowId][3]
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

    // It creates latency histogram report
    for( source = 0; source < numberOfElements; source++ ) {
        for( unsigned int flowId = 0; flowId < MAX_FLOW_IDS; flowId++ ) {
            if( flowAccNbOfPck[source][flowId] > 0 ) {
                sprintf(str,"%s/Results/flow_%u_%u_%u_latency_histogram",workDir,source,destination,flowId);
                if( (fpOut=fopen(str,"wt")) == NULL ) {
                    printf("ERROR: Impossible to create file %s",str);
                    return TrafficAnalysis::NoOutputFile;
                } else {
                    for( unsigned int i = 0; i < 100; i++ ) {
                        if( flowLatencyRngPcks[source][flowId][i] != 0 ) {
                            float tmpFloat = 100.0f * ((float) flowLatencyRngPcks[source][flowId][i]
                                                       / (float) flowAccNbOfPck[source][flowId] );
                            sprintf(str,"%lu\t%lu\t%.1f",flowLatencyRng[source][flowId][i],
                                    flowLatencyRngPcks[source][flowId][i],tmpFloat);
                            fprintf(fpOut,"%s\n",str);
                        }
                    }
                    fclose(fpOut);
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
}
