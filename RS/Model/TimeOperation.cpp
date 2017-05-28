/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* TimeOperation.cpp
* Copyright (C) 2016 LEDS - Univali <zeferino@univali.br>
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
* 09/07/2016 - 1.1     - Eduardo Alves da Silva      | First refactoring
* ----------------------------------------------------------------------------
*
*/


#include "Model/TimeOperation.h"

#include <cstdio>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

TimeOperation::TimeOperation() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/TimeOperation" << std::endl;
#endif

}

char* TimeOperation::formatTime(unsigned long long timeMilis) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TimeOperation::formatTime" << std::endl;
#endif

    unsigned long long tempo = timeMilis / 1000;
    unsigned int	hours, mins, secs;
    char* str = new char[256];

    hours = (unsigned int)( tempo/3600);
    mins  = (unsigned int)((tempo - hours*3600)/60);
    secs  = (unsigned int)( tempo - hours*3600 - mins*60);

    if (hours) {
        //		sprintf(str," %d hour, %d min and %d sec (i.e. %llu seconds) ",hours, mins, secs, total_sec);
        sprintf(str," %d h, %d m and %d s ",hours, mins, secs);
    } else {
        if (mins) {
            sprintf(str," %d m and %d s ",mins, secs);
        } else {
            if(secs) {
                sprintf(str," %d s ",secs);
            } else {
                sprintf(str," %llu ms ",timeMilis);
            }
        }
    }
    return str;
}
