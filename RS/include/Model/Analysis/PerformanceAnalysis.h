/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* PerformanceAnalysis.h
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
* 01/12/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*
*/

#ifndef __PERFORMANCEANALYSIS_H__
#define __PERFORMANCEANALYSIS_H__

#include "TrafficAnalysis.h"
#include "PacketInfo.h"
#include <vector>

/*!
 * \brief The PerformanceAnalysis class read log files
 * and generate report files.
 *
 * The main function of this class is generate performance
 * analysis results of packets simulated on NoC (Network-on-Chip).
 * The read is run only one time.
 * The latency and throughput are performed for all and individual flows
 * and reports will be created.
 *
 * The calculations performed by this class are:
 *   * Average latency;
 *   * Accepted Traffic;
 *   * Standard Deviation of the latency;
 *   * Frequency of latencies; &
 *   * Ideal average latency.
 *
 * The reports generated by this class are:
 *   * Summary report, with the analysis of all flows
 *   * Summary latency histogram, with latency histogram of all flows
 *   * Summary report per traffic class of all flows
 *   * Summary latency histogram per traffic class of all flows
 *   * Report per individual flow
 *   * Latency histogram per individual flow
 */
class PerformanceAnalysis : public TrafficAnalysis {
private:

    static const int MAX_CLASSES = 8;
    static const int IDEAL_LATENCY_RNG_SIZE = 1002;
    static const int MAX_FLOW_IDS = 4;

    std::vector<std::vector<PacketInfo* > > packets;

    unsigned int tRA;

    char  str[2048];                            //! Auxiliary string
    float idealAverageLatency;                  //! Ideal average latency (with none contention)

    unsigned int nbOfCyclesPerRouter;           //! Minimum number of cycles spent by a header in a router
    unsigned int nbOfCyclesPerFlit;             //! Numb. of cycles spent to send a flit (it depends on the flow control tech.)

    std::vector<unsigned long> accPckWithDeadline;	//! Counter of packes with deadline requirements
    std::vector<unsigned long> accMissedDeadlines;	//! Accumulated missed deadlines
    std::vector<float>         metDeadlinesPer;     //! Met deadlines

    unsigned long int accLatency;                   //! Sum of the latencies of all the packets
    float 			  avgLatency;                   //! Average of all packet latencies
    float             stdevLatency;                 //! Standard deviation of latency
    unsigned long int minLatency;                   //! Smallest latency
    unsigned long int maxLatency;                   //! Greatest latency
    unsigned long int idealLatency;                 //! Ideal latency (it does not take into account the network congestion)
    unsigned long int idealAccLatency;              //! Average of all ideal packet latencies
    float             accLatencyMinusAvgLatency;    //! Diference beetwen the latency of a packet and the average latency

    // The following variables are used to identify the frequency of latencys in a scale of 100 intervals of
    // latency from the smallest latency to the greatest one, in intervals of (max_latency-min_latency)/100 cycles
    std::vector<unsigned long> latencyRng;	//! Array of latency intervals
    std::vector<unsigned long> latencyRngPcks;	//! Number of packets of latency in each interval
    unsigned long int latencyRngInc;		//! Size of each interval
    unsigned long int idealLatencyIndex;  	//! Index to access each latency interval
    unsigned long int idealLatencyRng[IDEAL_LATENCY_RNG_SIZE]; //! Array with the amount of latency that are X% greater than the ideal latency

    // Variables to collect data for perfomance analysis of the NoC
    float             acceptedTrafficFlits;	//! Traffic accepted by the network
    float             acceptedTrafficBps;	//! Traffic accepted by the network
    float             accRequiredBw;		//! Accumulated bandwidth required by all the packets
    float             avgRequiredBw;		//! Avergage bandwidth required by all the packets
    unsigned long int accNbOfPck;           //! Sum of all the number of packets
    unsigned long int simulatedTimeCycles;  //! Total simulation time in cycles
    unsigned long int simulatedTimeNs;      //! Total simulation time in ns
    unsigned long int accNbOfFlits;         //! Accumulated number of flits
    unsigned long int startCycle;			//! First cycle of a packet
    unsigned long int endCycle;             //! Last  cycle of a packet
    unsigned long int smallestStartCycle;   //! First cycle of simulation (considering only the packets to be analized) - Alternative 1: when there is no previous processing
    unsigned long int smallestEndCycle;     //! First cycle of simulation (considering only the packets to be analized) - Alternative 2: when a packet is created after a processing
    unsigned long int biggestEndCycle;      //! Last  cycle of simulation (considering only the packets to be analized)

