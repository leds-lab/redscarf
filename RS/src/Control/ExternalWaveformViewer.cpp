/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* ExternalWaveformViewer.cpp
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

#include "include/Control/ExternalWaveformViewer.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

ExternalWaveformViewer::ExternalWaveformViewer(QString toolExe, QStringList args, QObject *parent)
    : WaveformViewer(parent) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Control/ExternalWaveformViewer" << std::endl;
#endif

    this->toolExe = toolExe;
    this->args = args;

}

void ExternalWaveformViewer::viewWaveform(QString dir) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/ExternalWaveformViewer::viewWaveform" << std::endl;
#endif

    QProcess* externalWaveformViewer = new QProcess(this);
    externalWaveformViewer->setProcessChannelMode( QProcess::MergedChannels );
    externalWaveformViewer->setWorkingDirectory(dir);
    connect(externalWaveformViewer,SIGNAL(finished(int,QProcess::ExitStatus)),this,SIGNAL(finished(int,QProcess::ExitStatus)));
    connect(externalWaveformViewer,SIGNAL(finished(int)),externalWaveformViewer,SLOT(kill()));
    connect(externalWaveformViewer,SIGNAL(finished(int)),externalWaveformViewer,SLOT(deleteLater()));
    connect(externalWaveformViewer,SIGNAL(readyRead()),this,SLOT(readReady()));
    connect(externalWaveformViewer,SIGNAL(error(QProcess::ProcessError)),this,SLOT(erroOnExecute(QProcess::ProcessError)));
    // "-a ../list_nodes.sav"  -- argumento para gtkwave
    QString command = QString("%1 %2/snocs_wave.vcd ").arg(toolExe).arg(dir);//.arg(args);
    for( int i = 0; i < args.size(); i++) {
        command += QString("%1 ").arg(args.at(i));
    }
    externalWaveformViewer->start(command);
}

void ExternalWaveformViewer::erroOnExecute(QProcess::ProcessError error) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/ExternalWaveformViewer::errorOnExecute" << std::endl;
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

    emit sendMessage( trUtf8("<font color=red>Waveform tool error: </font> %1")
            .arg(typeError));

}

void ExternalWaveformViewer::readReady() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/ExternalWaveformViewer::readReady" << std::endl;
#endif

    QProcess* tool = (QProcess* ) sender();

    emit sendMessage( trUtf8("Waveform viewer: %1").arg(QString::fromUtf8(tool->readAll())) );

}
