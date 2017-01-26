/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* TrafficModelGenerator.cpp
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
* ----------------------------------------------------------------------------
* 11/12/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*/



#include "Model/Traffic/TrafficModelGenerator.h"
#include "Model/Traffic/MathFunction.h"

#include "Model/System/Defines.h"
#include "Model/System/SystemOperation.h"
#include "Model/System/Experiment.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include <QString>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

TrafficModelGenerator::TrafficModelGenerator(SystemConfiguration sysConf, float fClk, unsigned short numberCyclesPerFlit) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/TrafficPattern/TrafficModelGenerator" << std::endl;
#endif

    this->sysConf = sysConf;
    this->fClk = fClk;
    this->tClk = (1.0f / fClk ) * 1000.0f;
    this->numElements = sysConf.getSystemParameters().getNumberElements();
    this->traffics = sysConf.getTrafficConfiguration();
    this->countTrafficConfigured = traffics.size();
    this->dataWidth = sysConf.getSystemParameters().getDataWidth();
    this->channelBW = dataWidth * fClk;

    this->numberCyclesPerFlit = numberCyclesPerFlit;

    this->lastPayloadLength = 0;
    this->pck2Send = 0;
    this->requiredBw = 0.0;

    this->flowsStr.resize(numElements);
}

int TrafficModelGenerator::functionProbability(TrafficParameters *tp) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::functionProbability" << std::endl;
#endif

    unsigned int i,j;
    unsigned long int pckInserted = 0, temp = 0, pck2Fix = 0;

    unsigned long int pck2Send = tp->getPackageToSend();
    float requiredBwStdDev = tp->getRequiredBandwidthStdDeviation() / 100.0f;

    unsigned int probFunction = tp->getProbabilityFunction();

    // It determines the number of packets to be sent for each rate range
    for( pckInserted = 0, i = 0; i < 100; i++ ) {
        switch( probFunction ) {
            case 0:{ // NORMAL
                pck2SendArray[i] = (unsigned long int) roundf( (0.01f * (float) pck2Send *
                        MathFunction::fNormal(requiredBwArray[i],requiredBw,requiredBwStdDev)) );
                break;
            }
            case 1: // EXPONENTIAL
                pck2SendArray[i] = (unsigned long int) roundf( (0.01f * pck2Send *
                        MathFunction::fExponential(requiredBwArray[i],requiredBw)) );
                break;
        }
        pckInserted += pck2SendArray[i];
    }

    // It determines how many packets were not inserted due to truncation. This
    // value is applied to the probability function iterativelly until it
    // reaches zero
    j = 0;
    while( (pck2Send > pckInserted) && (j < 100) ) {
        pck2Fix = pck2Send - pckInserted;
        for(i = 0; i < 100; i++) {
            if(pck2Fix > 0) {
                switch(probFunction) {
                    case 0: // NORMAL
                        temp = (unsigned long int) roundf((0.01f * pck2Fix *
                                MathFunction::fNormal(requiredBwArray[i],requiredBw,requiredBwStdDev)));
                        break;
                    case 1: // EXPONENTIAL
                        temp = (unsigned long int) roundf((0.01f * pck2Fix *
                                MathFunction::fExponential(requiredBwArray[i],requiredBw)));
                        break;
                }
                if( pck2Send > (temp + pckInserted) ) {
                    pck2SendArray[i] += temp;
                } else {
                    pck2SendArray[i] += pck2Send - pckInserted;
                }
                pckInserted += temp;
            }
        }
        j++;
    }

    if( pck2Send > pckInserted ) {
        switch( probFunction ) {
            case 0: // NORMAL
                i = 0;
                while(requiredBwArray[i] < requiredBw) {
                    i++;
                }
                pck2SendArray[i] += pck2Send - pckInserted;
                break;
            case 1: // EXPONENTIAL
                pck2SendArray[0] += pck2Send - pckInserted;
                break;
        }
    }

    if(pckInserted == 0) {
        return pck2Send;
    }
    return 0;

}

