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
*
*/

#include "include/Model/Traffic/TrafficModelGenerator.h"
#include "include/Model/Traffic/TrafficPatternManager.h"
#include "include/Model/Traffic/MathFunction.h"

#include "include/Model/System/SystemParameters.h"
#include "include/Model/System/Experiment.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

TrafficModelGenerator::TrafficModelGenerator(SystemParameters *sp, Experiment *exp, TrafficPatternManager *gpt) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/TrafficPattern/TrafficModelGenerator" << std::endl;
#endif

    this->sp = sp;
    this->exp = exp;
    this->gpt = gpt->clone();

    this->lastPayloadLength = 0;
    this->requiredBw = 0;
    this->numberCyclesPerFlit = 0;
    this->numTrafficPatter = 0;
    this->pck2Send = 0;
    this->requiredBw = 0.0;
}

void TrafficModelGenerator::getDestination(TrafficParameters *tp) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::getDestination" << std::endl;
#endif

    unsigned int xSrc = tp->getSourceNodeX();
    unsigned int ySrc = tp->getSourceNodeY();
    unsigned int zSrc = tp->getSourceNodeZ();
    unsigned int xSize = sp->getXSize();
    unsigned int ySize = sp->getYSize();
//    unsigned int zSize = sp->getZSize();

    unsigned int mask, i;
    unsigned int addressWidth;

    // It determines the width of the addresses fields
    if( xSize >= ySize ) {
        i = xSize - 1;
    } else {
        i = ySize - 1;
    }
    addressWidth = 1;
    while( (i = i >> 1) > 0 ) {
        addressWidth++;
    }

    // It determines the mask to be used with to calculate the destination address
    for( mask = 1, i = 0; i < addressWidth - 1; i++ ) {
        mask = (mask << 1) | 1;
    }

    // TODO: Verificar como calcular considerando a dimensão "Z"
    // It determines the destination address
    switch( tp->getSpatialDistribution() ) {
        case 1: // BIT_REVERSAL
            tp->setDestinationNodeX( ( ( ySrc << ( addressWidth - 1 ) ) | ( ySrc >> ( addressWidth - 1 ) ) ) & mask );
            tp->setDestinationNodeY( ( ( xSrc << ( addressWidth - 1 ) ) | ( xSrc >> ( addressWidth - 1 ) ) ) & mask );
//            tp->setDestinationNodeZ( ( ( zSrc << ( addressWidth - 1 ) ) | ( zSrc >> ( addressWidth - 1 ) ) ) & mask );
            break;
        case 2: // PERFECT_SHUFFLE
            tp->setDestinationNodeX( ( ( xSrc << 1 ) | ( ySrc >> ( addressWidth - 1 ) ) ) & mask );
            tp->setDestinationNodeY( ( ( ySrc << 1 ) | ( xSrc >> ( addressWidth - 1 ) ) ) & mask );
//            tp->setDestinationNodeZ();
            break;
        case 3: // BUTTERFLY
            tp->setDestinationNodeX( (xSrc & ((~(0x1 << (addressWidth-1))) & mask)) | ((ySrc & 0x1) << (addressWidth-1)) );
            tp->setDestinationNodeY( ((ySrc >> 1) << 1) | (xSrc >> (addressWidth-1)) );
//            tp->setDestinationNodeZ();
            break;
        case 4: // MATRIX_TRANSPOSE
            tp->setDestinationNodeX( ySrc );
            tp->setDestinationNodeY( xSrc );
//            tp->setDestinationNodeZ();
            break;
        case 5: // COMPLEMENT
            tp->setDestinationNodeX( ~xSrc & mask );
            tp->setDestinationNodeY( ~ySrc & mask );
            tp->setDestinationNodeZ( ~zSrc & mask );
            break;
    }


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
            case 3: // Variable message size - Fix message inter-arrival
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

unsigned int TrafficModelGenerator::calculateIdleBasedOnPayloadLength(unsigned int payloadLength,
                                                         float channelBw,float requiredBw) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::calculateIdleBasedOnPayloadLength" << std::endl;
#endif
// It determines the number of idle cycles taking into account only the bandwidth
// required by the application. Therefore, it does not take into account the header overhead
// If application requires a bandwidth greater than the channel bandwidth it is considered
// that 100% of the channel bandwidth is required (none idle cycle is inserted)

    float tmp = (float) ( channelBw / requiredBw ) - 1.0f;

    if( tmp < 0 ) {
        tmp = 0;
    }

    return ( (unsigned int) ( payloadLength * numberCyclesPerFlit * tmp ) );
}

unsigned int TrafficModelGenerator::calculatePayloadLengthBasedOnIdle(unsigned int idle,
                                                                 float channelBw, float requiredBw) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::calculatePayloadLengthBasedOnIdle" << std::endl;
