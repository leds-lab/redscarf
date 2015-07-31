/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* SystemParameters.cpp
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

#include "include/Model/System/SystemParameters.h"

#include <sstream>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

/**
 * Constructor
 * initializes attributes with default values
 */
SystemParameters::SystemParameters(bool defaultValues) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/SystemConfiguration/SystemParameters" << std::endl;
#endif

    if(defaultValues){
    // Initializing attributes with default values defined in the define.h
        this->ageClockWidth           = AGE_CLOCK_WIDTH                              ; // Width of aging counter
        this->ageWidth                = AGE_WIDTH                                    ; // Width of the header's age field
        this->dataWidth               = DefaultValuesSystem::DEFAULT_DATA_WIDTH      ; // Width of the data channel
        this->sidebandWidth           = 2                                            ; // Width of the sideband field (framing)

        this->destinationAddressWidth = DefaultValuesSystem::DEFAULT_RIB_WIDTH       ; // Width of the destination address
        this->phitWidth               = this->dataWidth + this->sidebandWidth        ; // Width of the flit

        this->safOption               = DefaultValuesSystem::DEFAULT_SAF_OPTION      ; // Option for SAF generation
        this->stopOption              = DefaultValuesSystem::DEFAULT_STOP_OPTION     ; // Stop simulation option
        this->stopTime_cycles         = DefaultValuesSystem::DEFAULT_STOP_TIME_CYCLES; // Time to stop simulation in cycles
        this->stopTime_ns             = DefaultValuesSystem::DEFAULT_STOP_TIME_NS    ; // Time to stop simulation in ns
        this->tClk                    = DefaultValuesSystem::DEFAULT_TCLK            ; // Clock cycle
        this->vcdOption               = DefaultValuesSystem::DEFAULT_VCD_OPTION      ; // VCD generation option
        this->xSize                   = DefaultValuesSystem::DEFAULT_X_SIZE          ; // Network dimension in axis x
        this->ySize                   = DefaultValuesSystem::DEFAULT_Y_SIZE          ; // Network dimensions in axis y

        this->fClkFirst    = DefaultValuesSystem::DEFAULT_FCLK_FIRST;
        this->fClkLast     = DefaultValuesSystem::DEFAULT_FCLK_LAST;
        this->fClkStep     = DefaultValuesSystem::DEFAULT_FCLK_STEP_VALUE;
        this->fClkStepType = DefaultValuesSystem::DEFAULT_FCLK_STEP_TYPE;

        this->channelBandwidth = DefaultValuesSystem::DEFAULT_CHANNEL_BW;

    }
}

///////////////// DEBUG /////////////////
/**
 * @brief Build a string for visualization values attributes
 * @return A string containing the status of the attributes
 */
std::string SystemParameters::toString() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemParameters::toString" << std::endl;
#endif

    std::stringstream intToString;
    std::string description = "";

    // Assembly string

    intToString << this->ageClockWidth;
    description += "Width of aging counter: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->ageWidth;
    description += "Width of the header's age field: "+intToString.str()+"\n";

//    intToString.str("");
//    intToString << this->arbiterType;
//    description += "Type of arbiter: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->dataWidth;
    description += "Width of the data channel: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->destinationAddressWidth;
    description += "Width of the destination address: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->phitWidth;
    description += "Width of the flit: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->safOption;
    description += "Option for SAF generation: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->sidebandWidth;
    description += "Width of the sideband field (framing): "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->stopOption;
    description += "Stop simulation option: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->stopTime_cycles;
    description += "Time to stop simulation in cycles: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->stopTime_ns;
    description += "Time to stop simulation in ns: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->tClk;
    description += "Clock cycle: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->vcdOption;
    description += "VCD generation option: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->xSize;
    description += "Network dimension in axis x: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->ySize;
    description += "Network dimensions in axis y: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->fClkFirst;
    description += "FClk First: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->fClkLast;
    description += "FClk Last: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->fClkStep;
    description += "FClk Step: "+intToString.str()+"\n";

    intToString.str("");
    intToString << this->fClkStepType;
    description += "FClk Step Type: "+intToString.str();

    return description;

}

/**
 * return true if the object parameter has the same values ??in its attributes, false otherwise or if it is null
 */
inline bool SystemParameters::equals(SystemParameters *obj) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemParameters::equals" << std::endl;
#endif

    if(obj == NULL) {
        return false;
    }
    if(obj->ageClockWidth                == this->ageClockWidth           &&
            obj->ageWidth                == this->ageWidth                &&
            obj->dataWidth               == this->dataWidth               &&
            obj->destinationAddressWidth == this->destinationAddressWidth &&
            obj->phitWidth               == this->phitWidth               &&
            obj->safOption               == this->safOption               &&
            obj->sidebandWidth           == this->sidebandWidth           &&
            obj->stopOption              == this->stopOption              &&
            obj->stopTime_cycles         == this->stopTime_cycles         &&
            obj->stopTime_ns             == this->stopTime_ns             &&
            obj->tClk                    == this->tClk                    &&
            obj->vcdOption               == this->vcdOption               &&
            obj->xSize                   == this->xSize                   &&
            obj->ySize                   == this->ySize
            ) {
        return true;
    }

    return false;
}