void TrafficModelGenerator::removeZeroPayloadPacket(TrafficParameters *tp) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::removeZeroPayloadPacket" << std::endl;
#endif

    unsigned int i,j;
    float temp;

    // It determines the index of the average value
    j = 0;
    while( tp->packageToSendArray[j] <= pck2SendArray[j+1] ) {
        j++;
    }

    // It calculates the payload length for each rate to be used
    for( i = 0; i < tp->getNumberRates(); i++) {
        switch (tp->getInjectionType()) {
            case 2: // Variable message size - Fix idle time
                temp = (1.0f/tp->requiredBandwidthArray[i]-1.0f);
                tp->setPayloadLength( (unsigned int) (tp->getIdleTime() / (numberCyclesPerFlit * (temp)))-1 );
                break;
            case 3: // Variable message size - Fix message interval
                tp->setPayloadLength( (unsigned int) ((tp->getIntervalTime()*tp->requiredBandwidthArray[i])/numberCyclesPerFlit) - 1 );
                break;
        }
        // It resets the number of packets to be sent with no payload and increments
        // the number of packets to be sent at the average injection rate
        if(tp->getPayloadLength() == 0) {
            tp->packageToSendArray[j] += tp->packageToSendArray[i];
            tp->packageToSendArray[i] = 0;
        }
    }

}

unsigned int TrafficModelGenerator::calculateIdleBasedOnPayloadLength(unsigned int payloadLength, float requiredBw) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::calculateIdleBasedOnPayloadLength" << std::endl;
#endif
// It determines the number of idle cycles taking into account only the bandwidth
// required by the application. Therefore, it does not take into account the header overhead
// If application requires a bandwidth greater than the channel bandwidth it is considered
// that 100% of the channel bandwidth is required (none idle cycle is inserted)

    float tmp = (float) ( this->channelBW / requiredBw ) - 1.0f;

    if( tmp < 0 ) {
        tmp = 0;
    }

    return ( (unsigned int) ( payloadLength * numberCyclesPerFlit * tmp ) );
}

unsigned int TrafficModelGenerator::calculatePayloadLengthBasedOnIdle(unsigned int idle, float requiredBw) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::calculatePayloadLengthBasedOnIdle" << std::endl;
#endif

    float tmp;

    tmp = (float) (this->channelBW / requiredBw) - 1.0f;

    if(tmp < 0) {
        tmp = 0;
    }

    tmp = idle / (numberCyclesPerFlit * tmp);

    if(tmp < 1) {
        tmp = 1;
    }
    return (unsigned int) tmp;
}

unsigned int TrafficModelGenerator::calculatePayloadLengthBasedOnInterArrival(unsigned int iat, float requiredBw) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::calculatePayloadLengthBasedOnInterArrival" << std::endl;
#endif
    return (unsigned int) (iat / numberCyclesPerFlit) * (unsigned int) (requiredBw/this->channelBW);

}

unsigned int TrafficModelGenerator::calculateInterArrivalBasedOnPayloadLength(unsigned int payloadLength, float requiredBw) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::calculateInterArrivalBasedOnPayloadLength" << std::endl;
#endif
    float tmp;

    tmp = (float) (this->channelBW/requiredBw);
    return (unsigned int) (payloadLength * numberCyclesPerFlit * tmp);
}