#endif

    float tmp;

    tmp = (float) (channelBw / requiredBw) - 1.0f;

    if(tmp < 0) {
        tmp = 0;
    }

    tmp = idle / (numberCyclesPerFlit * tmp);

    if(tmp < 1) {
        tmp = 1;
    }
    return (unsigned int) tmp;
}

unsigned int TrafficModelGenerator::calculatePayloadLengthBasedOnInterArrival(unsigned int iat,
                                                                         float channelBw, float requiredBw) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::calculatePayloadLengthBasedOnInterArrival" << std::endl;
#endif
    return (unsigned int) (iat / numberCyclesPerFlit) * (unsigned int) (requiredBw/channelBw);

}

unsigned int TrafficModelGenerator::calculateInterArrivalBasedOnPayloadLength(unsigned int payloadLength,
                                                                         float channelBw, float requiredBw) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::calculateInterArrivalBasedOnPayloadLength" << std::endl;
#endif
    float tmp;

    tmp = (float) (channelBw/requiredBw);
    return (unsigned int) (payloadLength * numberCyclesPerFlit * tmp);
}

void TrafficModelGenerator::validateTrafficAddresses() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::validateTrafficAddresses" << std::endl;
#endif

    unsigned int xSize = this->sp->getXSize();
    unsigned int ySize = this->sp->getYSize();
    unsigned int zSize = this->sp->getZSize();

    for(unsigned int x = 0; x < xSize; x++) {
        for(unsigned int y = 0; y < ySize; y++) {
            for(unsigned int z = 0; z < zSize; z++) {
                unsigned id = COORDINATE_3D_TO_ID(x,y,z,xSize,ySize);
                Node* no = gpt->getNode(id);
                if(no != NULL) {
                    for(unsigned int p = 0; p < MAX_PATTERNS; p++) {
                        if(no->isPatternActive(p)) {
                            TrafficParameters* trafficPattern = no->getTrafficPattern(p);
                            unsigned int spatialDistribution = trafficPattern->getSpatialDistribution();
                            //                       > SPECIFIED ADDRESS          < UNIFORM
                            if( (spatialDistribution > 0) && (spatialDistribution < 6) ) {
                                getDestination(trafficPattern);
                            }
                            //                     < UNIFORM
                            if(spatialDistribution < 6) {
                                unsigned int xDest = trafficPattern->getDestinationNodeX();
                                unsigned int yDest = trafficPattern->getDestinationNodeY();
                                unsigned int zDest = trafficPattern->getDestinationNodeZ();
                                // If destination node is equal the source node, disable traffic pattern
                                if( (xDest == x) && (yDest == y) && (zDest == z) ) {
                                    no->setPatternActive(false,p);
                                }
                                // If destination node is outside from the network, disable traffic pattern
                                if( (xDest >= xSize) || (yDest >= ySize) || (zDest >= zSize) ) {
                                    no->setPatternActive(false,p);
                                }
                            }
//                            std::cout << ""
                        }
                    }
                }

            }
        }
    }

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

    unsigned int xSize = this->sp->getXSize();
    unsigned int ySize = this->sp->getYSize();
    unsigned int zSize = this->sp->getZSize();

    for(unsigned int x = 0; x < xSize; x++) {
        for(unsigned int y = 0; y < ySize; y++) {
            for(unsigned int z = 0; z < zSize; z++) {
                Node* no = gpt->getNode(COORDINATE_3D_TO_ID(x,y,z,
                                                            xSize,
                                                            ySize));
                if(no != NULL) {
                    for(unsigned int p = 0; p < MAX_PATTERNS; p++) {
                        if(no->isPatternActive(p)) {
                            TrafficParameters* tp = no->getTrafficPattern(p);
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
                                case 3: // Variable packet size, fixed inter-arrival
                                    iat = tp->getIntervalTime();
                                    break;
                                case 4: // Variable idle inter-arrival time, fixed packet size
                                    payloadLength = tp->getMessageSize();
                                    break;
                                case 5: // Variable burst size, fixed idle inter-arrival time
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
                                tp->setPayloadLength((unsigned int) ceil( (float)payloadLength/(float)sp->getDataWidth()) );
                            }
                            if( tp->getPayloadLength() < DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH ) {
                                tp->setPayloadLength( DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH );
                            }

                            if(idle) {
                                tp->setIdleTime( (unsigned int) ceil( (float)idle / (float)sp->getTClk() ) );
                            }
                            if(iat) {
                                tp->setIntervalTime( (unsigned int) ceil( (float)iat /(float)sp->getTClk() ) );
                            }
                            tp->setRequiredBandwidthStdDeviation( requiredBwStdev );
                            tp->setAlfaOn( alfaOn );
                            tp->setAlfaOff( alfaOff );
                            tp->setNumberRates( numberRates );
                        }
                    }
                }
            }
        }
    }

}

