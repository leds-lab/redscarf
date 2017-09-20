#include "Model/Analysis/InternalAnalysis.h"
#include "Model/System/Defines.h"

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
            packets.resize(numX);
            status = this->readLogsFiles("%s/int_x_e_%u", numX);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            packets.resize(numX);
            status = this->readLogsFiles("%s/int_x_w_%u", numX);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            numY = (ySize-1) * xSize * zSize;
            packets.resize(numY);
            status = this->readLogsFiles("%s/int_y_n_%u", numY);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            packets.resize(numY);
            status = this->readLogsFiles("%s/int_y_s_%u", numY);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            numZ = (zSize-1) * xSize * ySize;
            packets.resize(numZ);
            status = this->readLogsFiles("%s/int_z_u_%u", numZ);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            packets.resize(numZ);
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
            packets.resize(numX);
            status = this->readLogsFiles("%s/int_x_e_%u", numX);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            packets.resize(numX);
            status = this->readLogsFiles("%s/int_x_w_%u", numX);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            packets.resize(numY);
            status = this->readLogsFiles("%s/int_y_n_%u", numY);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            packets.resize(numY);
            status = this->readLogsFiles("%s/int_y_s_%u", numY);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            break;
        default:
            packets.resize(numberOfElements);
            status = this->readLogsFiles("%s/int_cw_%u", numberOfElements);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            packets.resize(numberOfElements);
            status = this->readLogsFiles("%s/int_ccw_%u", numberOfElements);
            if( status != TrafficAnalysis::Ok ) {
                return status;
            }
            internalPackets.push_back(packets);
            packets.clear();
            if( topology->getTopology() == Topology::ChordalRing ){
                packets.resize(numberOfElements);
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

    return TrafficAnalysis::Ok;
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

    this->acceptedTraffic.resize(internalPackets.size());
    this->linkAvCpF.resize(internalPackets.size());
    this->linkAvgBandwidth.resize(internalPackets.size());
    this->linkThroughput.resize(internalPackets.size());

    for(unsigned int dim = 0; dim < internalPackets.size(); dim++){
        this->acceptedTraffic[dim].resize(internalPackets[dim].size());
        this->linkAvCpF[dim].resize(internalPackets[dim].size());
        this->linkAvgBandwidth[dim].resize(internalPackets[dim].size());
        this->linkThroughput[dim].resize(internalPackets[dim].size());

        for(unsigned int link = 0; link < internalPackets[dim].size(); link++){
            PacketInfo* aux;

            float avcpf = 0;
            float abw = 0;
            float thr = 0;
            if(internalPackets[dim][link].size() > 0){
                aux = internalPackets[dim][link][0];
                unsigned long long tsfint = aux->getCycleReceivedHeader();
                aux = internalPackets[dim][link][internalPackets[dim][link].size()-1];
                unsigned long long tslint = aux->getCycleReceivedTrailer();

                this->acceptedTraffic[dim][link].resize(internalPackets[dim][link].size());

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
                this->linkAvCpF[dim][link] = avcpf/internalPackets[dim][link].size();
                //Average bandwidth (part 2)
                this->linkAvgBandwidth[dim][link] = abw/(tslint - tsfint);
                //Throughput (part 2)
                unsigned long long nsimc = tslint - tsfint; //TODO: check nsimc
                unsigned int flitsize = aux->getPacketSize()*1; //TODO: check flit size
                this->linkThroughput[dim][link] = thr*flitsize / nsimc;
            }
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

    //TODO: verify other topologies (this was written for mesh)
    for(unsigned int dim = 0; dim < internalPackets.size(); dim++){
        for(unsigned int link = 0; link < internalPackets[dim].size(); link++){
            if(internalPackets[dim][link].size() > 0){
                sprintf(str,"%s/link_%u_%u",resultDir,dim,link);
                if( (fpOut=fopen(str,"a+t")) == NULL ) {
                    printf("ERROR: Impossible to create file %s",str);
                    return TrafficAnalysis::NoOutputFile;
                } else {
                    sprintf(str,"%.1f\t%.5f\t%.5f",
                            linkAvCpF[dim][link],
                            linkAvgBandwidth[dim][link],
                            linkThroughput[dim][link]);
                    // Replace ',' by '.'
                    unsigned int i = 0;
                    do {
                        if(str[i] == ',') str[i] = '.';
                        i++;
                    }while(str[i] != '\0');
                    fprintf(fpOut,"%s\n",str);

                    //Accepted Traffic per packet - to identify flux path
                    for(unsigned int p = 0; p < internalPackets[dim][link].size(); p++){
                        PacketInfo *info = internalPackets[dim][link][p];
                        sprintf(str,"%.5f\t%ull\t%i\t%i",
                                acceptedTraffic[dim][link][p],
                                info->getFlowId(),
                                info->getSource(),
                                info->getDestination());
                        // Replace ',' by '.'
                        i = 0;
                        do {
                            if(str[i] == ',') str[i] = '.';
                            i++;
                        }while(str[i] != '\0');
                        fprintf(fpOut,"%s\n",str);
                    }

                    fclose(fpOut);
                }
            }
        }
    }

    //Textual Maps
    //obs: for mesh2d, dim 0-3 => E,W,N,S
    std::string aux = "\nAverage Bandwidth\n";
    unsigned int nodeId, linkId;
    for(int y = topology->sp.getYSize() - 1; y >= 0; y--){
        for(unsigned int x = 0; x < topology->sp.getXSize(); x++){
            nodeId = COORDINATE_2D_TO_ID(x,y,topology->sp.getXSize());
            linkId = COORDINATE_2D_TO_ID(x,y,topology->sp.getXSize()-1);
            if(x < topology->sp.getXSize() - 1){
                sprintf(str, "(%03d) %.2f  %.2f ", nodeId, linkAvgBandwidth[0][linkId], linkAvgBandwidth[1][linkId]);
                //std::cout << "X=" << x << "; Y=" << y << "; linkAvgBw=" << linkAvgBandwidth[0][id] << std::endl;
            }else{
                sprintf(str, "(%03d)\n", nodeId);
            }
            aux.append(str);
        }
        if(y > 0){
            for(unsigned int x = 0; x < topology->sp.getXSize(); x++){
                linkId = COORDINATE_2D_TO_ID(x,y-1,topology->sp.getXSize());
                if(x < topology->sp.getXSize() - 1){
                    sprintf(str, "%.2f             ", linkAvgBandwidth[3][linkId]);
                }else{
                    sprintf(str, "%.2f\n\n", linkAvgBandwidth[3][linkId]);
                }
                aux.append(str);
            }

            for(unsigned int x = 0; x < topology->sp.getXSize(); x++){
                linkId = COORDINATE_2D_TO_ID(x,y-1,topology->sp.getXSize());
                if(x < topology->sp.getXSize() - 1){
                    sprintf(str, "%.2f             ", linkAvgBandwidth[2][linkId]);
                }else{
                    sprintf(str, "%.2f\n", linkAvgBandwidth[2][linkId]);
                }
                aux.append(str);
            }
        }
    }

    sprintf(str,"%s/text_maps",resultDir);
    if( (fpOut=fopen(str,"a+t")) == NULL ) {
        printf("ERROR: Impossible to create file %s",str);
        return TrafficAnalysis::NoOutputFile;
    } else {
        fprintf(fpOut,aux.c_str());
    }
    fclose(fpOut);

    //todo: max/avg/min
    aux = "\nAverage Cycles per Flit\n";
    for(int y = topology->sp.getYSize() - 1; y >= 0; y--){
        for(unsigned int x = 0; x < topology->sp.getXSize(); x++){
            nodeId = COORDINATE_2D_TO_ID(x,y,topology->sp.getXSize());
            linkId = COORDINATE_2D_TO_ID(x,y,topology->sp.getXSize()-1);
            if(x < topology->sp.getXSize() - 1){
                sprintf(str, "(%03d) %.2f  %.2f ", nodeId, linkAvCpF[0][linkId], linkAvCpF[1][linkId]);
                //std::cout << "X=" << x << "; Y=" << y << "; linkAvgBw=" << linkAvgBandwidth[0][id] << std::endl;
            }else{
                sprintf(str, "(%03d)\n", nodeId);
            }
            aux.append(str);
        }
        if(y > 0){
            for(unsigned int x = 0; x < topology->sp.getXSize(); x++){
                linkId = COORDINATE_2D_TO_ID(x,y-1,topology->sp.getXSize());
                if(x < topology->sp.getXSize() - 1){
                    sprintf(str, "%.2f             ", linkAvCpF[3][linkId]);
                }else{
                    sprintf(str, "%.2f\n\n", linkAvCpF[3][linkId]);
                }
                aux.append(str);
            }

            for(unsigned int x = 0; x < topology->sp.getXSize(); x++){
                linkId = COORDINATE_2D_TO_ID(x,y-1,topology->sp.getXSize());
                if(x < topology->sp.getXSize() - 1){
                    sprintf(str, "%.2f             ", linkAvCpF[2][linkId]);
                }else{
                    sprintf(str, "%.2f\n", linkAvCpF[2][linkId]);
                }
                aux.append(str);
            }
        }
    }

    sprintf(str,"%s/text_maps",resultDir);
    if( (fpOut=fopen(str,"a+t")) == NULL ) {
        printf("ERROR: Impossible to create file %s",str);
        return TrafficAnalysis::NoOutputFile;
    } else {
        fprintf(fpOut,aux.c_str());
    }
    fclose(fpOut);

    aux = "\nThroughput\n";
    for(int y = topology->sp.getYSize() - 1; y >= 0; y--){
        for(int x = 0; x < topology->sp.getXSize(); x++){
            nodeId = COORDINATE_2D_TO_ID(x,y,topology->sp.getXSize());
            linkId = COORDINATE_2D_TO_ID(x,y,topology->sp.getXSize()-1);
            if(x < topology->sp.getXSize() - 1){
                sprintf(str, "(%03d) %.2f  %.2f ", nodeId, linkThroughput[0][linkId], linkThroughput[1][linkId]);
                //std::cout << "X=" << x << "; Y=" << y << "; linkAvgBw=" << linkAvgBandwidth[0][id] << std::endl;
            }else{
                sprintf(str, "(%03d)\n", nodeId);
            }
            aux.append(str);
        }
        if(y > 0){
            for(int x = 0; x < topology->sp.getXSize(); x++){
                linkId = COORDINATE_2D_TO_ID(x,y-1,topology->sp.getXSize());
                if(x < topology->sp.getXSize() - 1){
                    sprintf(str, "%.2f             ", linkThroughput[3][linkId]);
                }else{
                    sprintf(str, "%.2f\n\n", linkThroughput[3][linkId]);
                }
                aux.append(str);
            }

            for(int x = 0; x < topology->sp.getXSize(); x++){
                linkId = COORDINATE_2D_TO_ID(x,y-1,topology->sp.getXSize());
                if(x < topology->sp.getXSize() - 1){
                    sprintf(str, "%.2f             ", linkThroughput[2][linkId]);
                }else{
                    sprintf(str, "%.2f\n", linkThroughput[2][linkId]);
                }
                aux.append(str);
            }
        }
    }

    sprintf(str,"%s/text_maps",resultDir);
    if( (fpOut=fopen(str,"a+t")) == NULL ) {
        printf("ERROR: Impossible to create file %s",str);
        return TrafficAnalysis::NoOutputFile;
    } else {
        fprintf(fpOut,aux.c_str());
    }
    fclose(fpOut);

    return TrafficAnalysis::Ok;
}

InternalAnalysis::~InternalAnalysis() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Model/Analysis/InternalAnalysis" << std::endl;
#endif
}