void TrafficModelGenerator::adjustParameters() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::adjustParameters" << std::endl;
#endif

    unsigned int payloadLength   = 0;
    unsigned int idle            = 0;
    unsigned int iat             = 0;
    float        requiredBwStdev = 0.0;
    float        alfaOn          = 0.0;
    float        alfaOff         = 0.0;

    unsigned int numberRates = 0;

    for(unsigned int trafficIndex = 0; trafficIndex < countTrafficConfigured; trafficIndex++) {
        TrafficParameters* tp = &traffics[trafficIndex];
        unsigned int injType = tp->getInjectionType();
        switch(injType) {
        // Cases according to the view
            case 0: // Constant injection rate
                payloadLength = tp->getMessageSize();
                numberRates = 1; // There is no probabilistic generation
                break;
            case 1: // Variable idle period, fixed packet size
                payloadLength = tp->getMessageSize();
                break;
            case 2: // Variable packet size, fixed idle period
                idle = tp->getIdleTime();
                break;
            case 3: // Variable packet size, fixed interval
                iat = tp->getIntervalTime();
                break;
            case 4: // Variable idle interval time, fixed packet size
                payloadLength = tp->getMessageSize();
                break;
            case 5: // Variable burst size, fixed idle interval time
                payloadLength = tp->getMessageSize();
                iat = tp->getIntervalTime();
                break;
        }

        // Injection type different of constant
        if( injType != 0  ) {
            switch(tp->getProbabilityFunction()) {
                case 0: // Normal
                    requiredBwStdev = tp->getRequiredBandwidthStdDeviation();
                case 1: // Exponential
                    numberRates = DefaultValuesTrafficGeneration::DEFAULT_NUMBER_RATES;
                    break;
                case 2: // Pareto
                    alfaOn = tp->getAlfaOn();
                    alfaOff = tp->getAlfaOff();
                    numberRates = 1; // It is determined at run time
                    break;
            }
        }

        // It updates the traffic patterns parameters by doing the conversion
        // of units: from ns to cycles, from bits to flits
        if(payloadLength) {
            tp->setPayloadLength((unsigned int) ceil( (float)payloadLength/(float)dataWidth) );
        }
        if( tp->getPayloadLength() < DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH ) {
            tp->setPayloadLength( DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH );
        }
        if(idle) {
            tp->setIdleTime( (unsigned int) ceil( (float)idle / (float)tClk) );
        }
        if(iat) {
            tp->setIntervalTime( (unsigned int) ceil( (float)iat /(float)tClk ) );
        }
        tp->setRequiredBandwidthStdDeviation( requiredBwStdev );
        tp->setAlfaOn( alfaOn );
        tp->setAlfaOff( alfaOff );
        tp->setNumberRates( numberRates );
    }

}

void TrafficModelGenerator::generateVariableRate() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::generateVariableRate" << std::endl;
#endif

    for(unsigned int trafficIndex = 0; trafficIndex < countTrafficConfigured; trafficIndex++) {
        TrafficParameters* tp = &traffics[trafficIndex];
        // It generates the variable rates
        if( tp->getInjectionType() != 0 /*CONSTANT*/  ) {
            // It fills required_bw_array
            for(unsigned int i = 0; i < 100; i++) {
                this->requiredBwArray[i] = ((float)i+1)/100.0f; // required_bw_array[i+1] + 0.01
            }

            // It fills pck_2send_array by using a probability function
            requiredBw = tp->getRequiredBandwidth() / channelBW;
            if(requiredBw > 1.0) {
                requiredBw = 1.0;
            }
            int resFunProb = this->functionProbability(tp);
            //  != ERROR |
            if( resFunProb != 0 ) {
                char *error = new char[512];
                sprintf(error,"[TrafficModelGenerator] WARNING: No packet was inserted! Input variable \"pck_2send\" (=%d) is too small for the selected function of probability.",resFunProb);
                throw error;
            }
            for(unsigned int z = 0; z < 100; z++) {
                tp->requiredBandwidthArray[z] = this->requiredBwArray[z];
                tp->packageToSendArray[z] = this->pck2SendArray[z];
            }
        }
    }

}

