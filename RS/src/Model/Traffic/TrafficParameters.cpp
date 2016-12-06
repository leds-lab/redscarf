/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* TrafficParameters.cpp
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

#include "include/Model/Traffic/TrafficParameters.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

TrafficParameters::TrafficParameters(bool defaultValues) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/TrafficPattern/TrafficParameters" << std::endl;
#endif

    if( defaultValues ) {
        this->alfaOff                       = DefaultValuesTrafficGeneration::DEFAULT_ALFA_OFF;
        this->alfaOn                        = DefaultValuesTrafficGeneration::DEFAULT_ALFA_ON;
        this->deadline                      = DefaultValuesTrafficGeneration::DEFAULT_DEADLINE;
        this->destinationNodeX              = 0;
        this->destinationNodeY              = 0;
        this->destinationNodeZ              = 0;
        this->idleTime                      = DefaultValuesTrafficGeneration::DEFAULT_IDLE_TIME;
        this->injectionType                 = DefaultValuesTrafficGeneration::DEFAULT_INJECTION_TYPE;
        this->intervalTime                  = DefaultValuesTrafficGeneration::DEFAULT_INTERVAL_TIME;
        this->numberRates                   = DefaultValuesTrafficGeneration::DEFAULT_NUMBER_RATES;
        this->packageToSend                 = DefaultValuesTrafficGeneration::DEFAULT_PACKAGE_TO_SEND;
        this->messageSize                   = 0;
        this->payloadLength                 = DefaultValuesTrafficGeneration::DEFAULT_PAYLOAD_LENGTH;
        this->probabilityFunction           = DefaultValuesTrafficGeneration::DEFAULT_PROBABILITY_FUNCTION;
        this->requiredBandwidth             = DefaultValuesTrafficGeneration::DEFAULT_REQUIRED_BANDWIDTH;
        this->requiredBandwidthStdDeviation = DefaultValuesTrafficGeneration::DEFAULT_REQUIRED_BANDWIDTH_STD_DEVIATION;
        this->sourceNodeX                   = 0;
        this->sourceNodeY                   = 0;
        this->sourceNodeZ                   = 0;
        this->spatialDistribution           = DefaultValuesTrafficGeneration::DEFAULT_SPATIAL_DISTRIBUTION;
        this->switchingTechnique            = DefaultValuesTrafficGeneration::DEFAULT_SWITCHING_TYPE;
        this->trafficClass                  = DefaultValuesTrafficGeneration::DEFAULT_TRAFFIC_CLASS;
    }

}


/**
 * return A string containing the status of the attributes
 */
std::string TrafficParameters::toString() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficParameters::toString" << std::endl;
#endif

    std::stringstream intToString;
    std::string description = "";

    // Assembling string

    intToString << this->sourceNodeX;
    description += "Source node - X coordinate: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->sourceNodeY;
    description += "Source node - Y coordinate: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->sourceNodeZ;
    description += "Source node - Z coordinate: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->spatialDistribution;
    description += "Type of spatial distribution (e.g. Uniform): "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->destinationNodeX;
    description += "Destination node - X coordinate: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->destinationNodeY;
    description += "Destination node - Y coordinate: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->destinationNodeZ;
    description += "Destination node - Z coordinate: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->trafficClass;
    description += "Traffic class: RT0, RT1, nRT0 or nRT1: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->injectionType;
    description += "Injection mode (e.g. Constant rate): "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->switchingTechnique;
    description += "Swithcing mode: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->packageToSend;
    description += "Number of packets to be sent by the flow: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->deadline;
    description += "Deadline to deliver the packet: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->requiredBandwidth;
    description += "Required bandwidth in Mbps: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->payloadLength;
    description += "Number of flits in the payload (including the trailer): "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->messageSize;
    description += "Message Size (bits): "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->idleTime;
    description += "Number of idle cycles between two packets: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->intervalTime;
    description += "Inter-arrival time (in cycles) between two adjacent packets: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->probabilityFunction;
    description += "Type of probailistic function to be used (e.g. Normal): "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->requiredBandwidthStdDeviation;
    description += "Standard deviation for required_bw: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->alfaOn;
    description += "Parameter for Pareto ON-OFF (e.g. 1.9)(on): "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->alfaOff;
    description += "Parameter for Pareto ON-OFF (e.g. 1.9)(off): : "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->numberRates;
    description += "Number of rates for variable injection rate: "+intToString.str();

    return description;


}

/**
 * return true if the object parameter has the same values in its attributes, false otherwise or if it is null
 */

bool TrafficParameters::equals(TrafficParameters* obj) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficParameters::equals" << std::endl;
#endif

    if(obj == NULL) {
        return false;
    }
    if(obj->sourceNodeX                        == this->sourceNodeX                   &&
            obj->sourceNodeY                   == this->sourceNodeY                   &&
            obj->sourceNodeZ                   == this->sourceNodeZ                   &&
            obj->spatialDistribution           == this->spatialDistribution           &&
            obj->destinationNodeX              == this->destinationNodeX              &&
            obj->destinationNodeY              == this->destinationNodeY              &&
            obj->destinationNodeZ              == this->destinationNodeZ              &&
            obj->trafficClass                  == this->trafficClass                  &&
            obj->injectionType                 == this->injectionType                 &&
            obj->switchingTechnique            == this->switchingTechnique            &&
            obj->packageToSend                 == this->packageToSend                 &&
            obj->deadline                      == this->deadline                      &&
            obj->requiredBandwidth             == this->requiredBandwidth             &&
            obj->payloadLength                 == this->payloadLength                 &&
            obj->messageSize                   == this->messageSize                   &&
            obj->idleTime                      == this->idleTime                      &&
            obj->intervalTime                  == this->intervalTime                  &&
            obj->probabilityFunction           == this->probabilityFunction           &&
            obj->requiredBandwidthStdDeviation == this->requiredBandwidthStdDeviation &&
            obj->alfaOn                        == this->alfaOn                        &&
            obj->alfaOff                       == this->alfaOff                       &&
            obj->numberRates                   == this->numberRates
            ) {
        return true;
    }

    return false;

}
