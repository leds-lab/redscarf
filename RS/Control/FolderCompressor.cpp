/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* FolderCompressor.cpp
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

#include "FolderCompressor.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

FolderCompressor::FolderCompressor(Operation op, QString source, QString destination) :
    QObject(0) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor class Control/FolderCompressor" << std::endl;
#endif
    this->operation = op;
    this->source = source;
    this->destination = destination;
}

FolderCompressor::~FolderCompressor() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor class Control/FolderCompressor" << std::endl;
#endif

}

void FolderCompressor::run() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/FolderCompressor::run" << std::endl;
#endif

    bool result = false;
    int op = 0;
    switch(operation) {
        case Compress:{
            op = 1;
            result = this->compressFolder();
            break;
        }
        case Decompress: {
            op = 2;
            result = this->decompressFolder();
            break;
        }
    }

    emit completed(result,op,destination);

}

bool FolderCompressor::compressFolder() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/FolderCompressor::compressFolder" << std::endl;
#endif

    QDir src(source);
    if(!src.exists()) { //folder not found
        return false;
    }

    file.setFileName(destination);
    if(!file.open(QIODevice::WriteOnly)) { //could not open file
        return false;
    }

    dataStream.setDevice(&file);

    bool success = compress(source, "");
    file.close();

    return success;
}

bool FolderCompressor::compress(QString sourceFolder, QString prefix) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/FolderCompressor::compress" << std::endl;
#endif

    QDir dir(sourceFolder);
    if(!dir.exists()) {
        return false;
    }

    //1 - list all folders inside the current folder
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    QFileInfoList foldersList = dir.entryInfoList();

    //2 - For each folder in list: call the same function with folders' paths
    for(int i=0; i<foldersList.length(); i++) {
        QString folderName = foldersList.at(i).fileName();
        QString folderPath = dir.absolutePath()+"/"+folderName;
        QString newPrefix = prefix+"/"+folderName;

        compress(folderPath, newPrefix);
    }

    //3 - List all files inside the current folder
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    QFileInfoList filesList = dir.entryInfoList();

    //4- For each file in list: add file path and compressed binary data
    for(int i=0; i<filesList.length(); i++) {
        QFile file(dir.absolutePath()+"/"+filesList.at(i).fileName());
        if(!file.open(QIODevice::ReadOnly)) { //couldn't open file
            return false;
        }

        dataStream << QString(prefix+"/"+filesList.at(i).fileName());
        dataStream << qCompress(file.readAll());

        file.close();
    }

    return true;
}

bool FolderCompressor::decompressFolder() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/FolderCompressor::decompressFolder" << std::endl;
#endif

    // validation
    QFile src(source);
    if(!src.exists()) {//file not found, to handle later
        return false;
    }
    QDir dir;
    if(!dir.mkpath(destination)) {//could not create folder
        return false;
    }

    file.setFileName(source);
    if(!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    dataStream.setDevice(&file);

    while(!dataStream.atEnd()) {
        QString fileName;
        QByteArray data;

        //extract file name and data in order
        dataStream >> fileName >> data;

        //create any needed folder
        QString subfolder;
        for(int i=fileName.length()-1; i>0; i--) {
            if((QString(fileName.at(i)) == QString("\\")) || (QString(fileName.at(i)) == QString("/"))) {
                subfolder = fileName.left(i);
                dir.mkpath(destination+"/"+subfolder);
                break;
            }
        }

        QFile outFile(destination+"/"+fileName);
        if(!outFile.open(QIODevice::WriteOnly)) {
            file.close();
            return false;
        }
        outFile.write(qUncompress(data));
        outFile.close();
    }

    file.close();
    return true;
}
