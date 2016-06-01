/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* ReportReader.h
* Copyright (C) 2014 - 2016 LEDS - Univali <zeferino@univali.br>
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
#ifndef REPORTREADER_H
#define REPORTREADER_H

#include <list>

class DataReport;

class ReportReader {

public:
    ReportReader();
    std::list<DataReport *>* readLatencyDistributionReport(const char *file);
    std::list<DataReport *>* readReport(const char *file);


};

#endif // REPORTREADER_H
