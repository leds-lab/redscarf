#include "Archiver.h"

#ifdef DEBUG_POINTS_METHODS
#   include <iostream>
#endif

#include <QDateTime>
#include "FolderCompressor.h"

Archiver::Archiver(QObject *parent) : QObject(parent)
{}

FolderCompressor* Archiver::loadSimulationResults(QString filename, QString workFolder) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Archiver::loadSimulationResults" << std::endl;
#endif

    // Get date and time
    QDateTime dateTime = QDateTime::currentDateTime();
    // Get a string with date and time in ISO format
    QString dateTimeDir = dateTime.toString(Qt::ISODate);
    dateTimeDir.replace(':','-');
    QString workDirLoad= workFolder + "/SimulationsLoaded/@" + dateTimeDir + "/";
    FolderCompressor* fc = new FolderCompressor( FolderCompressor::Decompress,
                        filename, workDirLoad );
    return fc;
}

void Archiver::saveSimulationResults(QStringList simulation) {

}
