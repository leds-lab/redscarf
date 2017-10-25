/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Defines.h
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
* 20/11/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__ 

// Maximum of Traffic Patterns
#define MAX_PATTERNS        4

// Aging arbiter parameters
#define AGE_CLOCK_WIDTH     2
#define AGE_WIDTH           3

//#define PI                3.1415926535897932384626433832795029

// Convert from router cartesian coordinate in the MESH network to router ID
#define COORDINATE_2D_TO_ID(x,y,X_SIZE) (y * X_SIZE + x)
#define COORDINATE_3D_TO_ID(x,y,z,X_SIZE,Y_SIZE) ( COORDINATE_2D_TO_ID(x,y,X_SIZE) + (X_SIZE * Y_SIZE * z) )
// Convert from network router ID to cartesian coordinate in the MESH
#define ID_TO_COORDINATE_2D_X(id,X_SIZE) (id % X_SIZE)
#define ID_TO_COORDINATE_2D_Y(id,X_SIZE) (id / X_SIZE)
#define ID_TO_COORDINATE_3D_Z(id,X_SIZE,Y_SIZE) (id / (X_SIZE * Y_SIZE))
#define ID_TO_COORDINATE_3D_X(id,X_SIZE,Y_SIZE) ( ID_TO_COORDINATE_2D_X(id - (X_SIZE*Y_SIZE*ID_TO_COORDINATE_3D_Z(id,X_SIZE,Y_SIZE)),X_SIZE) )
#define ID_TO_COORDINATE_3D_Y(id,X_SIZE,Y_SIZE) ( ID_TO_COORDINATE_2D_Y(id - (X_SIZE*Y_SIZE*ID_TO_COORDINATE_3D_Z(id,X_SIZE,Y_SIZE)),X_SIZE) )


class DefaultValuesSystem {

public:
    static const unsigned int DEFAULT_X_SIZE = 2;
    static const unsigned int DEFAULT_Y_SIZE = 2;
    static const unsigned int DEFAULT_Z_SIZE = 1;
    static const unsigned int DEFAULT_DATA_WIDTH = 32;
    static const unsigned int DEFAULT_RIB_WIDTH = 8;
    static const unsigned int DEFAULT_FCLK = 100;
    static const unsigned int DEFAULT_TCLK = 10;
    static const unsigned int DEFAULT_CHANNEL_BW = 3200;

    static const unsigned int DEFAULT_TOPOLOGY = 0;
    static const unsigned int DEFAULT_ROUTING_TYPE = 0;
    static const unsigned int DEFAULT_FC_TYPE = 1;
    static const unsigned int DEFAULT_ARBITER_TYPE = 0;
    static const unsigned int DEFAULT_VC_OPTION = 0;
    static const unsigned int DEFAULT_FIFO_IN_DEPTH = 4;
    static const unsigned int DEFAULT_FIFO_OUT_DEPTH = 0;
    static const bool         DEFAULT_USE_CRYPTOGRAPHY = false;

    static const unsigned int DEFAULT_STOP_OPTION = 0;
    static const unsigned int DEFAULT_SAF_OPTION = 0;
    static const unsigned int DEFAULT_VCD_OPTION = 0;
    static const unsigned int DEFAULT_STOP_TIME_NS = 100;
    static const unsigned int DEFAULT_STOP_TIME_CYCLES = 100;

    static const unsigned int DEFAULT_FCLK_FIRST = 100;
    static const unsigned int DEFAULT_FCLK_LAST = 100;
    static const unsigned int DEFAULT_FCLK_STEP_TYPE = 0;
    static const int DEFAULT_FCLK_STEP_VALUE = -10;
    static const int DEFAULT_FCLK_INC = -1;



};

class DefaultValuesTrafficGeneration {

public:
    static const unsigned int DEFAULT_PACKAGE_TO_SEND = 0;
    static const unsigned int DEFAULT_REQUIRED_BANDWIDTH = 0;
    static const unsigned int DEFAULT_SPATIAL_DISTRIBUTION = 0;      // 0 == Specified Address
    static const unsigned int DEFAULT_TRAFFIC_CLASS = 0;
    static const unsigned int DEFAULT_INJECTION_TYPE = 0;
    static const unsigned int DEFAULT_SWITCHING_TYPE = 0;
    static const unsigned int DEFAULT_PAYLOAD_LENGTH = 0;
    static const unsigned int DEFAULT_DEADLINE = 0;
    static const unsigned int DEFAULT_REQUIRED_BANDWIDTH_BPS = 0;
    static const unsigned int DEFAULT_IDLE_TIME = 0;
    static const unsigned int DEFAULT_INTERVAL_TIME = 0;
    static const unsigned int DEFAULT_PROBABILITY_FUNCTION = 0;
    static const float DEFAULT_REQUIRED_BANDWIDTH_STD_DEVIATION;
    static const float DEFAULT_ALFA_ON;
    static const float DEFAULT_ALFA_OFF;

    static const unsigned int DEFAULT_NUMBER_RATES = 100;

    static const unsigned int MIN_PAYLOAD_LENGTH = 1;       // Bye word on trailer
};


#endif //__DEFINES_H__

