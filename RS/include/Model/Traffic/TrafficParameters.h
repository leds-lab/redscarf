/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* TrafficParameters.h
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

#ifndef TRAFFICPARAMETERS_H
#define TRAFFICPARAMETERS_H

#include <sstream>

#include "include/Model/System/Defines.h"

class TrafficParameters {

private:

    unsigned int        sourceNodeX                  ;  // Source node - X coordinate
    unsigned int        sourceNodeY                  ;  // Source node - Y coordinate
    unsigned int        sourceNodeZ                  ;  // Source node - Z coordinate
    unsigned int        spatialDistribution          ;  // Type of spatial distribution (e.g. Uniform)
    unsigned int        destinationNodeX             ;  // Destination node - X coordinate
    unsigned int        destinationNodeY             ;  // Destination node - Y coordinate
    unsigned int        destinationNodeZ             ;  // Destination node - Z coordinate
    unsigned int        trafficClass                 ;  // Traffic class: RT0, RT1, nRT0 or nRT1
    unsigned int        injectionType                ;  // Injection mode (e.g. Constant rate)
    unsigned int        switchingTechnique           ;  // Swithcing mode
    unsigned long int   packageToSend                ;  // Number of packets to be sent by the flow
    unsigned long int   deadline                     ;  // Deadline to deliver the packet
    float               requiredBandwidth            ;  // Required bandwidth in Mbps
    unsigned int        messageSize                  ;  // Message size
    unsigned int        payloadLength                ;  // Number of flits in the payload (including the trailer)
    unsigned int        idleTime                     ;  // Number of idle cycles between two packets
    unsigned int        intervalTime                 ;  // Inter-arrival time (in cycles) between two adjacent packets
    unsigned int        probabilityFunction          ;  // Type of probailistic function to be used (e.g. Normal)
    float               requiredBandwidthStdDeviation;  // Standard deviation for required_bw
    float               alfaOn                       ;  // Parameter for Pareto ON-OFF (e.g. 1.9)
    float               alfaOff                      ;  // Parameter for Pareto ON-OFF (e.g. 1.25)
    unsigned int        numberRates                  ;  // Number of rates for variable injection rate


public:
    float requiredBandwidthArray[100];
    unsigned long int packageToSendArray[100];

    TrafficParameters(bool defaultValues = true);

    ///////////////// SETs /////////////////
    inline void setSourceNodeX                  (unsigned int sourceX               ) { this->sourceNodeX                   = sourceX                       ; }
    inline void setSourceNodeY                  (unsigned int sourceY               ) { this->sourceNodeY                   = sourceY                       ; }
    inline void setSourceNodeZ                  (unsigned int sourceZ               ) { this->sourceNodeZ                   = sourceZ                       ; }
    inline void setDestinationNodeX             (unsigned int destinationX          ) { this->destinationNodeX              = destinationX                  ; }
    inline void setDestinationNodeY             (unsigned int destinationY          ) { this->destinationNodeY              = destinationY                  ; }
    inline void setDestinationNodeZ             (unsigned int destinationZ          ) { this->destinationNodeZ              = destinationZ                  ; }
    inline void setTrafficClass                 (unsigned int trafficClass          ) { this->trafficClass                  = trafficClass                  ; }
    inline void setInjectionType                (unsigned int injectionType         ) { this->injectionType                 = injectionType                 ; }
    inline void setSwitchingTechnique           (unsigned int switchingTechnique    ) { this->switchingTechnique            = switchingTechnique            ; }
    inline void setMessageSize                  (unsigned int messageSize           ) { this->messageSize                   = messageSize                   ; }
    inline void setPayloadLength                (unsigned int payloadLength         ) { this->payloadLength                 = payloadLength                 ; }
    inline void setIdleTime                     (unsigned int idleTime              ) { this->idleTime                      = idleTime                      ; }
    inline void setIntervalTime                 (unsigned int intervalTime          ) { this->intervalTime                  = intervalTime                  ; }
    inline void setProbabilityFunction          (unsigned int probabilityFunction   ) { this->probabilityFunction           = probabilityFunction           ; }
    inline void setSpatialDistribution          (unsigned int spatialDistribution   ) { this->spatialDistribution           = spatialDistribution           ; }
    inline void setPackageToSend                (unsigned long int packageToSend    ) { this->packageToSend                 = packageToSend                 ; }
    inline void setDeadline                     (unsigned long int deadline         ) { this->deadline                      = deadline                      ; }
    inline void setRequiredBandwidth            (float requiredBandwidth            ) { this->requiredBandwidth             = requiredBandwidth             ; }
    inline void setRequiredBandwidthStdDeviation(float requiredBandwidthStdDeviation) { this->requiredBandwidthStdDeviation = requiredBandwidthStdDeviation ; }
    inline void setAlfaOn                       (float alfaOn                       ) { this->alfaOn                        = alfaOn                        ; }
    inline void setAlfaOff                      (float alfaOff                      ) { this->alfaOff                       = alfaOff                       ; }
    inline void setNumberRates                  (unsigned int numberRates           ) { this->numberRates                   = numberRates                   ; }
    ///////////////// GETs /////////////////
    inline unsigned int      getSourceNodeX                  () const { return this->sourceNodeX                   ; }
    inline unsigned int      getSourceNodeY                  () const { return this->sourceNodeY                   ; }
    inline unsigned int      getSourceNodeZ                  () const { return this->sourceNodeZ                   ; }
    inline unsigned int      getDestinationNodeX             () const { return this->destinationNodeX              ; }
    inline unsigned int      getDestinationNodeY             () const { return this->destinationNodeY              ; }
    inline unsigned int      getDestinationNodeZ             () const { return this->destinationNodeZ              ; }
    inline unsigned int      getTrafficClass                 () const { return this->trafficClass                  ; }
    inline unsigned int      getInjectionType                () const { return this->injectionType                 ; }
    inline unsigned int      getSwitchingTechnique           () const { return this->switchingTechnique            ; }
    inline unsigned int      getMessageSize                  () const { return this->messageSize                   ; }
    inline unsigned int      getPayloadLength                () const { return this->payloadLength                 ; }
    inline unsigned int      getIdleTime                     () const { return this->idleTime                      ; }
    inline unsigned int      getIntervalTime                 () const { return this->intervalTime                  ; }
    inline unsigned int      getProbabilityFunction          () const { return this->probabilityFunction           ; }
    inline unsigned int      getSpatialDistribution          () const { return this->spatialDistribution           ; }
    inline unsigned long int getPackageToSend                () const { return this->packageToSend                 ; }
    inline unsigned long int getDeadline                     () const { return this->deadline                      ; }
    inline float             getRequiredBandwidth            () const { return this->requiredBandwidth             ; }
    inline float             getRequiredBandwidthStdDeviation() const { return this->requiredBandwidthStdDeviation ; }
    inline float             getAlfaOn                       () const { return this->alfaOn                        ; }
    inline float             getAlfaOff                      () const { return this->alfaOff                       ; }
    inline unsigned int      getNumberRates                  () const { return this->numberRates                   ; }

    ///////////////// Debug /////////////////
    std::string toString() const;
    bool equals(TrafficParameters*) const;

};

#endif // TRAFFICPARAMETERS_H
