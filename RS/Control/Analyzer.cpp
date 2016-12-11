/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Analyzer.cpp
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
* ----------------------------------------------------------------------------
* 20/11/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*
*/

#include "Control/Analyzer.h"
#include "Model/Analysis/PerformanceAnalysis.h"
#include "Model/System/SystemDefines.h"

#include <QDir>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

Analyzer::Analyzer(QList<QString> *analysisFolders,
                   unsigned short numElements,
                   unsigned int dataWidth,
                   float lower,float upper,
                   QObject* parent)
    : QObject(parent) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Control/Analyzer" << std::endl;
#endif

    this->numElements = numElements;
    this->dataWidth = dataWidth;
    this->lower = lower;
    this->upper = upper;

    this->analysisFolders = analysisFolders;
}

Analyzer::~Analyzer() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Control/Analyzer" << std::endl;
#endif
}

void Analyzer::analyze() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Analyzer::analyze" << std::endl;
#endif

    for (int i = 0; i < analysisFolders->size(); i++) {
        bool falhaAnalise;
        QString diretorioAnalise = analysisFolders->at(i);
        int lio = diretorioAnalise.lastIndexOf("/");
        QString diretorioExperimento = diretorioAnalise.left(lio);
        QDir dirResultados = QDir(diretorioExperimento + "/Results");
        if (!dirResultados.exists()) {
            dirResultados.mkpath(".");
        }
        QDir dirAnalise = QDir(diretorioAnalise + "/Results");
        if (!dirAnalise.exists()) {
            dirAnalise.mkpath(".");
        }
        // Obtendo dados a partir do diretorio de analise
        float fClk = diretorioAnalise.mid(lio + 1, diretorioAnalise.size() - lio - 4).toFloat();
        QString auxiliarExperimento = diretorioExperimento.right(
                    diretorioExperimento.size() - 1 -
                    diretorioExperimento.lastIndexOf("/"));  // Extrair controle de fluxo e
        // profundidade dos buffers de saída
        QStringList configuracao = auxiliarExperimento.split("_");
        unsigned int flowControlType =
                SystemDefines::getInstance()->getKeyFlowControl(configuracao.at(3));
        auxiliarExperimento = configuracao.at(6);
        auxiliarExperimento.remove(0, 3);
        unsigned int fifoOutDepth = auxiliarExperimento.toUInt();

        QByteArray byteArrayAnalise = diretorioAnalise.toUtf8();
        QByteArray byteArrayResultado = dirResultados.absolutePath().toUtf8();
        const char* analise = byteArrayAnalise.constData();
        const char* resultado = byteArrayResultado.constData();

        // Realizar a análise
        TrafficAnalysis* analyzer = new PerformanceAnalysis(
                    numElements, dataWidth, lower, upper, fClk,
                    fifoOutDepth, flowControlType, analise, resultado);
        TrafficAnalysis::StatusAnalysis resultAnalysis = analyzer->makeAnalysis();
        falhaAnalise = true;
        QString dirAnalyzed = diretorioAnalise.mid( diretorioExperimento.lastIndexOf("/") + 5 );
        dirAnalyzed.replace("_"," ");
        dirAnalyzed.replace("/"," @ ");
        switch (resultAnalysis) {
            case TrafficAnalysis::NoInputFile:
                this->sendMessage(trUtf8("<font color=red>There is no input file for analysis (maybe "
                                      "simulation was not run) in %1</font>").arg(dirAnalyzed));
                break;
            case TrafficAnalysis::Ok:  // Analysis successfully
                this->sendMessage(trUtf8("<font color=black>- %1</font>")
                            .arg(dirAnalyzed));
                falhaAnalise = false;
                break;
            case TrafficAnalysis::NoPacketsDelivered:
                this->sendMessage(trUtf8("<font color=red>Simulation time too short. None packet was "
                                      "delivered in %1</font>").arg(dirAnalyzed));
                break;
            case TrafficAnalysis::NoOutputFile:
                this->sendMessage(trUtf8("<font color=red>Impossible create result file in %1</font>")
                                  .arg(dirAnalyzed));
                break;
        }
        delete analyzer;
        if (falhaAnalise) {
            emit this->finished(false);
            return;
        }
    }

    emit this->finished(true);
}