void TrafficModelGenerator::generateFlow(TrafficParameters *tp) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::generateSingleDestinationDistributions" << std::endl;
#endif

    unsigned int numberRatesWithNoPackets = 0;

    unsigned int spatialDistribution = tp->getSpatialDistribution();

    unsigned int injType = tp->getInjectionType();

    // For variable injection rates based on the variation of the
    // payload length (except for Pareto-based ones),
    // it removes the packets with no payload
    if(tp->getProbabilityFunction() != 2) { // != PARETO
        // == VAR_PCK_FIX_IDL ||       == VAR_PCK_FIX_IAT
        if( (injType == 2) || (injType == 3) ) {
            this->removeZeroPayloadPacket(tp);

        }

        // It determines how many rates in the distribution has no packets
        // to be sent. They will not be written to the file
        if( injType != 0 ) { // != CONSTANT
            unsigned int i;
            for( numberRatesWithNoPackets = 0,  i = 0; i < tp->getNumberRates(); i++) {
                if( tp->packageToSendArray[i] == 0 ) {
                    numberRatesWithNoPackets++;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    /// PART VI(a) - CBR TRAFFIC GENERATION
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    if(injType == 0) { // == CONSTANT
        tp->setIdleTime( this->calculateIdleBasedOnPayloadLength(tp->getPayloadLength(),
                                                                 tp->getRequiredBandwidth()) );

        // Non-uniform distribution type 1: the number of packets to be sent decreases with
        // the distance between the source and the destination nodes
        if(spatialDistribution == 7) { // Non-uniform
            pck2Send = tp->getPackageToSend()/2; // TODO Identificar se é vizinho ou não
        } else {
            pck2Send = tp->getPackageToSend();
        }
        requiredBw = tp->getRequiredBandwidth();

        this->writeFlow(tp);
    } else {
        //////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////
        /// PART VI(b) - VBR TRAFFIC GENERATION
        //////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////
        if( tp->getProbabilityFunction() == 2 ) { // == PARETO
            this->requiredBw = 0.0;
            if( spatialDistribution == 7 ) { // Non-uniform
                this->pck2Send = tp->getPackageToSend() / 2;
            } else {
                this->pck2Send = tp->getPackageToSend();
            }
            this->writeFlow( tp, tp->getInjectionType() );
        } else {
            for( unsigned int i = 0; i < tp->getNumberRates() ; i++ ) {
                if( tp->packageToSendArray[i] != 0 ) {
                    unsigned int burstSize = 0;
                    switch( injType ) {
                        case 1: // VAR_IDL_FIX_PCK // It determines the number of idle cycles
                            tp->setIdleTime(this->calculateIdleBasedOnPayloadLength( tp->getPayloadLength(),
                                                                                     tp->requiredBandwidthArray[i] ) );
                            break;
                        case 2: // VAR_PCK_FIX_IDL // It determines the payload length
                            tp->setPayloadLength( this->calculatePayloadLengthBasedOnIdle( tp->getIdleTime(),
                                                                                           tp->requiredBandwidthArray[i]) );
                            if(tp->getPayloadLength() < DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH) {
                                tp->setPayloadLength( DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH );
                            }
                            break;
                        case 3: // VAR_PCK_FIX_IAT // It determines the payload length and the number of idle cycles
                            tp->setPayloadLength( this->calculatePayloadLengthBasedOnInterArrival(tp->getIntervalTime(),
                                                                                                  tp->requiredBandwidthArray[i]) );
                            if( tp->getPayloadLength() < DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH ) {
                                tp->setPayloadLength( DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH );
                            }

                            if( tp->getIntervalTime() < (tp->getPayloadLength() * numberCyclesPerFlit) ) {
                                continue;
                            }
                            tp->setIdleTime( tp->getIntervalTime() - (tp->getPayloadLength() * numberCyclesPerFlit) );
                            break;
                        case 4: // VAR_IAT_FIX_PCK // It determines the interval time and the number if idle cycles
                            tp->setIntervalTime( this->calculateInterArrivalBasedOnPayloadLength(tp->getPayloadLength(),
                                                                                                 tp->requiredBandwidthArray[i]) );

                            if( tp->getIntervalTime() < (tp->getPayloadLength() * numberCyclesPerFlit) ) {
                                // Não gera fluxo
                                continue;
                            }
                            tp->setIdleTime( tp->getIntervalTime() - (tp->getPayloadLength() * numberCyclesPerFlit) );
                            break;
                        case 5:{ // VAR_BST_FIX_IAT
                            // It determines the burst size and the size of the last packet
                            // It determines the burst size. If it does not have a decimal part, then
                            // last_payload_lengh = payload_length. If it has a decimal part, then the
                            // burst size is incremented by one packet and the length of this last
                            // packet is determined. Depending on the required bandwidth, the length of
                            // the payload of the last packet can equal 0, 1 or a number <= payload_length
                            float tmp;
                            tmp = ((tp->requiredBandwidthArray[i]*tp->getIntervalTime()) /
                                   ((float)((tp->getPayloadLength()*numberCyclesPerFlit))));
                            if(((unsigned int)(100*tmp)%100) == 0) { // If decimal part is 0
                                burstSize = (unsigned int) tmp;
                                lastPayloadLength = tp->getPayloadLength();
                            } else {
                                burstSize = (unsigned int) (round((float) (tmp+0.5)));
                                tmp = (trunc((fmod( tp->requiredBandwidthArray[i] * tp->getIntervalTime(),
                                                    (float) (tp->getPayloadLength())*numberCyclesPerFlit ))/numberCyclesPerFlit));
                                if(tmp < 1) {
                                    lastPayloadLength = 0;
                                } else {
                                    if(tmp < DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH) {
                                        lastPayloadLength = DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH;
                                    } else {
                                        lastPayloadLength = (unsigned int) tmp;
                                    }
                                }
                            }
                            if( lastPayloadLength == 0 ) {
                                if( tp->getIntervalTime() < numberCyclesPerFlit * ((burstSize-1)*(tp->getPayloadLength())) ) {
                                    // Não gera o fluxo
                                    continue;
                                }
                                tp->setIdleTime( tp->getIntervalTime() - numberCyclesPerFlit *
                                                 ((burstSize-1)*(tp->getPayloadLength())) );
                            } else {
                                if( tp->getIntervalTime() < numberCyclesPerFlit * (((burstSize-1)*(tp->getPayloadLength())) + (lastPayloadLength)) ) {
                                    // Não gera o fluxo
                                    continue;
                                }
                                tp->setIdleTime( tp->getIntervalTime() - numberCyclesPerFlit *
                                                 (((burstSize-1)*(tp->getPayloadLength())) + (lastPayloadLength)) );
                            }
                            break;
                        }
                    }
                    requiredBw = tp->requiredBandwidthArray[i] * channelBW;
                    this->pck2Send = tp->packageToSendArray[i];
                    this->writeFlow( tp,0,burstSize );
                }
            }
        }
    }

}

void TrafficModelGenerator::generateTrafficModel() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::generateTrafficModel" << std::endl;
#endif

    for(unsigned int trafficIndex = 0; trafficIndex < countTrafficConfigured; trafficIndex++) {
        TrafficParameters* tp = &traffics[trafficIndex];
        this->flowId = 0; // NOTE Here set the flowId | threadId
        //////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////
        /// PART VI - TRAFFIC GENERATION FOR SINGLE DESTINATION DISTRIBUTIONS
        //////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////
        this->generateFlow(tp);
    }

}

void TrafficModelGenerator::generateTraffic(const char *diretorio) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::generateTraffic" << std::endl;
#endif

    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    /// PART I - ADDRESS-BASED TRAFFIC VALIDATION
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    // It determines the destination node for some spatial distributions	and
    // checks if it is valid (different of the source node and inside the network)

    // No longer necessary - since version 2.0 - address validation on MainWindow


    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    /// PART II - PARAMETERS ADJUSTING
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    // Adjusts the parameters of the traffic pattens by reseting the ones which
    // are not necessary and converting some of the paramenters to a system of
    // units based on clock cycles and flits.
    // It resets the additional parameters before it reads them, because, for some
    // injection types, some of the parameters are not used, and the configuration
    // file is clearer if a defined value is set (that is, 0)
    this->adjustParameters();

    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    /// PART III - FLOW CONTROL DETERMINATION
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    // It calculates the number of cycles per flit for the used flow control
    // technique
//    No longer necessary - since version 2.0 - the control sends this information

    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    /// PART IV - VARIABLE RATE GENERATION
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    try {
        this->generateVariableRate();
    } catch(const char* exception) {
        throw exception;
    }

    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    /// PART V - TRAFFIC MODEL GENERATION
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    this->generateTrafficModel();

    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    /// PART VI - GENERATE FILE TRAFFIC CONFIGURATION (.tcf)
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    try {
        this->createFileTrafficConfiguration( diretorio );
    } catch (const char* exception) {
        throw exception;
    }

}

void TrafficModelGenerator::createFileTrafficConfiguration(const char *diretorio) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::createFileTrafficConfiguration" << std::endl;
#endif

    FILE* outFile;

    std::stringstream ss;
    ss << diretorio;
    ss << "/";
    ss << "traffic.tcf";

    std::string arqDir = ss.str();

    if( (outFile = fopen(arqDir.c_str(),"wt")) == NULL ) {
        char* buffer = new char[512];
        sprintf(buffer,"ERROR: Impossible to create file: %s",arqDir.c_str());
        throw buffer;
    }

    for(unsigned int source = 0; source < numElements; source++) {
        fprintf(outFile,"tg_%u\n",source);
        fprintf(outFile,"%d\n",flowsStr[source].size());
        for( int flowIndex = 0; flowIndex < flowsStr[source].size(); flowIndex++ ) {
            QString flow = flowsStr[source].at(flowIndex);
            fprintf(outFile,"%s",flow.toStdString().c_str());
        }

        fprintf(outFile,"\n\n");
    }
    fprintf(outFile,"\n\n// Parameters");
    fprintf(outFile,"\n 0  Type");
    fprintf(outFile,"\n 1  Destination");
    fprintf(outFile,"\n 2  Flow Id ");
    fprintf(outFile,"\n 3  Traffic Class");
    fprintf(outFile,"\n 4  Switching Type");
    fprintf(outFile,"\n 5  Package to send");
    fprintf(outFile,"\n 6  Deadline");
    fprintf(outFile,"\n 7  Required bandwidth");
    fprintf(outFile,"\n 8  Payload Length");
    fprintf(outFile,"\n 9  Idle Cycles");
    fprintf(outFile,"\n10  Out Interval Time");
    fprintf(outFile,"\n11  Burst Size");
    fprintf(outFile,"\n12  Last Payload Length");
    fprintf(outFile,"\n13  Pareto - Alfa On");
    fprintf(outFile,"\n14  Pareto - Alfa Off");

    fclose(outFile);
}

