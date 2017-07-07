/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Experiment.cpp
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
* 22/12/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*/

#include "Model/System/Experiment.h"
#include "Model/System/Defines.h"

#include <sstream>


#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

Experiment::Experiment(bool defaultValues){
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/SystemConfiguration/Experiment" << std::endl;
#endif

    if(defaultValues) {
        this->active = true;
        this->topology = DefaultValuesSystem::DEFAULT_TOPOLOGY;
        this->routingAlgorithm = DefaultValuesSystem::DEFAULT_ROUTING_TYPE;
        this->flowControl = DefaultValuesSystem::DEFAULT_FC_TYPE;
        this->arbiterType = DefaultValuesSystem::DEFAULT_ARBITER_TYPE;
        this->virtualChannelsOption = DefaultValuesSystem::DEFAULT_VC_OPTION;
        this->inputBufferSize = DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH;
        this->outputBufferSize = DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH;
    } else {
        this->active = false;
        this->topology = 0;
        this->routingAlgorithm = 0;
        this->flowControl = 0;
        this->arbiterType = 0;
        this->virtualChannelsOption = 0;
        this->inputBufferSize = 0;
        this->outputBufferSize = 0;
    }
}

std::string Experiment::toString() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/Experiment::toString" << std::endl;
#endif

    std::stringstream valueToString;
    std::string str = "";

    // Assembly string
    valueToString << this->topology;
    str += "Topology: "+valueToString.str()+"\n";

    valueToString.str("");
    valueToString << this->routingAlgorithm;
    str += "Routing algorithm: "+valueToString.str()+"\n";

    valueToString.str("");
    valueToString << this->flowControl;
    str += "Flow Control: "+valueToString.str()+"\n";

    valueToString.str("");
    valueToString << this->arbiterType;
    str += "Arbiter type: "+valueToString.str()+"\n";

    valueToString.str("");
    valueToString << this->virtualChannelsOption;
    str += "Virtual channels: "+valueToString.str()+"\n";

    valueToString.str("");
    valueToString << this->inputBufferSize;
    str += "Input Buffers Size (flits): "+valueToString.str()+"\n";

    valueToString.str("");
    valueToString << this->outputBufferSize;
    str += "Output Buffers Size (flits): "+valueToString.str()+"\n";

    valueToString.str("");
    valueToString << (active ? std::string("true"): std::string("false"));
    str += "isActive: "+valueToString.str();

    return str;

}

bool Experiment::equals(Experiment *exp) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/Experiment::equals" << std::endl;
#endif
    if( this->topology           == exp->topology           &&
        this->routingAlgorithm   == exp->routingAlgorithm   &&
        this->flowControl        == exp->flowControl        &&
        this->arbiterType        == exp->arbiterType        &&
        this->virtualChannelsOption == exp->virtualChannelsOption &&
        this->inputBufferSize    == exp->inputBufferSize    &&
        this->outputBufferSize   == exp->outputBufferSize ) {

        return true;
    }
    return false;
}
