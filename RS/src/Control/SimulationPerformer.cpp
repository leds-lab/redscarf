/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* SimulationPerformer.cpp
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

#include "include/Control/SimulationPerformer.h"
#include "include/Model/Util.h"
#include <QElapsedTimer>
#include <QFile>
#include <QTextStream>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

SimulationPerformer::SimulationPerformer(float TClk, QString diretorio, QString compilerDir, QObject *parent) :
    QObject(parent) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Control/SimulationPerformer" << std::endl;
#endif

    this->TClk = TClk;
    this->compilerDir = compilerDir;
    this->executor = new QProcess(this);
    this->executor->setWorkingDirectory( diretorio );
    this->executor->setProcessChannelMode(QProcess::MergedChannels);

    connect( executor, SIGNAL(readyRead()),this,SLOT(readyRead()) );
    connect( executor, SIGNAL(error(QProcess::ProcessError)),this, SLOT(handleError(QProcess::ProcessError)) );
    connect( executor, SIGNAL(finished(int)),executor,SLOT(kill()) );
    connect(this,SIGNAL(finished()),executor,SLOT(kill()));
    connect( executor, SIGNAL(finished(int)),executor,SLOT(deleteLater()) );

}

void SimulationPerformer::execute() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/SimulationPerformer::execute" << std::endl;
#endif

#ifdef Q_OS_WIN32
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PATH",env.value("Path") + ";"+compilerDir+"\\bin");
    executor->setProcessEnvironment(env);
#endif

    QElapsedTimer timer;
    QString workSystem = this->executor->workingDirectory();
    QString localSystem = workSystem.left( workSystem.lastIndexOf("/") );
    timer.start();
    executor->start(localSystem+"/system.x",QStringList() << "." << QString::number(this->TClk) );
    executor->deleteLater();

    if(!executor->waitForFinished(-1)) {
        emit this->sendMessage( trUtf8("<font color=red><br />Failed execution on:</font> %1").arg(workSystem) );
        emit this->unsuccessfullyExecution();
    } else {
        if( executor->exitStatus() == QProcess::NormalExit ) {
            char* tempo = Util::formatTime( qlonglong(timer.elapsed()) );
            QString temp = workSystem.left( workSystem.lastIndexOf("/") );
            temp = workSystem.mid( temp.lastIndexOf("/") + 5 );
            temp.replace("/"," @ ");
            temp.replace("_"," ");
            emit sendMessage(trUtf8("<font color=green><br />Configuration %1</font>")
                             .arg(temp));
            emit sendMessage(trUtf8("<font color=black>- Simulation time: %1</font>")
                             .arg(tempo));
            delete[] tempo;
            unsigned long long simulatedTimeCycles = 0;
            QFile stopOut(workSystem+"/stopsim.out");
            if(stopOut.open(QIODevice::ReadOnly | QIODevice::Text) ) {
                QTextStream ts(&stopOut);
                ts >> simulatedTimeCycles;
                emit sendMessage( trUtf8("<font color=black>- Simulated  time: %1 cycles = %2 us</font>")
                                  .arg(simulatedTimeCycles)
                                  .arg(QString::number(simulatedTimeCycles * this->TClk/1000,'f',2)) );

                stopOut.close();
            }
        }
    }

    emit finished();
}

void SimulationPerformer::handleError(QProcess::ProcessError error) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/SimulationPerformer::handleError" << std::endl;
#endif

    QString typeError;

    switch( error ) {
        case QProcess::FailedToStart:
            typeError = trUtf8("Failed to start");
            break;
        case QProcess::Crashed:
            typeError = trUtf8("Crashed");
            break;
        case QProcess::Timedout:
            typeError = trUtf8("Timedout");
            break;
        case QProcess::WriteError:
            typeError = trUtf8("Write error");
            break;
        case QProcess::ReadError:
            typeError = trUtf8("Read error");
            break;
        case QProcess::UnknownError:
            typeError = trUtf8("Unknown error");
            break;
    }

    emit sendMessage( trUtf8("<font color=red>%1 simulator on directory:</font> %2")
            .arg(typeError)
            .arg(executor->workingDirectory()) );

    emit unsuccessfullyExecution();
    emit finished();
}

void SimulationPerformer::cancel() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/SimulationPerformer::cancel" << std::endl;
#endif

    if( executor != NULL ) {
        if( executor->state() != QProcess::NotRunning ) {
            executor->kill();
        }
    }
}

void SimulationPerformer::readyRead() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/SimulationPerformer::readyRead" << std::endl;
#endif
    QString readOutput = QString::fromUtf8( executor->readAll() );
    readOutput.replace('\n',"<br />");
    emit this->sendMessage( readOutput );
}

SimulationPerformer::~SimulationPerformer() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Control/SimulationPerformer" << std::endl;
#endif

}