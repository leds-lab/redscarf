/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* FolderOperation.cpp
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
* 10/12/2014 - 1.0     - Eduardo Alves da Silva      | Initial release
* ----------------------------------------------------------------------------
* 09/07/2016 - 1.1     - Eduardo Alves da Silva      | First refactoring
* ----------------------------------------------------------------------------

*
*/

#include "include/Model/FolderOperation.h"

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

FolderOperation::FolderOperation() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/FolderOperation" << std::endl;
#endif

}


bool FolderOperation::folderExists(const char *path) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/FolderOperation::folderExists" << std::endl;
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

bool FolderOperation::createFolder(const char *path) throw(const char* ) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/FolderOperation::createFolder" << std::endl;
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
