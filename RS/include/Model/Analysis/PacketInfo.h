/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* PacketInfo.h
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

#ifndef PACKETINFO_H
#define PACKETINFO_H

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
//    char* str;

    unsigned long int receivedPacketId;
    unsigned int      xDestination;
    unsigned int      yDestination;
    unsigned int      xSource;
    unsigned int      ySource;
    unsigned int      flowId;
    unsigned int      trafficClass;
    unsigned long long deadline;
    unsigned long long cycleCreationPacket;
    unsigned long long cycleReceivedHeader;
    unsigned long long cycleReceivedTrailer;
    unsigned int      packetSize;           // Payload
    float             requiredBandwidth;

public:
    /*!
     * \brief PacketInfo is default constructor of this class that initializes all attributes with zero
     */
    PacketInfo();
    /*!
     * \brief PacketInfo is secondary constructor that initializes all attributes with your respective argument
     * \param receivedPacketId Packet id of simulated packets
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
    PacketInfo(unsigned long int receivedPacketId, unsigned int xDestination, unsigned int yDestination,
            unsigned int xSource, unsigned int ySource, unsigned int flowId, unsigned int trafficClass,
            unsigned long int deadline, unsigned long long cycleCreationPacket, unsigned long long cycleReceivedHeader,
            unsigned long long cycleReceivedTrailer, unsigned int packetSize, float requiredBandwidth);

    // Seters
    inline void setReceivedPacketId     (unsigned long int pckId   ) { this->receivedPacketId = pckId;              }
    inline void setXDestination         (unsigned int xDest        ) { this->xDestination = xDest;                  }
    inline void setYDestionation        (unsigned int yDest        ) { this->yDestination = yDest;                  }
    inline void setXSource              (unsigned int xSource      ) { this->xSource = xSource;                     }
    inline void setYSource              (unsigned int ySource      ) { this->ySource = ySource;                     }
    inline void setFlowId               (unsigned int flowId       ) { this->flowId = flowId;                       }
    inline void setTrafficClass         (unsigned int trafficClass ) { this->trafficClass = trafficClass;           }
    inline void setDeadline             (unsigned long long deadline) { this->deadline = deadline;                   }
    inline void setCycleCreationPacket  (unsigned long long cycle   ) { this->cycleCreationPacket = cycle;           }
    inline void setCycleReceivedHeader  (unsigned long long cycle   ) { this->cycleReceivedHeader = cycle;           }
    inline void setCycleReceivedTrailer (unsigned long long cycle   ) { this->cycleReceivedTrailer = cycle;          }
    inline void setPacketSize           (unsigned int size         ) { this->packetSize = size;                     }
    inline void setRequiredBandwidth    (float requiredBandwidth   ) { this->requiredBandwidth = requiredBandwidth; }

    // Geters
    inline unsigned long int getReceivedPacketId     () { return this->receivedPacketId;    }
    inline unsigned int      getXDestination         () { return this->xDestination;        }
    inline unsigned int      getYDestionation        () { return this->yDestination;        }
    inline unsigned int      getXSource              () { return this->xSource;             }
    inline unsigned int      getYSource              () { return this->ySource;             }
    inline unsigned int      getFlowId               () { return this->flowId;              }
    inline unsigned int      getTrafficClass         () { return this->trafficClass;        }
    inline unsigned long long getDeadline             () { return this->deadline;            }
    inline unsigned long long getCycleCreationPacket  () { return this->cycleCreationPacket; }
    inline unsigned long long getCycleReceivedHeader  () { return this->cycleReceivedHeader; }
    inline unsigned long long getCycleReceivedTrailer () { return this->cycleReceivedTrailer;}
    inline unsigned int      getPacketSize           () { return this->packetSize;          }
    inline float             getRequiredBandwidth    () { return this->requiredBandwidth;   }

    /*!
     * \brief getLatency Method used to get latency of packet in cycles where
     * the outcome is the time difference between the arrival of the trailer
     * at the destination and packet creation at source, less one.
     * \return Packet latency in cycles
     */
    virtual inline unsigned long int getLatency() { return (unsigned long int)(this->getCycleReceivedTrailer() - this->getCycleCreationPacket() + 1); }

    /*!
     * \brief getNumberOfRoutersInPath Method to get the number of routers in path
     * between source and destination.
     * \return Number of routers in path
     */
    virtual unsigned int getNumberOfRoutersInPath();
    /*!
     * \brief getNumberOfLinksInPath Method to get the number of links in path
     * between source and destination.
     * \return Number of links in path
     */
    virtual unsigned int getNumberOfLinksInPath();

    /*!
     * \brief ~PacketInfo Destructor for deallocate string debug space allocated
     */
    virtual ~PacketInfo();

};

#endif // PACKETINFO_H
