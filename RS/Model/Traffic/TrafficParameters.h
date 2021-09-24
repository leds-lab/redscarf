/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* TrafficParameters.h
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
* ----------------------------------------------------------------------------
* 20/11/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*
*/

#ifndef __TRAFFICPARAMETERS_H__
#define __TRAFFICPARAMETERS_H__

#include <QApplication>

#include "SpatialDistribution.h"

class TrafficParameters {
    Q_DECLARE_TR_FUNCTIONS(TrafficParameters)
private:

    SpatialDistribution::Distribution distribution ;  // Type of spatial distribution (e.g. Uniform)
    unsigned short    source                       ;  // Packet source
    unsigned short    destination                  ;  // Packet destination
    unsigned short    trafficClass                 ;  // Traffic class: RT0, RT1, nRT0 or nRT1
    unsigned short    injectionType                ;  // Injection mode (e.g. Constant rate)
    unsigned short    switchingTechnique           ;  // Swithcing mode
    unsigned short    referenceTopology            ;  // Reference topology for Local and Non-Uniform traffic
    unsigned int      packageToSend                ;  // Number of packets to be sent by the flow
    unsigned long int deadline                     ;  // Deadline to deliver the packet
    float             requiredBandwidth            ;  // Required bandwidth in Mbps
    unsigned int      messageSize                  ;  // Message size
    unsigned int      payloadLength                ;  // Number of flits in the payload (including the trailer)
    unsigned int      idleTime                     ;  // Number of idle cycles between two packets
    unsigned int      intervalTime                 ;  // Interval time (in cycles) between two adjacent packets
    unsigned int      probabilityFunction          ;  // Type of probailistic function to be used (e.g. Normal)
    float             requiredBandwidthStdDeviation;  // Standard deviation for required_bw
    float             alfaOn                       ;  // Parameter for Pareto ON-OFF (e.g. 1.9)
    float             alfaOff                      ;  // Parameter for Pareto ON-OFF (e.g. 1.25)
    unsigned int      numberRates                  ;  // Number of rates for variable injection rate

public:
    float requiredBandwidthArray[100];
    unsigned long int packageToSendArray[100];

    TrafficParameters();

    ///////////////// SETs /////////////////
    inline void setSpatialDistribution(SpatialDistribution::Distribution dist) { this->distribution   = dist               ; }
    void setSpatialDistribution(int index);
    inline void setSource             (unsigned short source             ) { this->source             = source             ; }
    inline void setDestination        (unsigned short destination        ) { this->destination        = destination        ; }
    inline void setTrafficClass       (unsigned short trafficClass       ) { this->trafficClass       = trafficClass       ; }
    inline void setInjectionType      (unsigned short injectionType      ) { this->injectionType      = injectionType      ; }
    inline void setSwitchingTechnique (unsigned short switchingTechnique ) { this->switchingTechnique = switchingTechnique ; }
    inline void setReferenceTopology  (unsigned short referenceTopology  ) { this->referenceTopology  = referenceTopology  ; }
    inline void setMessageSize        (unsigned int   messageSize        ) { this->messageSize        = messageSize        ; }
    inline void setPayloadLength      (unsigned int   payloadLength      ) { this->payloadLength      = payloadLength      ; }
    inline void setIdleTime           (unsigned int   idleTime           ) { this->idleTime           = idleTime           ; }
    inline void setIntervalTime       (unsigned int   intervalTime       ) { this->intervalTime       = intervalTime       ; }
    inline void setProbabilityFunction(unsigned int   probabilityFunction) { this->probabilityFunction= probabilityFunction; }
    inline void setPackageToSend      (unsigned long int packageToSend   ) { this->packageToSend      = packageToSend      ; }
    inline void setDeadline           (unsigned long int deadline        ) { this->deadline           = deadline           ; }
    inline void setRequiredBandwidth  (float requiredBandwidth           ) { this->requiredBandwidth  = requiredBandwidth  ; }
    inline void setAlfaOn             (float alfaOn                      ) { this->alfaOn             = alfaOn             ; }
    inline void setAlfaOff            (float alfaOff                     ) { this->alfaOff            = alfaOff            ; }
    inline void setNumberRates        (unsigned int numberRates          ) { this->numberRates        = numberRates        ; }
    inline void setRequiredBandwidthStdDeviation(float requiredBandwidthStdDeviation)
        { this->requiredBandwidthStdDeviation = requiredBandwidthStdDeviation ; }
    ///////////////// GETs /////////////////
    inline SpatialDistribution::Distribution getSpatialDistribution() const { return this->distribution; }
    inline unsigned short    getSource             () const { return this->source             ; }
    inline unsigned short    getDestination        () const { return this->destination        ; }
    inline unsigned short    getTrafficClass       () const { return this->trafficClass       ; }
    inline unsigned short    getInjectionType      () const { return this->injectionType      ; }
    inline unsigned short    getSwitchingTechnique () const { return this->switchingTechnique ; }
    inline unsigned short    getReferenceTopology  () const { return this->referenceTopology  ; }
    inline unsigned int      getMessageSize        () const { return this->messageSize        ; }
    inline unsigned int      getPayloadLength      () const { return this->payloadLength      ; }
    inline unsigned int      getIdleTime           () const { return this->idleTime           ; }
    inline unsigned int      getIntervalTime       () const { return this->intervalTime       ; }
    inline unsigned int      getProbabilityFunction() const { return this->probabilityFunction; }
    inline unsigned long int getPackageToSend      () const { return this->packageToSend      ; }
    inline unsigned long int getDeadline           () const { return this->deadline           ; }
    inline float             getRequiredBandwidth  () const { return this->requiredBandwidth  ; }
    inline float             getAlfaOn             () const { return this->alfaOn             ; }
    inline float             getAlfaOff            () const { return this->alfaOff            ; }
    inline unsigned int      getNumberRates        () const { return this->numberRates        ; }
    inline float             getRequiredBandwidthStdDeviation() const
        { return this->requiredBandwidthStdDeviation ; }


    static QStringList availableSpatialDistributions();
    QString getSpatialDistributionName(SpatialDistribution::Distribution dist) const;
    QString getSpatialDistributionName(int index) const;
    QString getSpatialDistributionName() const;

    static QStringList availableTrafficClasses();
    QString getTrafficClassName() const;
    QString getTrafficClassName(int index) const;

    static QStringList availableInjectionTypes();
    QString getInjectionTypeName() const;
    QString getInjectionTypeName(int index) const;

    static QStringList availableSwitchingTechniques();
    QString getSwitchingTechniqueName() const;
    QString getSwitchingTechniqueName(int index) const;

    static QStringList availableProbabilityFunctions();
    QString getProbabilityFunctionName(int index) const;
    QString getProbabilityFunctionName() const;

    static QStringList availableReferenceTopologies();
    QString getReferenceTopologyName(int index) const;
    QString getReferenceTopologyName() const;


    int indexOfSpatialDistribution(QString distName) const;
    int indexOfTrafficClass(QString name) const;
    int indexOfInjectionType(QString name) const;
    int indexOfSwitchingTechnique(QString name) const;
    int indexOfProbabilityFunction(QString name) const;
    int indexOfReferenceTopology(QString name) const;

    QString getFormattedString() const;

    QString toString() const;
    bool equals(TrafficParameters*) const;

    bool isValidForSystem(unsigned int numElements);

};

Q_DECLARE_METATYPE( TrafficParameters )

#endif // __TRAFFICPARAMETERS_H__
