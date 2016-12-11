/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* PacketInfo.cpp
* Copyright (C) 2014 - 2017 LEDS - Univali <zeferino@univali.br>
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
* 01/12/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*/

#include "Model/Analysis/PacketInfo.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

PacketInfo::PacketInfo() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/Analysis/PacketInfo(default)" << std::endl;
#endif
    this->receivedPacketId = 0;
    this->source = 0;
    this->destination= 0;
    this->hops = 0;
    this->flowId = 0;
    this->trafficClass = 0;
    this->deadline = 0;
    this->cycleCreationPacket = 0;
    this->cycleReceivedHeader = 0;
    this->cycleReceivedTrailer = 0;
    this->packetSize = 0;
    this->requiredBandwidth = 0;

}

PacketInfo::PacketInfo(unsigned long receivedPacketId,
                       unsigned int source,unsigned int destination, unsigned short hops,
                       unsigned int flowId, unsigned int trafficClass, unsigned long deadline,
                       unsigned long long cycleCreationPacket, unsigned long long cycleReceivedHeader,
                       unsigned long long cycleReceivedTrailer, unsigned int packetSize, float requiredBandwidth)
        : receivedPacketId(receivedPacketId),source(source),destination(destination),
          hops(hops),flowId(flowId),trafficClass(trafficClass),deadline(deadline),
          cycleCreationPacket(cycleCreationPacket),cycleReceivedHeader(cycleReceivedHeader),
          cycleReceivedTrailer(cycleReceivedTrailer),packetSize(packetSize),requiredBandwidth(requiredBandwidth)
{
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/Analysis/PacketInfo(...)" << std::endl;
#endif
}

unsigned int PacketInfo::getNumberOfRoutersInPath() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/PacketInfo::getNumberOfRoutersInPath" << std::endl;
#endif

    return hops;

}

unsigned int PacketInfo::getNumberOfLinksInPath() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/PacketInfo::getNumberOfLinksInPath" << std::endl;
#endif
    return hops+1;

}

PacketInfo::~PacketInfo() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Model/Analysis/PacketInfo" << std::endl;
#endif

}
#include <iostream>
void PacketInfo::printInfo() {

    std::cout << "\n\nPck ID      : " << receivedPacketId
              << "\nSource        : " << source
              << "\nDestination   : " << destination
              << "\nHops          : " << hops
              << "\nFlowId        : " << flowId
              << "\nClass         : " << trafficClass
              << "\nDeadline      : " << deadline
              << "\nCycle creation: " << cycleCreationPacket
              << "\nHeader at     : " << cycleReceivedHeader
              << "\nTrailer at    : " << cycleReceivedTrailer
              << "\nPck Size      : " << packetSize
              << "\nReq. BW       : " << requiredBandwidth;
}