void TrafficModelGenerator::determineFlowControl() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::determineFlowControl" << std::endl;
#endif

    switch( exp->getFlowControl() ) {
        case 0: this->numberCyclesPerFlit = 4; break; // HS-(WH)
        case 1: // CB-(WH)
        case 2: this->numberCyclesPerFlit = 1; break; // CB-(VCT)
    }

}

void TrafficModelGenerator::generateVariableRate() throw (const char*) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::generateVariableRate" << std::endl;
#endif

    unsigned int xSize = sp->getXSize();
    unsigned int ySize = sp->getYSize();
    unsigned int zSize = sp->getZSize();

    for( unsigned int x = 0; x < xSize; x++ ) {
        for(unsigned int y = 0; y < ySize; y++) {
            for(unsigned int z = 0; z < zSize; z++) {
                Node* no = gpt->getNode(COORDINATE_3D_TO_ID(x,y,z,
                                                            xSize,
                                                            ySize));
                if(no != NULL) {
                    for(unsigned int p = 0; p < MAX_PATTERNS; p++) {
                        if(no->isPatternActive(p)) {
                            TrafficParameters* tp = no->getTrafficPattern(p);
                            // It generates the variable rates
                            if( tp->getInjectionType() != 0 /*CONSTANT*/  ) {
                                // It fills required_bw_array
                                for(unsigned int i = 0; i < 100; i++) {
                                    this->requiredBwArray[i] = ((float)i+1)/100.0f; // required_bw_array[i+1] + 0.01
                                }

                                // It fills pck_2send_array by using a probability function
                                requiredBw = tp->getRequiredBandwidth() / sp->getChannelBandwidth();

                                if(requiredBw > 1.0) {
                                    requiredBw = 1.0;
                                }
                                int resFunProb = this->functionProbability(tp);
                                           //  != ERROR |
                                if( resFunProb != 0 ) {
                                    char *error = new char[512];
                                    sprintf(error,"[probability_function] WARNING: No packet was inserted! Input variable \"pck_2send\" (=%d) is too small for the selected function of probability.",resFunProb);
                                    throw error;
                                }
                                for(unsigned int z = 0; z < 100; z++) {
                                    tp->requiredBandwidthArray[z] = this->requiredBwArray[z];
                                    tp->packageToSendArray[z] = this->pck2SendArray[z];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

}

void TrafficModelGenerator::generateSingleDestinationDistributions(TrafficParameters *tp) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::generateSingleDestinationDistributions" << std::endl;
#endif

    unsigned int numberOfDestinations;
    unsigned int numberRatesWithNoPackets = 0;

    unsigned int spatialDistribution = tp->getSpatialDistribution();

    unsigned int injType = tp->getInjectionType();


    //                      < UNIFORM
    if( spatialDistribution < 6 ) { // != UNIFORM, NON-UNIFORM or LOCAL
        // It determines the destination address for each flow
        if(spatialDistribution != 0) { // != SPECIFIED_ADDRESS
            this->getDestination(tp);
        }

        // It determines the number of destinations for the traffic generator
        if( (tp->getDestinationNodeX() == tp->getSourceNodeX()) &&
            (tp->getDestinationNodeY() == tp->getSourceNodeY()) &&
            (tp->getDestinationNodeZ() == tp->getSourceNodeZ()) ) {
            numberOfDestinations = 0;
        } else {
            numberOfDestinations = 1;
        }

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

        unsigned int numberFlows;
        // It calculates the number of flows
        if(injType == 0) { // == CONSTANT
            numberFlows = numberOfDestinations;
        } else {
            if( tp->getProbabilityFunction() == 2 ) { // == PARETO
                numberFlows = numberOfDestinations;
            } else {
                numberFlows = numberOfDestinations * ( tp->getNumberRates() - numberRatesWithNoPackets );
            }
        }

        // It generates the flows
        if( numberFlows != 0 ) {
            //////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////
            /// PART VI.1(a) - CBR TRAFFIC GENERATION FOR SINGLE DESTINATION DISTRIBUTIONS
            //////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////
            if(injType == 0) { // == CONSTANT
                tp->setIdleTime( this->calculateIdleBasedOnPayloadLength(tp->getPayloadLength(),
                                                                         sp->getChannelBandwidth(),
                                                                         tp->getRequiredBandwidth()) );
                // After calculating idle cycles, it determines the actual bandwidth that the
                // the flow will consume, taking into account the header costs. The bandwidth
                // consumed by the flow will be always greater than the one required by the
                // application. Indeed, it will be determined a normalized value, varying from
                // 1 to 100% of the channel bandwidth.
                //required_bw = ((float)  (win.payload_length[x][y][k] + 1) * nb_of_cycles_per_flit) /
                //				((float)(((win.payload_length[x][y][k] + 1) * nb_of_cycles_per_flit) + win.idle[x][y][k]));

                //				required_bw = win.required_bw_bps[x][y][k]/win.channel_bw;
                //				if (required_bw > 1.0) required_bw = 1.0;
                requiredBw = tp->getRequiredBandwidth();
                pck2Send = tp->getPackageToSend();
                this->writeFlow(tp);
            } else {
            //////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////
            /// PART VI.1(b) - VBR TRAFFIC GENERATION FOR SINGLE DESTINATION DISTRIBUTIONS
            //////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////
                if( tp->getProbabilityFunction() == 2 ) { // == PARETO
                    this->requiredBw = 0.0;
                    this->pck2Send = tp->getPackageToSend();
                    this->writeFlow( tp, tp->getInjectionType() );
                } else {
                    for( unsigned int i = 0; i < tp->getNumberRates() ; i++ ) {
                        if( tp->packageToSendArray[i] != 0 ) {
                            unsigned int burstSize = 0;
                            switch( injType ) {
                                case 1: // VAR_IDL_FIX_PCK // It determines the number of idle cycles
                                    tp->setIdleTime(this->calculateIdleBasedOnPayloadLength( tp->getPayloadLength(),
                                                                                             1.0, // channelBW
                                                                                             tp->requiredBandwidthArray[i] ) );
                                    break;
                                case 2: // VAR_PCK_FIX_IDL // It determines the payload length
                                    tp->setPayloadLength( this->calculatePayloadLengthBasedOnIdle( tp->getIdleTime(),
                                                                                                   1.0, // channelBw
                                                                                                   tp->requiredBandwidthArray[i]) );
                                    if(tp->getPayloadLength() < DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH) {
                                        tp->setPayloadLength( DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH );
                                    }
                                    break;
                                case 3: // VAR_PCK_FIX_IAT // It determines the payload length and the number of idle cycles
                                    tp->setPayloadLength( this->calculatePayloadLengthBasedOnInterArrival(tp->getIntervalTime(),
                                                                                                          1.0, // channelBw
                                                                                                          tp->requiredBandwidthArray[i]) );
                                    if( tp->getPayloadLength() < DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH ) {
                                        tp->setPayloadLength( DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH );
                                    }
                                    // Não gerar o fluxo se a condição abaixo for verdadeira
                                    if( tp->getIntervalTime() < (tp->getPayloadLength() * numberCyclesPerFlit) ) {
                                        continue;
                                    }
                                    tp->setIdleTime( tp->getIntervalTime() - (tp->getPayloadLength() * numberCyclesPerFlit) );
                                    break;
                                case 4: // VAR_IAT_FIX_PCK // It determines the inter-arrival time and the number if idle cycles
                                    tp->setIntervalTime( this->calculateInterArrivalBasedOnPayloadLength(tp->getPayloadLength(),
                                                                                                         1.0, // channelBw
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
                            requiredBw = tp->requiredBandwidthArray[i] * sp->getChannelBandwidth();
                            this->pck2Send = tp->packageToSendArray[i];
                            this->writeFlow( tp,0,burstSize );
                        }
                    }
                }
            }
        }

    }

}

void TrafficModelGenerator::generateMultipleDestinationDistributions(TrafficParameters *tp) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::generateMultipleDestinationsDistributions" << std::endl;
#endif

    unsigned int numberOfDestinations = 0;
    unsigned int numberRatesWithNoPackets = 0;

    unsigned int spatialDistribution = tp->getSpatialDistribution();
    unsigned int injType = tp->getInjectionType();


    if( spatialDistribution >= 6 ) { // == UNIFORM, NON-UNIFORM (1 and 2) or LOCAL
        // It determines the number of destinations for the traffic generator
        switch( spatialDistribution ) {
            case 6: // UNIFORM          // Equal to non-uniform distributions
            case 7: // NON-UNIFORM_1
            case 8: // NON_UNIFORM_2
                numberOfDestinations = ( sp->getXSize()*sp->getYSize()*sp->getZSize() - 1 );
                break;
            case 9: { // LOCAL
                unsigned int x = tp->getSourceNodeX();
                unsigned int y = tp->getSourceNodeY();
                //unsigned int z = tp->getSourceNodeZ();
    // TODO: Adicionar vizinhos UP e DOWN. Mudar abordagem para use[N|E|S|W|U|D]
                unsigned int xy;
                if( (x != 0) && (x != (sp->getXSize()-1)) && (y!=0) && (y!=(sp->getYSize()-1)) ) {
                    numberOfDestinations = 4;
                } else {
                    xy = x + y;
                    if( (xy == 0) || (xy==(sp->getXSize()-1)) ||
                        (xy == (sp->getYSize()-1)) ||
                        (xy == (sp->getXSize()+sp->getYSize()-2)) ) {
                        numberOfDestinations = 2;
                    } else {
                        numberOfDestinations = 3;
                    }
                }
                break;
            }
        }
        // For variable injection rates based on the variation of the
        // payload length (except for Pareto-based ones),
        // it removes the packets with no payload
        if( tp->getProbabilityFunction() != 2 ) { // != PARETO
            //     == VAR_PCK_FIX_IDL ||   == VAR_PCK_FIX_IAT
            if( (injType == 2) || (injType == 3) ) {
                this->removeZeroPayloadPacket( tp );
            }

            // It determines how many rates in the distribution has no packets
            // to be sent. They will not be written to the file
            if( injType != 0 ) { // != CONSTANT
                numberRatesWithNoPackets = 0;
                for( unsigned int i = 0; i < tp->getNumberRates(); i++) {
                    if( tp->packageToSendArray[i] == 0 ) {
                        numberRatesWithNoPackets++;
                    }
                }
            }
        }

        unsigned int numberFlows = 0;
        // It calculates the number of flows
        if( injType == 0 ) { // == CONSTANT
            numberFlows = numberOfDestinations;
        } else {
            if( tp->getProbabilityFunction() == 2) { // == PARETO
                numberFlows = numberOfDestinations;
            } else {
                numberFlows = numberOfDestinations * (tp->getNumberRates() - numberRatesWithNoPackets);
            }
        }


        // It generates the flows
        if( numberFlows != 0 ) {
            ////////////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////////////
            /// PART VI.2(a) - CBR TRAFFIC GENERATION FOR MULTIPLE DESTINATION DISTRIBUTIONS
            ////////////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////////////
            if( injType == 0 ) { // == CONSTANT
                tp->setIdleTime( this->calculateIdleBasedOnPayloadLength( tp->getPayloadLength(),
                                                                          sp->getChannelBandwidth(),
                                                                          tp->getRequiredBandwidth()) );

                for( unsigned int zDest = 0; zDest < sp->getZSize(); zDest++ ) {
                    for( unsigned int yDest = 0; yDest < sp->getYSize(); yDest++ ) {
                        for( unsigned int xDest = 0; xDest < sp->getXSize(); xDest++ ) {
                            bool destOk= false;
                            // It determines if the destination node is OK
                            switch( spatialDistribution ) {
                                case 6: // UNIFORM
                                case 7: // NON_UNIFORM_1
                                case 8: // NON_UNIFORM_2
                                    if( (xDest != tp->getSourceNodeX()) ||
                                        (yDest != tp->getSourceNodeY()) ||
                                        (zDest != tp->getSourceNodeZ()) ) {
                                        destOk = true;
                                    } else {
                                        destOk = false;
                                    }
                                    break;
                                case 9: // LOCAL
                                    if( (abs( long(xDest-tp->getSourceNodeX()) ) + abs( long(yDest-tp->getSourceNodeY()) ) ) == 1) {
                                        destOk = true;
                                    } else {
                                        destOk = false;
                                    }
                                    break;
                            }

                            if(destOk) {
                                tp->setDestinationNodeX(xDest);
                                tp->setDestinationNodeY(yDest);
                                tp->setDestinationNodeZ(zDest);
                                unsigned int xy;
                                unsigned long int pck2SendNonUniform;
                                switch ( spatialDistribution ) {
                                // Non-uniform distribution type 1: the number of packets to be sent decreases with
                                // the distance between the source and the destination nodes
                                    case 7 : // NON_UNIFORM_1
                                        xy = abs( long(xDest-tp->getSourceNodeX()) ) + abs( long(yDest-tp->getSourceNodeY()) );
                                        pck2SendNonUniform = (unsigned long int) ( tp->getPackageToSend() / pow(2,xy-1));
                                        pck2Send = pck2SendNonUniform;
                                        break;
                                // Non-uniform distribution type 2: the number of packets to be sent to the nodes outside of the
                                // neighbourhood is the half of the number of packets to be sent to the neighbour nodes
                                    case 8: // NON_UNIFORM_2
                                        xy = abs( long(xDest-tp->getSourceNodeX()) ) + abs( long(yDest-tp->getSourceNodeY()) );
                                        if (xy == 1) {
                                            pck2SendNonUniform = tp->getPackageToSend();
                                        } else {
                                            pck2SendNonUniform = (unsigned long int) (tp->getPackageToSend() / 2);
                                        }

                                        pck2Send = pck2SendNonUniform;
                                        break;

                                // For Uniform and Local, the number of packets is the same to all the destination nodes
                                    default :
                                        pck2Send = tp->getPackageToSend();
                                        break;
                                }
                                requiredBw = tp->getRequiredBandwidth();
                                this->writeFlow(tp);
                            }

                        }
                    }
                }
            } else {
            ////////////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////////////
            /// PART VI.2(b) - VBR TRAFFIC GENERATION FOR MULTIPLE DESTINATION DISTRIBUTIONS
            ////////////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////////////
                if( tp->getProbabilityFunction() == 2 ) { // == PARETO
                    for( unsigned int zDest = 0; zDest < sp->getZSize(); zDest++ ) {
                        for( unsigned int yDest = 0; yDest < sp->getYSize(); yDest++ ) {
                            for( unsigned int xDest = 0; xDest < sp->getXSize(); xDest++ ) {
                                tp->setDestinationNodeX(xDest);
                                tp->setDestinationNodeY(yDest);
                                tp->setDestinationNodeZ(zDest);
                                if( (xDest != tp->getSourceNodeX()) ||
                                    (yDest != tp->getSourceNodeY()) ||
                                    (zDest != tp->getSourceNodeZ()) ) {
                                    this->requiredBw = 0.0;
                                    this->pck2Send = tp->getPackageToSend();
                                    this->writeFlow(tp,tp->getInjectionType());
                                }
                            }
                        }
                    }
                } else { // NORMAL and EXPONENTIAL
                    for(unsigned int zDest = 0; zDest < sp->getZSize(); zDest++) {
                        for( unsigned int yDest = 0; yDest < sp->getYSize(); yDest++ ) {
                            for( unsigned int xDest = 0; xDest < sp->getXSize(); xDest++ ) {
                                bool destOk = false;
                                // It determines if the destination node is OK
                                switch (spatialDistribution) {
                                    case 6: // UNIFORM
                                    case 7: // NON_UNIFORM_1
                                    case 8: // NON_UNIFORM_2:
                                        if ((xDest!=tp->getSourceNodeX()) ||
                                            (yDest!=tp->getSourceNodeY()) ||
                                            (zDest!=tp->getSourceNodeZ()) ) {
                                            destOk = true;
                                        } else {
                                            destOk = false;
                                        }
                                        break;
                                    case 9: // LOCAL
                                        if ((abs( long(xDest-tp->getSourceNodeX()))+abs(long(yDest-tp->getSourceNodeY()))) == 1) {
                                            destOk = true;
                                        } else {
                                            destOk = false;
                                        }
                                        break;
                                }

                                if (destOk) {
                                    tp->setDestinationNodeX(xDest);
                                    tp->setDestinationNodeY(yDest);
                                    tp->setDestinationNodeZ(zDest);
                                    for (unsigned int i = 0; i < tp->getNumberRates(); i++) {
                                        if (tp->packageToSendArray[i] != 0) {
                                            unsigned int burstSize = 0;
                                            switch(injType) {
                                                case 1: // VAR_IDL_FIX_PCK  // It determines the number of idle cycles
                                                    tp->setIdleTime(this->calculateIdleBasedOnPayloadLength( tp->getPayloadLength(),
                                                                                                             1.0, // channelBw
                                                                                                             tp->requiredBandwidthArray[i]) );
                                                    break;
                                                case 2: // VAR_PCK_FIX_IDL:  // It determines the payload length
                                                    tp->setPayloadLength( this->calculatePayloadLengthBasedOnIdle(
                                                                              tp->getIdleTime(),
                                                                              1.0, // channelBw
                                                                              tp->requiredBandwidthArray[i]) );

                                                    if (tp->getPayloadLength() < DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH) {
                                                        tp->setPayloadLength( DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH );
                                                    }
                                                    break;
                                                case 3: // VAR_PCK_FIX_IAT:  // It determines the payload length and the number of idle cycles
                                                    tp->setPayloadLength( this->calculatePayloadLengthBasedOnInterArrival(
                                                                              tp->getIntervalTime(),
                                                                              1.0, // channelBw
                                                                              tp->requiredBandwidthArray[i]) );

                                                    if (tp->getPayloadLength() < DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH) {
                                                        tp->setPayloadLength( DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH );
                                                    }

                                                    // Não gerar o fluxo se a condição abaixo for verdadeira
                                                    if( tp->getIntervalTime() < (tp->getPayloadLength() * numberCyclesPerFlit) ) {
                                                        continue;
                                                    }

                                                    tp->setIdleTime( tp->getIntervalTime() - (tp->getPayloadLength() * numberCyclesPerFlit) );
                                                    break;
                                                case 4: //  VAR_IAT_FIX_PCK:  // It determines the inter-arrival time and the number of idle cycles
                                                    tp->setIntervalTime( this->calculateInterArrivalBasedOnPayloadLength(
                                                                             tp->getPayloadLength(),
                                                                             1.0, // ChannelBw
                                                                             tp->requiredBandwidthArray[i]) );

                                                    if( tp->getIntervalTime() < (tp->getPayloadLength() * numberCyclesPerFlit) ) {
                                                        // Não gera fluxo
                                                        continue;
                                                    }
                                                    tp->setIdleTime( tp->getIntervalTime() - (tp->getPayloadLength()*numberCyclesPerFlit) );
                                                    break;
                                                case 5: // VAR_BST_FIX_IAT:
                                                    // It determines the burst size and the size of the last packet
                                                    // It determines the burst size. If it does not have a decimal part, then
                                                    // last_payload_lengh = payload_length. If it has a decimal part, then the
                                                    // burst size is incremented by one packet and the length of this last
                                                    // packet is determined. Depending on the required bandwidth, the length of
                                                    // the payload of the last packet can equal 0, 1 or a number <= payload_length
                                                    float tmp = ((tp->requiredBandwidthArray[i]*tp->getIntervalTime()) /
                                                                 ((float)((tp->getPayloadLength()) * numberCyclesPerFlit)));

                                                    if (((unsigned int)(100*tmp)%100) == 0) { // If decimal part is 0
                                                        burstSize = (unsigned int) tmp;
                                                        lastPayloadLength = tp->getPayloadLength();
                                                    } else {
                                                        burstSize = (unsigned int) (roundf((float)(tmp+0.5)));
                                                        tmp = (trunc((fmod( tp->requiredBandwidthArray[i]*tp->getIntervalTime(),
                                                                           (float) (tp->getPayloadLength()) * numberCyclesPerFlit))/
                                                                           numberCyclesPerFlit));
                                                        if (tmp < 1) {
                                                            lastPayloadLength = 0;
                                                        } else {
                                                            if (tmp < DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH) {
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

                                            unsigned long int pck2SendNonUnif;
                                            unsigned int xy;
                                            switch (spatialDistribution) {
                                            // Non-uniform distribution type 1: the number of packets to be sent decreases with
                                            // the distance between the source and the destination nodes
                                                case 7: // NON_UNIFORM_1 :
                                                    xy = abs(long(xDest-tp->getSourceNodeX())) + abs(long(yDest-tp->getSourceNodeY()));
                                                    pck2SendNonUnif = (unsigned long int) (tp->packageToSendArray[i] / pow(2,xy-1));

                                                    pck2Send = pck2SendNonUnif;
                                                    break;
                                            // Non-uniform distribution type 2: the number of packets to be sent to the nodes outside of the
                                            // neighbourhood is the half of the number of packets to be sent to the neighbour nodes
                                                case 8: // NON_UNIFORM_2 :
                                                    xy = abs(long(xDest-tp->getSourceNodeX())) + abs(long(yDest-tp->getSourceNodeY()));
                                                    if (xy==1) {
                                                        pck2SendNonUnif = tp->packageToSendArray[i];
                                                    } else {
                                                        pck2SendNonUnif = (unsigned long int) (tp->packageToSendArray[i] / 2);
                                                    }
                                                    pck2Send = pck2SendNonUnif;
                                                    break;
                                            // For Uniform and Local, the number of packets is the same to all the destination nodes
                                                default :
                                                    pck2Send = tp->packageToSendArray[i];
                                                    break;
                                            }
                                            requiredBw = tp->requiredBandwidthArray[i]*sp->getChannelBandwidth();
                                            this->writeFlow(tp,0,burstSize);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

}

void TrafficModelGenerator::generateTrafficModel() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::generateTrafficModel" << std::endl;
#endif

    unsigned int xSize = this->sp->getXSize();
    unsigned int ySize = this->sp->getYSize();
    unsigned int zSize = this->sp->getZSize();

    for(unsigned int x = 0; x < xSize; x++) {
        for(unsigned int y = 0; y < ySize; y++) {
            for(unsigned int z = 0; z < zSize; z++) {
                Node* nodo = this->gpt->getNode(COORDINATE_3D_TO_ID(x,y,z,
                                                                    xSize,
                                                                    ySize));
                this->nodeTemp = nodo;
                if(nodo != NULL) {
                    for( unsigned int k = 0; k < MAX_PATTERNS; k++ ) {
                        if( nodo->isPatternActive(k) ) {
                            TrafficParameters* tp = nodo->getTrafficPattern(k);
                            this->numTrafficPatter = k;
                            //////////////////////////////////////////////////////////////////////////////
                            //////////////////////////////////////////////////////////////////////////////
                            /// PART VI.1 - TRAFFIC GENERATION FOR SINGLE DESTINATION DISTRIBUTIONS
                            //////////////////////////////////////////////////////////////////////////////
                            //////////////////////////////////////////////////////////////////////////////
                            this->generateSingleDestinationDistributions(tp);
                            //////////////////////////////////////////////////////////////////////////////
                            //////////////////////////////////////////////////////////////////////////////
                            /// PART VI.2 - TRAFFIC GENERATION FOR MULTIPLE DESTINATION DISTRIBUTIONS
                            //////////////////////////////////////////////////////////////////////////////
                            //////////////////////////////////////////////////////////////////////////////
                            this->generateMultipleDestinationDistributions(tp);
                        }
                    }
                }
            }
        }
    }

}

void TrafficModelGenerator::generateTraffic(const char *diretorio) throw (const char*) {
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
    this->validateTrafficAddresses();

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
    this->determineFlowControl();

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

void TrafficModelGenerator::createFileTrafficConfiguration(const char *diretorio) throw (const char*) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::createFileTrafficConfiguration" << std::endl;
#endif

    FILE* arquivo;

    std::stringstream ss;
    ss << diretorio;
    ss << "/";
    ss << "traffic.tcf";

    std::string arqDir = ss.str();

    if( (arquivo = fopen(arqDir.c_str(),"wt")) == NULL ) {
        char* buffer = new char[512];
        sprintf(buffer,"ERROR: Impossible to create file: %s",arqDir.c_str());
        throw buffer;
    }

    for(unsigned int z = 0; z < sp->getZSize(); z++) {
        for( unsigned int y = 0; y < sp->getYSize(); y++ ) {
            for( unsigned int x = 0; x < sp->getXSize(); x++ ) {
                unsigned int id = COORDINATE_3D_TO_ID(x,y,z,
                                                      sp->getXSize(),
                                                      sp->getYSize());
//                        COORDINATE_TO_ID(x,y,sp->getXSize());
                fprintf(arquivo,"tg_%u\n",id);
                Node* no = gpt->getNode(id);
                if( no != NULL ) {
                    fprintf(arquivo,"%u\n",no->getNumberFlows());
                    fprintf(arquivo,"%s",no->fluxos.c_str());
                } else {
                    fprintf(arquivo,"0\n");
                }

                fprintf(arquivo,"\n\n");

            }
        }
    }
    fprintf(arquivo,"\n\n// Parameters");
    fprintf(arquivo,"\n 0  type");
//    fprintf(arquivo,"\n 1  x_dest");
//    fprintf(arquivo,"\n 2  y_dest");
    fprintf(arquivo,"\n 1  destination");
    fprintf(arquivo,"\n 2  flow_id");
    fprintf(arquivo,"\n 3  traffic_class");
    fprintf(arquivo,"\n 4  switching_type");
    fprintf(arquivo,"\n 5  pck_2send");
    fprintf(arquivo,"\n 6  deadline");
    fprintf(arquivo,"\n 7  required_bw");
    fprintf(arquivo,"\n 8  payload_length");
    fprintf(arquivo,"\n 9  idle_cycles");
    fprintf(arquivo,"\n10  iat");
    fprintf(arquivo,"\n11  burst_size");
    fprintf(arquivo,"\n12  last_payload_length");
    fprintf(arquivo,"\n13  alfa_on");
    fprintf(arquivo,"\n14  alfa_off");

    fclose(arquivo);
}

void TrafficModelGenerator::writeFlow(TrafficParameters *tp, unsigned int type, unsigned int burstSize) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficModelGenerator::writeFlow" << std::endl;
#endif

    char str[512];

    unsigned int id = COORDINATE_3D_TO_ID(tp->getDestinationNodeX(),
                                          tp->getDestinationNodeY(),
                                          tp->getDestinationNodeZ(),
                                          sp->getXSize(),
                                          sp->getYSize());
//            COORDINATE_TO_ID(tp->getDestinationNodeX(),tp->getDestinationNodeY(),sp->getXSize());

    sprintf(str,"%u  %u  %u  %u  %u  %lu  %lu  %.4f  %u  %u  %u  %u  %u  %.2f  %.2f  \n",
            type,                       // Type = 0 when not pareto distribution, TypeOfInjection when pareto distribution
//            tp->getDestinationNodeX(),  // x_dest
//            tp->getDestinationNodeY(),  // y_dest
            id,
            this->numTrafficPatter,     // flow_id
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

    std::stringstream ss;
    ss << str;
    nodeTemp->fluxos.append(ss.str());
    nodeTemp->incrementNumberFlows();
}


TrafficModelGenerator::~TrafficModelGenerator() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Model/TrafficPattern/TrafficModelGenerator" << std::endl;
#endif
    this->gpt->clear();
    delete this->gpt;
}