    // Variables to collect data for perfomance analysis of individual flows
    std::vector<std::vector<unsigned long> > flowAccNbOfPck;
    std::vector<std::vector<unsigned long> > flowAccNbOfFlits;
    std::vector<std::vector<unsigned long> > flowAccRequiredBw;
    std::vector<std::vector<float> >         flowAvgRequiredBw;
    std::vector<std::vector<float> >         flowAvgRequiredBwNorm;
    std::vector<std::vector<float> >         flowAcceptedTrafficFlits;
    std::vector<std::vector<float> >         flowAcceptedTrafficBps;
    std::vector<std::vector<std::vector<unsigned long> > > flowAccPckWithDeadline;
    std::vector<std::vector<std::vector<unsigned long> > > flowAccMissedDeadlines;
    std::vector<std::vector<std::vector<float> > >         flowMissedDeadlinesPer;
    std::vector<std::vector<std::vector<float> > >         flowMetDeadlinesPer;
    std::vector<std::vector<unsigned long> >               flowAccLatency;
    std::vector<std::vector<float> >                       flowAvgLatency;
    std::vector<std::vector<float> >                       flowIdealAvgLatency;
    std::vector<std::vector<float> >                       flowAccLatencyMinusAvgLatency;
    std::vector<std::vector<float> >                       flowStdevLatency;
    std::vector<std::vector<unsigned long> >               flowMinLatency;                //! Minimal flow latency
    std::vector<std::vector<unsigned long> >               flowMaxLatency;                //! Maximal flow latency
    std::vector<std::vector<std::vector<unsigned long> > > flowLatencyRng;                //! Array of latency intervals
    std::vector<std::vector<unsigned long> >               flowLatencyRngInc;             //! Array of latency intervals
    std::vector<std::vector<std::vector<unsigned long> > > flowLatencyRngPcks;            //! Number of packets of latency

    // Variables to collect data for perfomance analysis of each class
    std::vector<unsigned long> classAccNbOfPck;
    std::vector<unsigned long> classAccNbOfFlits;
    std::vector<unsigned long> classAccRequiredBw;
    std::vector<float>         classAvgRequiredBw;
    std::vector<float>         classAvgRequiredBwNorm;
    std::vector<float>         classAcceptedTrafficFlits;
    std::vector<float>         classAcceptedTrafficBps;
    std::vector<unsigned long> classAccPckWithDeadline;
    std::vector<unsigned long> classAccMissedDeadlines;
    std::vector<float>         classMetDeadlinesPer;
    std::vector<unsigned long> classAccLatency;
    std::vector<float>         classAvgLatency;
    std::vector<float>         classIdealAvgLatency;
    std::vector<float>         classAccLatencyMinusAvgLatency;
    std::vector<float>         classStdevLatency;
    std::vector<unsigned long> classMinLatency;                   //! Minimal flow latency
    std::vector<unsigned long> classMaxLatency;                   //! Maximal flow latency
    std::vector<std::vector<unsigned long> > classLatencyRng;     //! Array of latency intervals
    std::vector<unsigned long>               classLatencyRngInc;  //! Array of latency intervals
    std::vector<std::vector<unsigned long> > classLatencyRngPcks; //! Number of packets of latency

    float channelBwAvailable;	//! It takes into account the used flow control (= channel_bw/nb_of_cycles_per_flit)

    /*!
     * \brief initSystemVariables Initializes attributes with default values
     */
    void initSystemVariables();

protected:
    /*!
     * \brief readLogsFiles Read the log files generated by simulations
     * and store objects for will be manipulated.
     * \return The status from read files
     */
    virtual StatusAnalysis readLogsFiles();
    /*!
     * \brief analyzeAllFlows Run performance analysis for all flows
     * \return The status from analysis
     */
    virtual StatusAnalysis analyzeAllFlows();
    /*!
     * \brief createReportsForAllFlows Write report files for all flows
     * \return The status from write reports
     */
    virtual StatusAnalysis createReportsForAllFlows();
    /*!
     * \brief analyzeIndividualFlows Run performance analysis for individuals flows
     * \return The status from analysis
     */
    virtual StatusAnalysis analyzeIndividualFlows();
    /*!
     * \brief createReportsForIndividualFlows Write report files for individual flow
     * \param destionation Destination address of the flow
     * \return The status from write reports
     */
    virtual StatusAnalysis createReportsForIndividualFlows(unsigned int destination);

    /*!
     * \brief calculateIdealLatency Calculate ideal latency in relation to the
     * number of links, routers and payload of packet
     * \param nLinks Number of links in path
     * \param nRouters Number of routers in path
     * \param payload Payload of packet
     * \return Ideal latency of packet
     */
    virtual unsigned long int calculateIdealLatency(unsigned int nLinks,
                                            unsigned int nRouters,
                                            unsigned int payload);


public:
    /*!
     * \brief PerformanceAnalysis Constructor with parameters to be set
     * attributes and allocate memory for calculations.
     * \param numElements Number of elements in the system
     * \param dataWidth Channel data width
     * \param lower Initial percentual of packets discarded in analysis
     * \param upper End percentual of packets discarded in analysis
     * \param fClk Frequency operation (MHz) to be analyzed
     * \param fifoOutDepth Fifo out depth
     * \param flowControlType Flow control type
     * \param workDir Read log files folder for analysis
     * \param resultDir Write report files folder of analysis
     */
    PerformanceAnalysis(unsigned short numElements,unsigned short dataWidth,
                        float lower,float upper,float fClk,
                        unsigned short fifoOutDepth,
                        unsigned short flowControlType,
                        const char* workDir, const char* resultDir);

    /*!
     * \brief makeAnalysis Run all analysis
     * \return End status from performance analysis
     */
    StatusAnalysis makeAnalysis();

    /*!
     * \brief ~PerformanceAnalysis Destructor to be free memory
     */
    virtual ~PerformanceAnalysis();

};

#endif // __PERFORMANCEANALYSIS_H__
