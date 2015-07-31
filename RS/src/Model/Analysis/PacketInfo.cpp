/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* PacketInfo.cpp
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

#include "include/Model/Analysis/PacketInfo.h"

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
    this->xDestination = 0;
    this->yDestination = 0;
    this->xSource = 0;
    this->ySource = 0;
    this->flowId = 0;
    this->trafficClass = 0;
    this->deadline = 0;
    this->cycleCreationPacket = 0;
    this->cycleReceivedHeader = 0;
    this->cycleReceivedTrailer = 0;
    this->packetSize = 0;
    this->requiredBandwidth = 0;

}

PacketInfo::PacketInfo(unsigned long receivedPacketId, unsigned int xDestination,
        unsigned int yDestination,unsigned int xSource, unsigned int ySource,
        unsigned int flowId, unsigned int trafficClass, unsigned long deadline,
        unsigned long long cycleCreationPacket, unsigned long long cycleReceivedHeader,
        unsigned long long cycleReceivedTrailer, unsigned int packetSize, float requiredBandwidth)
        : receivedPacketId(receivedPacketId),xDestination(xDestination),yDestination(yDestination),
          xSource(xSource),ySource(ySource),flowId(flowId),trafficClass(trafficClass),deadline(deadline),
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

    return (abs(xDestination-xSource) + abs(yDestination-ySource) + 1);

}

unsigned int PacketInfo::getNumberOfLinksInPath() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/PacketInfo::getNumberOfLinksInPath" << std::endl;
#endif
    return (abs(xDestination-xSource) + abs(yDestination-ySource) + 2);

}

PacketInfo::~PacketInfo() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Model/Analysis/PacketInfo" << std::endl;
#endif

}
