/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* FolderCompressor.h
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


#ifndef FOLDERCOMPRESSOR_H
#define FOLDERCOMPRESSOR_H

#include <QRunnable>
#include <QDir>
#include <QObject>
#include <QFile>
#include <QDataStream>

/*!
 * \brief The FolderCompressor class compress and decompress
 * folders and files within the directory.
 */
class FolderCompressor : public QObject,public QRunnable {
    Q_OBJECT
public:

    enum Operation { Compress, Decompress };

    explicit FolderCompressor(Operation op, QString source,QString destination);
    ~FolderCompressor();

    /*!
     * \brief compressFolder A recursive method that scans all files inside the source folder
     * and serializes all files in a row of file names and compressed binary data in a single file
     * \return True if compress successfully completed, otherwise return false
     */
    bool compressFolder();

    /*!
     * \brief decompressFolder A method that deserializes data from the compressed file and
     * creates any needed subfolders before saving the file.
     * \return True if decompress successfully completed, otherwise return false
     */
    bool decompressFolder();

    void run();

signals:
    void completed(bool,int);

private:
    QFile file;
    QDataStream dataStream;

    Operation operation;
    QString source;
    QString destination;

    /*!
     * \brief compress Method in fact that compress files recursively
     * \param sourceFolder Folder to compress files
     * \param prefex Folder prefix to compress
     * \return
     */
    bool compress(QString sourceFolder, QString prefex);

};

#endif // FOLDERCOMPRESSOR_H
