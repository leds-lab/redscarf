#ifndef __INTERNALANALYSIS_H__
#define __INTERNALANALYSIS_H__

#include "TrafficAnalysis.h"
#include "PacketInfo.h"
#include "Model/System/Topology.h"

class InternalAnalysis : public TrafficAnalysis {
private:

    /*!
     * \brief initSystemVariables Initializes attributes with default values
     */
    void initSystemVariables();
    Topology* topology;
    std::vector<std::vector<std::vector<PacketInfo* > > > internalPackets;
    std::vector<std::vector<float> > linkAvCpF;
    std::vector<std::vector<std::vector<float> > > acceptedTraffic;
    std::vector<std::vector<float> > linkAvgBandwidth;
    std::vector<std::vector<float> > linkThroughput;

protected:
    /*!
     * \brief analyzeAllFlows Run performance analysis for all flows
     * \return The status from analysis
     */
    StatusAnalysis analyzeAllLinks();
    /*!
     * \brief createReportsForAllFlows Write report files for all flows
     * \return The status from write reports
     */
    StatusAnalysis createReportsForAllLinks();
    /*!
     * \brief analyzeIndividualFlows Run performance analysis for individuals flows
     * \return The status from analysis
     */
    StatusAnalysis analyzeIndividualFlows();
    /*!
     * \brief createReportsForIndividualFlows Write report files for individual flow
     * \param destionation Destination address of the flow
     * \return The status from write reports
     */
    StatusAnalysis createReportsForIndividualFlows(unsigned int destination);

public:
    InternalAnalysis() : TrafficAnalysis(1,2,3,4,5,6,7,NULL,NULL){}
    /*!
     * \brief InternalAnalysis Constructor with parameters to be set
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
    InternalAnalysis(unsigned short numElements, unsigned short dataWidth,
                     float lower, float upper, float fClk,
                     unsigned short fifoOutDepth,
                     unsigned short flowControlType,
                     const char* workDir, const char* resultDir,
                     Topology *topology);

    /*!
     * \brief makeAnalysis Run all analysis
     * \return End status from performance analysis
     */
    StatusAnalysis makeAnalysis();

    /*!
     * \brief ~InternalAnalysis Destructor to be free memory
     */
     ~InternalAnalysis();

};

#endif // __INTERNALANALYSIS_H__
