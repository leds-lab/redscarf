/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Util.cpp
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

#include "include/Model/Util.h"

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
  #define OS_WIN
#endif

#ifndef OS_WIN
  #include <sys/stat.h>
#endif

#ifdef __GNUC__
    #include <dirent.h>
#else
    #include <direct.h>
    #include <io.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <tchar.h>
#endif

#include <string>
#include <stdio.h>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

Util::Util() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/Util" << std::endl;
#endif

}


bool Util::folderExists(const char *path) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Util::folderExists" << std::endl;
#endif

    if(path == NULL) {
        return false;
    }

#ifdef __GNUC__
    DIR* pDir;
    bool exist = false;

    pDir = opendir(path);

    if(pDir != NULL) {
        exist = true;
        (void) closedir(pDir);
    }

    return exist;
#else
    if( _taccess_s( path, 0 ) == 0 ) {
        struct _stat status;
        _tstat( path, &status );
        return (status.st_mode & S_IFDIR) != 0;
    }

    return false;
#endif

}

bool Util::createFolder(const char *path) throw(const char* ) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Util::createFolder" << std::endl;
#endif

    if(!folderExists(path)) {
        try {
#ifdef OS_WIN
    #ifdef __GNUC__
            mkdir(path);
    #else
            _mkdir(path);
    #endif
#else
            mkdir(path,0755);
#endif
            return true;
        } catch(...) {
            std::string problem = "Problem in create directory: ";
            problem += std::string(path);
            throw problem.c_str();
        }
    }
    return false;

}

char* Util::formatTime(unsigned long long timeMilis) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Util::formatTime" << std::endl;
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
