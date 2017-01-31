#include "NewTrafficAnalyzer.h"
#include "Model/Analysis/PacketInfo.h"

#include <cstdio>
#include <cstdlib>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

#include <QDebug>

NewTrafficAnalyzer::NewTrafficAnalyzer(unsigned short numElements,
                                       unsigned short dataWidth,
                                       float lower, float upper,
                                       float fClk,
                                       unsigned short fifoOutDepth,
                                       unsigned short flowControlType,
                                       const char *workDir,
                                       const char *resultDir)
    : TrafficAnalysis(numElements,dataWidth,lower,upper,fClk,fifoOutDepth,flowControlType,workDir,resultDir),
      simulatedCycles(0)
{
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor class Model/Analysis/NewTrafficAnalyzer" << std::endl;
#endif
}

NewTrafficAnalyzer::~NewTrafficAnalyzer() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor class Model/Analysis/NewTrafficAnalyzer" << std::endl;
#endif
}

TrafficAnalysis::StatusAnalysis NewTrafficAnalyzer::makeAnalysis() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/NewTrafficAnalyzer::makeAnalysis" << std::endl;
#endif

    // Read simulation logs
    StatusAnalysis status = this->readLogsFiles();
    if( status != TrafficAnalysis::Ok ) {
        return status;
    }

    return this->batchMethod();
}

TrafficAnalysis::StatusAnalysis NewTrafficAnalyzer::readLogsFiles() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/NewTrafficAnalyzer::readLogsFiles" << std::endl;
#endif

    char str[2048];

    FILE* fpIn;

    sprintf(str,"%s/stopsim.out",workDir);
    if( (fpIn = fopen(str,"rt")) == NULL ) {
        return TrafficAnalysis::NoInputFile;
    }
    fscanf(fpIn,"%s",str);
    fclose(fpIn);

    this->simulatedCycles = atoll(str);

    return TrafficAnalysis::readLogsFiles();
}

TrafficAnalysis::StatusAnalysis NewTrafficAnalyzer::batchMethod() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/NewTrafficAnalyzer::batchMethod" << std::endl;
#endif

    qDebug() << QString("Simulated time: %1 cycles @ %2")
                .arg(simulatedCycles)
                .arg(fClk);

    float startCycle = floor(simulatedCycles * lower);
    float endCycle = ceil(simulatedCycles * upper);

    qDebug() << QString("Start Cycle: %1 - End Cycle: %2")
                .arg(startCycle)
                .arg(endCycle);

    std::vector<PacketInfo *> packetsOnInterval;
    unsigned long long totalPackets = 0;
    for( unsigned int elementId = 0; elementId < numberOfElements; elementId++ ) {
        std::vector<PacketInfo *> pcks = this->packets[elementId];
        totalPackets += pcks.size();
        for( unsigned int pckIndex = 0; pckIndex < pcks.size(); pckIndex++ ) {
            PacketInfo* pckInfo = pcks[pckIndex];
            if( pckInfo->getCycleCreationPacket() >= startCycle &&
                    pckInfo->getCycleCreationPacket() < endCycle ) {
                packetsOnInterval.push_back(pckInfo);
            }
        }
    }

    qDebug() << QString("Total Packets in the simulation %1 - Packets Analyzed on the interval: %2")
                .arg(totalPackets)
                .arg(packetsOnInterval.size());

    float avgLatency = 0;
    unsigned int accNumFlits = 0;
    for( unsigned int pckIndex = 0; pckIndex < packetsOnInterval.size(); pckIndex++ ) {
        PacketInfo* pckInfo = packetsOnInterval.at(pckIndex);
        avgLatency += pckInfo->getLatency();
        accNumFlits += pckInfo->getPacketSize();
    }

    avgLatency /= (float) packetsOnInterval.size();
    float acceptedTraffic = (float) accNumFlits / (float) numberOfElements / (float) (endCycle - startCycle);

    qDebug() << QString("Avgerage Network Latency: %1 - Accepted Traffic: %2")
                .arg(avgLatency)
                .arg(acceptedTraffic);

    return StatusAnalysis::Ok;
}
