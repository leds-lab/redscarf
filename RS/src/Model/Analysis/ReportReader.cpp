/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* ReportReader.cpp
* Copyright (C) 2014-2016 LEDS - Univali <zeferino@univali.br>
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
* 31/05/2016 - 1.1     - Eduardo Alves da Silva      | First refactoring
*    ||      - ||      - Sérgio Vargas Júnior        |      ||
* ----------------------------------------------------------------------------
*
*/

#include "include/Model/Analysis/ReportReader.h"
#include "include/Model/Analysis/DataReport.h"

#include <cstdio>
#include <cstdlib>
#include <clocale>

ReportReader::ReportReader() {}

std::list<DataReport *>* ReportReader::readReport(const char *file) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/ReportReader::readReport" << std::endl;
#endif
    FILE* fpIn;

    std::setlocale(LC_ALL, "en_US.UTF-8");

    if( (fpIn = fopen(file,"rt")) == NULL  ) {
        return NULL;
    } else {
        int scanResult = 0;
        std::list<DataReport*>* listaDados = new std::list<DataReport*>();

        float fClk = 0.0f;
        long unsigned int accNbOfPck = 0;
        float avgRequiredBwNorm = 0.0f;
        float acceptedTrafficFlits = 0.0f;
        float idealAvgLatency = 0.0f;
        float avgLatencyCycles = 0.0f;
        float stdevLatency = 0.0f;
        float minLatency = 0.0f;
        float maxLatency = 0.0f;
        float avgRequiredBwBps = 0.0f;
        float acceptedTrafficBps = 0.0f;
        float idealAvgLatencyNs = 0.0f;
        float avgLatencyNs = 0.0f;
        float minLatencyNs = 0.0f;
        float maxLatencyNs = 0.0f;
        float metDeadlinesPer0 = 0.0f;
        float metDeadlinesPer1 = 0.0f;
        float metDeadlinesPer2 = 0.0f;
        float metDeadlinesPer3 = 0.0f;

        char str[16];
        do {
            DataReport* data = new DataReport;

            scanResult = fscanf(fpIn,"%s", str);
            if(scanResult != EOF) {

                fClk = (float) atof(str);

                scanResult = fscanf(fpIn,"%s", str);
                accNbOfPck = (unsigned long int) atol(str);

                scanResult = fscanf(fpIn,"%s", str);
                avgRequiredBwNorm = (float) atof(str);

                scanResult = fscanf(fpIn,"%s", str);
                acceptedTrafficFlits = (float) atof(str);

                scanResult = fscanf(fpIn,"%s", str);
                idealAvgLatency = (float) atof(str);

                scanResult = fscanf(fpIn,"%s", str);
                avgLatencyCycles = (float) atof(str);

                scanResult = fscanf(fpIn,"%s", str);
                stdevLatency = (float) atof(str);

                scanResult = fscanf(fpIn,"%s", str);
                minLatency = (float) atof(str);

                scanResult = fscanf(fpIn,"%s", str);
                maxLatency = (float) atof(str);

                scanResult = fscanf(fpIn,"%s", str);
                avgRequiredBwBps = (float) atof(str);

                scanResult = fscanf(fpIn,"%s", str);
                acceptedTrafficBps = (float) atof(str);

                scanResult = fscanf(fpIn,"%s", str);
                idealAvgLatencyNs = (float) atof(str);

                scanResult = fscanf(fpIn,"%s", str);
                avgLatencyNs = (float) atof(str);

                scanResult = fscanf(fpIn,"%s", str);
                minLatencyNs = (float) atof(str);

                scanResult = fscanf(fpIn,"%s", str);
                maxLatencyNs = (float) atof(str);

                scanResult = fscanf(fpIn,"%s", str);
                metDeadlinesPer0 = (float) atof(str);

                scanResult = fscanf(fpIn,"%s", str);
                metDeadlinesPer1 = (float) atof(str);

                scanResult = fscanf(fpIn,"%s", str);
                metDeadlinesPer2 = (float) atof(str);

                scanResult = fscanf(fpIn,"%s", str);
                metDeadlinesPer3 = (float) atof(str);


                data->fClk                  = fClk;
                data->accNbOfPck            = accNbOfPck;
                data->avgRequiredBwNorm     = avgRequiredBwNorm;
                data->acceptedTrafficFlits  = acceptedTrafficFlits;
                data->idealAvgLatency       = idealAvgLatency;
                data->avgLatencyCycles      = avgLatencyCycles;
                data->stdevLatency          = stdevLatency;
                data->minLatency            = minLatency;
                data->maxLatency            = maxLatency;
                data->avgRequiredBwBps      = avgRequiredBwBps;
                data->acceptedTrafficBps    = acceptedTrafficBps;
                data->idealAvgLatencyNs     = idealAvgLatencyNs;
                data->avgLatencyNs          = avgLatencyNs;
                data->minLatencyNs          = minLatencyNs;
                data->maxLatencyNs          = maxLatencyNs;
                data->metDeadlinesPer0      = metDeadlinesPer0;
                data->metDeadlinesPer1      = metDeadlinesPer1;
                data->metDeadlinesPer2      = metDeadlinesPer2;
                data->metDeadlinesPer3      = metDeadlinesPer3;


                listaDados->push_back(data);
            }

        } while (scanResult != EOF);
        if( listaDados->empty() ) {
            delete listaDados;
            listaDados = NULL;
        }
        fclose(fpIn);
        return listaDados;
    }

}

std::list<DataReport *>* ReportReader::readLatencyDistributionReport(const char *file) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Analysis/ReportReader::readLatencyDistributionReport" << std::endl;
#endif

    FILE* fpIn;

    if( (fpIn = fopen(file,"rt")) == NULL ) {
        return NULL;
    } else {
        int scanResult = 0,scanRes;
        std::list<DataReport*>* listaDados = new std::list<DataReport*>();
        long unsigned int latencyCycle,packetCount;
        float packetPercentage;
        char buffer[64];
        do {
            DataReport* data = new DataReport;
            scanResult = fscanf(fpIn,"%[^\n]\n",buffer);
            scanRes = sscanf(buffer,"%lu\t%lu\t%f",
                                &latencyCycle,
                                &packetCount,
                                &packetPercentage);
            data->packetCount      = packetCount;
            data->latencyCycle     = latencyCycle;
            data->packetPercentage = packetPercentage;

            if(scanRes == 3) {
                listaDados->push_back(data);
            } else {
                delete data;
            }
        } while (scanResult != EOF);
        if( listaDados->empty() ) {
            delete listaDados;
            listaDados = NULL;
        }
        fclose(fpIn);
        DataReport* dr = listaDados->back();
        delete dr;
        listaDados->pop_back();
        return listaDados;
    }
}
