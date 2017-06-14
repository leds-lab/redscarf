#include "Model/Analysis/InternalAnalysis.h"

#include <cstdio>
#include <math.h>
#include <limits.h>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

InternalAnalysis::InternalAnalysis(unsigned short numElements, unsigned short dataWidth,
                                   float lower, float upper, float fClk,
                                   unsigned short fifoOutDepth,
                                   unsigned short flowControlType,
                                   const char* workDir, const char* resultDir,
                                   Topology* topology) :
    TrafficAnalysis(numElements,dataWidth,lower,upper,fClk,fifoOutDepth,flowControlType,workDir,resultDir)
{
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/Analysis/InternalAnalysis" << std::endl;
#endif
    this->topology = topology;
}

TrafficAnalysis::StatusAnalysis InternalAnalysis::makeAnalysis() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/InternalAnalysis::makeAnalysis" << std::endl;
#endif

    StatusAnalysis status;

    unsigned short xSize = topology->sp.getXSize();
    unsigned short ySize = topology->sp.getYSize();
    unsigned short zSize = topology->sp.getZSize();
    unsigned short numX;
    unsigned short numY;
    unsigned short numZ;

    //Read simulation logs
    switch(topology->getTopologyType()){
        case SystemParameters::Orthogonal3D:
            numX = (xSize-1) * ySize * zSize;
            status = this->readLogsFiles("%s/int_x_n_%u", numX);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            status = this->readLogsFiles("%s/int_x_s_%u", numX);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            numY = (ySize-1) * xSize * zSize;
            status = this->readLogsFiles("%s/int_y_e_%u", numY);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            status = this->readLogsFiles("%s/int_y_w_%u", numY);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            numZ = (zSize-1) * xSize * ySize;
            status = this->readLogsFiles("%s/int_z_u_%u", numZ);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            status = this->readLogsFiles("%s/int_z_d_%u", numZ);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            break;
        case SystemParameters::Orthogonal2D:
            if( topology->getTopology() == Topology::Mesh2D ){
                numX = (xSize-1) * ySize;
                numY = (ySize-1) * xSize;
            }else if( topology->getTopology() == Topology::Torus2D){
                numX = xSize * ySize;
                numY = numX;
            }
            status = this->readLogsFiles("%s/int_x_n_%u", numX);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            status = this->readLogsFiles("%s/int_x_s_%u", numX);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            status = this->readLogsFiles("%s/int_y_e_%u", numY);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            status = this->readLogsFiles("%s/int_y_w_%u", numY);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            break;
        default:
            status = this->readLogsFiles("%s/int_cw_%u", numberOfElements);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            status = this->readLogsFiles("%s/int_ccw_%u", numberOfElements);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            if( topology->getTopology() == Topology::ChordalRing ){
                status = this->readLogsFiles("%s/int_a_%u", numberOfElements);
                if( status != TrafficAnalysis::Ok ) {
                    return status;
                }
                internalPackets.push_back(packets);
                packets.clear();
            }
    }

    // Analyze all links
    status = this->analyzeAllLinks();
    if( status != TrafficAnalysis::Ok ) {
        return status;
    }

    // Creates the reports for all linkss
    status = this->createReportsForAllLinks();
    if( status != TrafficAnalysis::Ok ) {
        return status;
    }

}

void InternalAnalysis::initSystemVariables() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/InternalAnalysis::initSystemVariables" << std::endl;
#endif


}

TrafficAnalysis::StatusAnalysis InternalAnalysis::analyzeAllLinks() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/InternalAnalysis::analyzeAllLinks" << std::endl;
#endif

    for(unsigned int dim = 0; dim < internalPackets.size(); dim++){
        for(unsigned int link = 0; link < internalPackets[dim].size(); link++){

            PacketInfo* aux;

            float avcpf = 0;
            float abw = 0;
            float thr = 0;
            aux = internalPackets[dim][link][0];
            unsigned long long tsfint = aux->getCycleReceivedHeader();
            aux = internalPackets[dim][link][internalPackets[dim][link].size()-1];
            unsigned long long tslint = aux->getCycleReceivedTrailer();

            for(unsigned int p = 0; p < internalPackets[dim][link].size(); p++){

                PacketInfo* packet = internalPackets[dim][link][p];

                unsigned long long tpfint = packet->getCycleReceivedHeader();
                unsigned long long tplint = packet->getCycleReceivedTrailer();
                unsigned int pcksize = packet->getPacketSize();

                //Cycles per flit (part 1)
                avcpf += (tplint - tpfint)/(float) pcksize;

                //Accepted traffic
                if(p < internalPackets[dim][link].size() - 1){
                    PacketInfo* nextPacket = internalPackets[dim][link][p+1];
                    unsigned long long nexttpfint = nextPacket->getCycleReceivedHeader();
                    this->acceptedTraffic[dim][link][p] = pcksize/(float) (nexttpfint - tpfint);
                }

                //Average bandwidth (part 1)
                abw += tplint - tpfint;

                //Throughput (part 1)
                thr += pcksize;
                //thrbps?

            }
            //Cycles per flit (part 2)
            this->linkAvCpF[dim][link] = avcpf;
            //Average bandwidth (part 2)
            this->linkAvgBandwidth[dim][link] = abw/(tslint - tsfint);
            //Throughput (part 2)
            unsigned long long nsimc = tslint - tsfint; //TODO: check nsimc
            unsigned int flitsize = aux->getPacketSize()*1; //TODO: check flit size
            this->linkThroughput[dim][link] = thr*flitsize / nsimc;
        }
    }

    return TrafficAnalysis::Ok;
}

TrafficAnalysis::StatusAnalysis InternalAnalysis::createReportsForAllLinks() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/InternalAnalysis::createReportsForIndividualFlows" << std::endl;
#endif

    FILE* fpOut;
    char str[512];

    //TODO: header

    for(unsigned int dim = 0; dim < internalPackets.size(); dim++){
        for(unsigned int link = 0; link < internalPackets[dim].size(); link++){
            if(internalPackets[dim][link].size() > 0){
                sprintf(str,"%s/link_%u_%u",resultDir,dim,link);
                if( (fpOut=fopen(str,"a+t")) == NULL ) {
                    printf("ERROR: Impossible to create file %s",str);
                    return TrafficAnalysis::NoOutputFile;
                } else {
                    sprintf(str,"%.1f\t%.1f\t%.1f",
                            linkAvCpF[dim][link],
                            linkAvgBandwidth[dim][link],
                            linkThroughput[dim][link]);
                    //TODO: check where to use individual acceptedTraffic for individual pkts
                    // Replace ',' by '.'
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

    return TrafficAnalysis::Ok;
}

InternalAnalysis::~InternalAnalysis() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Model/Analysis/InternalAnalysis" << std::endl;
#endif
}