void TrafficModelGenerator::writeFlow(TrafficParameters *tp, unsigned int type, unsigned int burstSize) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::writeFlow" << std::endl;
#endif

    char str[512];

    sprintf(str,"%u  %u  %u  %u  %u  %lu  %lu  %.4f  %u  %u  %u  %u  %u  %.2f  %.2f  \n",
            type,                       // Type = 0 when not pareto distribution, TypeOfInjection when pareto distribution
            tp->getDestination(),       // Destination
            this->flowId,               // flow_id
            tp->getTrafficClass(),      // traffic_class
            tp->getSwitchingTechnique(),// switching_type
            this->pck2Send,             // pck_2send
            tp->getDeadline(),          // deadline
            this->requiredBw,           // required_bw
            tp->getPayloadLength(),     // payload_length
            tp->getIdleTime(),          // idle
            tp->getIntervalTime(),      // iat
            burstSize,                  // burst_size
            lastPayloadLength,          // last_payload_length
            tp->getAlfaOn(),            // parameter1
            tp->getAlfaOff()            // parameter2
            );
    for( unsigned int i = 0; str[i] != '\n'; i++ ) {
        if( str[i] == ',' ) {
            str[i] = '.';
        }
    }

    QString flow = QString::fromStdString( str );
    flowsStr[tp->getSource()].append(flow);
}


TrafficModelGenerator::~TrafficModelGenerator() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Model/TrafficPattern/TrafficModelGenerator" << std::endl;
#endif
}
