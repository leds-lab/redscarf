/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* DataReport.h
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


#ifndef DATAREPORT_H
#define DATAREPORT_H

/*!
 * \brief The DataReport class is used to store latency data from latency reports.
 *
 * All attributes of this class are public to direct access.
 */

class DataReport {
public:

    // Attributes for general report
    float fClk;
    long unsigned int accNbOfPck;
    float avgRequiredBwNorm;
    float acceptedTrafficFlits;
    float idealAvgLatency;
    float avgLatencyCycles;
    float stdevLatency;
    float minLatency;
    float maxLatency;
    float avgRequiredBwBps;
    float acceptedTrafficBps;
    float idealAvgLatencyNs;
    float avgLatencyNs;
    float minLatencyNs;
    float maxLatencyNs;
    float metDeadlinesPer0;
    float metDeadlinesPer1;
    float metDeadlinesPer2;
    float metDeadlinesPer3;
    // Attributes for latency histogram
    long unsigned int latencyCycle;
    long unsigned int packetCount;
    float packetPercentage;

    DataReport();

    void debugGeneral();
    void debugHistogram();

};

#endif // DATAREPORT_H
