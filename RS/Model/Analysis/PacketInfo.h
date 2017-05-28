/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* PacketInfo.h
* Copyright (C) 2014-2017 LEDS - Univali <zeferino@univali.br>
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
* 01/12/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*/

#ifndef __PACKETINFO_H__
#define __PACKETINFO_H__

/*!
 * \brief The PacketInfo class is used to store packets information from logs of simulation.
 *
 * The available informations in this class are about the simulated packets,
 * such as: X, Y source and destination, packet id, traffic class, deadline,
 * creation packet cycle, received header and trailer cycles, packet size (payload)
 * and required bandwidth.
 */
class PacketInfo {
protected:
    unsigned long long receivedPacketId;
    unsigned int       source;
    unsigned int       destination;
    unsigned short     hops;
    unsigned int       flowId;
    unsigned int       trafficClass;
    unsigned long long deadline;
    unsigned long long cycleCreationPacket;
    unsigned long long cycleReceivedHeader;
    unsigned long long cycleReceivedTrailer;
    unsigned int       packetSize;           // Payload
    float              requiredBandwidth;

public:
    /*!
     * \brief PacketInfo is default constructor of this class that initializes all attributes with zero
     */
    PacketInfo();
    /*!
     * \brief PacketInfo is secondary constructor that initializes all attributes with your respective argument
     * \param receivedPacketId Packet id
     * \param xDestination X coordinate of the destination
     * \param yDestination Y coordinate of the destination
     * \param xSource X coordinate of the source
     * \param ySource Y coordinate of the source
     * \param flowId The flow id defined on System Configuration tab for the traffic pattern
     * \param trafficClass The traffic class of the packet
     * \param deadline The deadline of the packet
     * \param cycleCreationPacket The creation packet cycle
     * \param cycleReceivedHeader The received header cycle
     * \param cycleReceivedTrailer The received trailer cycle
     * \param packetSize The packet size (only payload)
     * \param requiredBandwidth The required bandwidth of the packet
     */
    PacketInfo(unsigned long int receivedPacketId, unsigned int source, unsigned int destination,
            unsigned short hops, unsigned int flowId, unsigned int trafficClass,
            unsigned long int deadline, unsigned long long cycleCreationPacket, unsigned long long cycleReceivedHeader,
            unsigned long long cycleReceivedTrailer, unsigned int packetSize, float requiredBandwidth);

    // Seters
    inline void setReceivedPacketId     (unsigned long long pckId   ) { this->receivedPacketId    = pckId       ;}
    inline void setDestination          (unsigned int  destination  ) { this->destination         = destination ;}
    inline void setSource               (unsigned int       source  ) { this->source              = source      ;}
    inline void setHops                 (unsigned short     hops    ) { this->hops                = hops        ;}
    inline void setFlowId               (unsigned int       flowId  ) { this->flowId              = flowId      ;}
    inline void setTrafficClass         (unsigned int  trafficClass ) { this->trafficClass        = trafficClass;}
    inline void setDeadline             (unsigned long long deadline) { this->deadline            = deadline    ;}
    inline void setCycleCreationPacket  (unsigned long long cycle   ) { this->cycleCreationPacket = cycle       ;}
    inline void setCycleReceivedHeader  (unsigned long long cycle   ) { this->cycleReceivedHeader = cycle       ;}
    inline void setCycleReceivedTrailer (unsigned long long cycle   ) { this->cycleReceivedTrailer= cycle       ;}
    inline void setPacketSize           (unsigned int       size    ) { this->packetSize          = size        ;}
    inline void setRequiredBandwidth    (float              reqBW   ) { this->requiredBandwidth   = reqBW       ;}

    // Geters
    inline unsigned long long getReceivedPacketId     () { return this->receivedPacketId    ;}
    inline unsigned int       getDestination          () { return this->destination         ;}
    inline unsigned int       getSource               () { return this->source              ;}
    inline unsigned int       getHops                 () { return this->hops                ;}
    inline unsigned int       getFlowId               () { return this->flowId              ;}
    inline unsigned int       getTrafficClass         () { return this->trafficClass        ;}
    inline unsigned long long getDeadline             () { return this->deadline            ;}
    inline unsigned long long getCycleCreationPacket  () { return this->cycleCreationPacket ;}
    inline unsigned long long getCycleReceivedHeader  () { return this->cycleReceivedHeader ;}
    inline unsigned long long getCycleReceivedTrailer () { return this->cycleReceivedTrailer;}
    inline unsigned int       getPacketSize           () { return this->packetSize          ;}
    inline float              getRequiredBandwidth    () { return this->requiredBandwidth   ;}

    /*!
     * \brief getLatency Method used to get latency of packet in cycles where
     * the outcome is the time difference between the arrival of the trailer
     * at the destination and packet creation at source, less one.
     * \return Packet latency in cycles
     */
    virtual inline unsigned long long getLatency() { return this->getCycleReceivedTrailer() - this->getCycleCreationPacket(); }

    /*!
     * \brief getNumberOfRoutersInPath Method to get the number of routers in path
     * between source and destination. It is the same as the number of hops
     * \return Number of routers in path
     */
    virtual unsigned int getNumberOfRoutersInPath();
    /*!
     * \brief getNumberOfLinksInPath Method to get the number of links in path
     * between source and destination. Number of router plus one
     * \return Number of links in path
     */
    virtual unsigned int getNumberOfLinksInPath();

    /*!
     * \brief ~PacketInfo Destructor for deallocate string debug space allocated
     */
    virtual ~PacketInfo();

    void printInfo();
};

#endif // __PACKETINFO_H__
