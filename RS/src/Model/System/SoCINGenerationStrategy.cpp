/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* SoCINGenerationStrategy.cpp
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

#include "include/Model/System/SoCINGenerationStrategy.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

void SoCINGenerationStrategy::generateParameters() throw(const char* ) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SoCINGenerationStrategy::generateParameters" << std::endl;
#endif

    unsigned int rib_width = this->systemParameters->getDestinationAddressWidth();// Width of the destination address

    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    // It creates the parameters file for SoCIN  (parameters.h)
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    if ((this->fileOut=fopen("system/parameters.h","wt")) == NULL){
        printf("\n\tERROR: Cannot open the file!!!\n\n");
        throw "Cannot open the file system/parameters.h for write!!!";
    }

    //////////////////////////////////////////////////////////////////////////////
    // It prints a header
    //////////////////////////////////////////////////////////////////////////////
    fprintf(this->fileOut,"/*");
    fprintf(this->fileOut,"\n--------------------------------------------------------------------------------");
    fprintf(this->fileOut,"\nPROJECT: ParIS");
    fprintf(this->fileOut,"\nFILE   : parameters.h");
    fprintf(this->fileOut,"\n--------------------------------------------------------------------------------");
    fprintf(this->fileOut,"\nDESCRIPTION: This file includes the definition of the general parameters for");
    fprintf(this->fileOut,"\nnotwrok generation. It also includes a list of definitions for constants used by");
    fprintf(this->fileOut,"\nsome modules. Furthermore, they are available debug switches used to enable ");
    fprintf(this->fileOut,"\nthe exhibition of debug messages (see last section).");
    fprintf(this->fileOut,"\n--------------------------------------------------------------------------------");
    fprintf(this->fileOut,"\nAUTHORS: Laboratory of Embedded and Distributed Systems (LEDS - UNIVALI)");
    fprintf(this->fileOut,"\nCONTACT: Prof. Cesar Zeferino (zeferino@univali.br)");
    fprintf(this->fileOut,"\n--------------------------------------------------------------------------------");
    fprintf(this->fileOut,"\nLast update on: October 29th, 2006.");
    fprintf(this->fileOut,"\n--------------------------------------------------------------------------------");
    fprintf(this->fileOut,"\n*/");

    //////////////////////////////////////////////////////////////////////////////
    // It prints the defines
    //////////////////////////////////////////////////////////////////////////////
    fprintf(this->fileOut,"\n#ifndef __PARAMS_H__");
    fprintf(this->fileOut,"\n#define __PARAMS_H__");
    fprintf(this->fileOut,"\n");
    fprintf(this->fileOut,"\n#define SIM // Used to isolate non-synthesizable code");

      switch (this->systemParameters->getSafOption()) {
          case 0 :  break;
          case 1 :
          case 2 :
          case 3 :  fprintf(this->fileOut,"\n#define PROBE_SWITCHING_ACTIVITY // Used to measure the switching activity of the registers\n");
                    break;
          default:  break;
      }

    fprintf(this->fileOut,"\n////////////////////////////// ROUTER PARAMETERS ///////////////////////////////");
    fprintf(this->fileOut,"\n// MECHANISMS TYPES");
    fprintf(this->fileOut,"\n#define ROUTING_TYPE        %u", this->experimento->getRoutingAlgorithm());
    fprintf(this->fileOut,"\n#define FC_TYPE             %u", this->experimento->getFlowControl());
    fprintf(this->fileOut,"\n#define ARBITER_TYPE        %u", this->experimento->getArbiterType());

    fprintf(this->fileOut,"\n#define SWITCH_TYPE         0");

    fprintf(this->fileOut,"\n");

    fprintf(this->fileOut,"\n// PACKET FORMAT");
    fprintf(this->fileOut,"\n#define FLIT_WIDTH          %u \t// Width of the flit", this->systemParameters->getPhitWidth());
    fprintf(this->fileOut,"\n#define SIDEBAND_WIDTH      %u \t// Width of the sideband field(framing bits)", this->systemParameters->getSidebandWidth());
    fprintf(this->fileOut,"\n#define DATA_WIDTH          %u \t// Width of the data field", this->systemParameters->getDataWidth());
    fprintf(this->fileOut,"\n#define RIB_WIDTH           %u \t// Width of the addressing field (RIB) in the header", rib_width);
    fprintf(this->fileOut,"\n#define FLIT_TYPE_WIDTH     2  \t// Width of the header field that specifies the flit type");


    fprintf(this->fileOut,"\n#define HEADER_LENGTH       1\t// Length of the header");
    fprintf(this->fileOut,"\n#define PAYLOAD_FRAME       0\t// Frame for payload flits");
    fprintf(this->fileOut,"\n#define HEADER_FRAME        1\t// Frame for header  flits");
    fprintf(this->fileOut,"\n#define TRAILER_FRAME       2\t// Frame for trailer flits");


    fprintf(this->fileOut,"\n\n// MASKS FOR EXTRACTION OF FIELDS FROM A PACKET");
    fprintf(this->fileOut,"\n#define DATA_MASK           0x%llX", ((unsigned long long int)(pow(2,this->systemParameters->getDataWidth())-1)) );
    fprintf(this->fileOut,"\n#define X_SRC_MASK          0x%X",((unsigned int)(pow(2,rib_width/2)-1) << rib_width) << rib_width/2);
    fprintf(this->fileOut,"\n#define Y_SRC_MASK          0x%X", (unsigned int)(pow(2,rib_width/2)-1) << rib_width);
    fprintf(this->fileOut,"\n#define X_DEST_MASK         0x%X", (unsigned int)(pow(2,rib_width/2)-1) << rib_width/2);
    fprintf(this->fileOut,"\n#define Y_DEST_MASK         0x%X", (unsigned int)(pow(2,rib_width/2)-1));

    fprintf(this->fileOut,"\n\n// BUFFERING");
    fprintf(this->fileOut,"\n#define FIFO_WIDTH          %u \t// Width of the buffer words (= FLIT_WIDTH)", this->systemParameters->getPhitWidth());
    fprintf(this->fileOut,"\n#define FIFO_IN_DEPTH       %u \t// Number of positions of the input buffers", (unsigned int)this->experimento->getInputBufferSize());
    fprintf(this->fileOut,"\n#define FIFO_IN_LOG2_DEPTH  %u \t// Log2 of INPUT_FIFO_DEPTH", (unsigned int) log2((double) this->experimento->getInputBufferSize()));
    fprintf(this->fileOut,"\n#define FIFO_OUT_DEPTH      %u \t// Number of positions of the output buffers", (unsigned int)this->experimento->getOutputBufferSize());
    fprintf(this->fileOut,"\n#define FIFO_OUT_LOG2_DEPTH %u \t// Log2 of OUTPUT_FIFO_DEPTH",(unsigned int) log2((double)this->experimento->getOutputBufferSize()));

    fprintf(this->fileOut,"\n\n// ARBITRATION");
    fprintf(this->fileOut,"\n#define N                   4  \t// Number of requests received by an arbiter");
    fprintf(this->fileOut,"\n#define LOG2_N              2  \t// Log2 of N");

    fprintf(this->fileOut,"\n\n// FLOW CONTROL");
    fprintf(this->fileOut,"\nenum fsm_state {S0,S1,S2};     \t// States of the FSMs of handshake-type ifc and ofc");
    fprintf(this->fileOut,"\n#define CREDIT              %u \t// Number of credits at power up", (unsigned int)this->experimento->getInputBufferSize());
    fprintf(this->fileOut,"\n#define CREDIT_COUNTER_SIZE %u \t// Size of the credit counter", (unsigned int) log2((double) this->experimento->getInputBufferSize()) + 1);

    fprintf(this->fileOut,"\n\n// TRAFFIC CLASSES");
    fprintf(this->fileOut,"\n#define RT0                 0");
    fprintf(this->fileOut,"\n#define RT1                 1");
    fprintf(this->fileOut,"\n#define NRT0                2");
    fprintf(this->fileOut,"\n#define NRT1                3");


    fprintf(this->fileOut,"\n\n// STAMPLER PARAMETERS (block used for aging in XIN_AG");
    fprintf(this->fileOut,"\nenum stampler_fsm_state {STAMPLER_S0, STAMPLER_S1};");
    fprintf(this->fileOut,"\n#define AGE_CLOCK_WIDTH     %u // VHDL Original name: SIZE_REF"  , this->systemParameters->getAgeClockWidth());
    fprintf(this->fileOut,"\n#define AGE_WIDTH           %u // VHDL Original name: SIZE_COUNT", this->systemParameters->getAgeWidth());

    fprintf(this->fileOut,"\n\n// PARIS CHANNEL TYPES");
    fprintf(this->fileOut,"\n#define PARIS_CHANNEL_NULL    0");
    fprintf(this->fileOut,"\n#define PARIS_CHANNEL_DEFAULT 1");
    fprintf(this->fileOut,"\n#define PARIS_CHANNEL_CS      2");
    fprintf(this->fileOut,"\n#define PARIS_CHANNEL_AG      3");

    fprintf(this->fileOut,"\n\n// PARIS CHANNELS SELECTION");

    switch( this->experimento->getRouterArchitecture() ) {
        case 0: // PARIS :
            fprintf(this->fileOut,"\n#define PARIS_VC0_TYPE      PARIS_CHANNEL_DEFAULT");
            fprintf(this->fileOut,"\n#define PARIS_VC1_TYPE      PARIS_CHANNEL_NULL");
            break;

        case 1: // PARIS2 :
            fprintf(this->fileOut,"\n#define PARIS_VC0_TYPE      PARIS_CHANNEL_DEFAULT");
            fprintf(this->fileOut,"\n#define PARIS_VC1_TYPE      PARIS_CHANNEL_DEFAULT");
            break;

        case 2: // PARIS2_AG :
            fprintf(this->fileOut,"\n#define PARIS_VC0_TYPE      PARIS_CHANNEL_AG");
            fprintf(this->fileOut,"\n#define PARIS_VC1_TYPE      PARIS_CHANNEL_DEFAULT");
            break;

        case 3: // PARIS2_AG_AG :
            fprintf(this->fileOut,"\n#define PARIS_VC0_TYPE      PARIS_CHANNEL_AG");
            fprintf(this->fileOut,"\n#define PARIS_VC1_TYPE      PARIS_CHANNEL_AG");
            break;

        default : break;
    }

    fprintf(this->fileOut,"\n\n////////////////////////////////////////////////////////////////////////////////");
    fprintf(this->fileOut,"\n///////////////////////////// CONSTANTS ////////////////////////////////////////");
    fprintf(this->fileOut,"\n////////////////////////////////////////////////////////////////////////////////");

    fprintf(this->fileOut,"\n\n// REQUEST ENCODING");
    fprintf(this->fileOut,"\n#define REQ_L    0x10");
    fprintf(this->fileOut,"\n#define REQ_N    0x08");
    fprintf(this->fileOut,"\n#define REQ_E    0x04");
    fprintf(this->fileOut,"\n#define REQ_S    0x02");
    fprintf(this->fileOut,"\n#define REQ_W    0x01");
    fprintf(this->fileOut,"\n#define REQ_NONE 0x00");

    fprintf(this->fileOut,"\n\n// PORTS IDs");
    fprintf(this->fileOut,"\n#define LOCAL_ID  0");
    fprintf(this->fileOut,"\n#define NORTH_ID  1");
    fprintf(this->fileOut,"\n#define EAST_ID   2");
    fprintf(this->fileOut,"\n#define SOUTH_ID  3");
    fprintf(this->fileOut,"\n#define WEST_ID   4");

    fprintf(this->fileOut,"\n\n////////////////////////////////////////////////////////////////////////////////");
    fprintf(this->fileOut,"\n///////////////////////////// DEBUG CONTROL ////////////////////////////////////");
    fprintf(this->fileOut,"\n////////////////////////////////////////////////////////////////////////////////");
    fprintf(this->fileOut,"\n\n//#define DEBUG_REQ_REG");
    fprintf(this->fileOut,"\n//#define DEBUG_FIFO_IN_DATAPATH");
    fprintf(this->fileOut,"\n//#define DEBUG_FIFO_IN_CONTROLLER");
    fprintf(this->fileOut,"\n//#define DEBUG_IFC_HANDSHAKE");
    fprintf(this->fileOut,"\n//#define DEBUG_IFC_CREDIT");
    fprintf(this->fileOut,"\n//#define DEBUG_FIFO_OUT_DATAPATH");
    fprintf(this->fileOut,"\n//#define DEBUG_FIFO_OUT_CONTROLLER");
    fprintf(this->fileOut,"\n//#define DEBUG_PG ");
    fprintf(this->fileOut,"\n//#define DEBUG_PPE");
    fprintf(this->fileOut,"\n//#define DEBUG_OFC_HANDSHAKE");
    fprintf(this->fileOut,"\n//#define DEBUG_OFC_CREDIT");
    fprintf(this->fileOut,"\n//#define DEBUG_PARIS_INTERCONNECT");
    fprintf(this->fileOut,"\n//#define DEBUG_STOPSIM");
    fprintf(this->fileOut,"\n//#define DEBUG_STAMPLER");
    fprintf(this->fileOut,"\n//#define DEBUG_STAMP_CHECKER");
    fprintf(this->fileOut,"\n//#define DEBUG_CLASS_CHECKER");
    fprintf(this->fileOut,"\n\n  #endif // __PARAMS_H__");

    fprintf(this->fileOut,"\n");
    fclose(this->fileOut);
}
