/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* XMLPreviewTrafficConfiguration.cpp
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

#include "include/View/XMLPreviewTrafficConfiguration.h"
#include "include/Model/Traffic/TrafficPatternDefines.h"
#include "include/Model/Traffic/Node.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

XMLPreviewTrafficConfiguration::XMLPreviewTrafficConfiguration(QWidget *parent) : PreviewDialog(parent) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class View/XMLPreviewTrafficConfiguration" << std::endl;
#endif

    QStringList header;
    header << qApp->translate("XMLPreviewTrafficConfiguration","Property",0)
           << qApp->translate("XMLPreviewTrafficConfiguration","Value",0);

    this->ui->treeView->setHeaderLabels(header);

    for(int i = 0; i < header.size(); i++) {
        this->ui->treeView->resizeColumnToContents(i);
    }

    this->ui->treeView->header()->resizeSection(0,200);

}

void XMLPreviewTrafficConfiguration::addNode(Node *nodo) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/XMLPreviewTrafficConfiguration::addNode" << std::endl;
#endif

    TrafficPatternDefines* def = TrafficPatternDefines::getInstance();

    QTreeWidgetItem* src = new QTreeWidgetItem(this->ui->treeView);
    src->setData(0,Qt::DisplayRole,qApp->translate("XMLPreviewTrafficConfiguration","Source Node",0));
    src->setData(1,Qt::DisplayRole,QString("X: %1\tY: %2").arg(QString::number(nodo->getPosicaoX())).arg(QString::number(nodo->getPosicaoY())));
    for(int i = 0; i < MAX_PATTERNS; i++) {
        if(nodo->isPatternActive(i)) {
            QTreeWidgetItem* trafficPattern = new QTreeWidgetItem(src);
            trafficPattern->setData(0,Qt::DisplayRole,qApp->translate("XMLPreviewTrafficConfiguration","Traffic Pattern"));
            trafficPattern->setData(1,Qt::DisplayRole,QString::number(i));
            TrafficParameters* tp = nodo->getTrafficPattern(i);
            QTreeWidgetItem* sptDist = new QTreeWidgetItem(trafficPattern);
            sptDist->setData(0,Qt::DisplayRole,qApp->translate("XMLPreviewTrafficConfiguration","Spatial Distribution",0));
            unsigned int nSptDist = tp->getSpatialDistribution();
            if(nSptDist == 0) {
                sptDist->setData(1,Qt::DisplayRole,qApp->translate("XMLPreviewTrafficConfiguration","Specified Address -> X Dest.: %1\tY Dest.: %2",0).arg(QString::number(tp->getDestinationNodeX())).arg(QString::number(tp->getDestinationNodeY())));
            } else {
                sptDist->setData(1,Qt::DisplayRole,QString::fromStdString(def->findSpatialDistribution(nSptDist)));
            }
            QTreeWidgetItem* trfClass = new QTreeWidgetItem(trafficPattern);
            trfClass->setData(0,Qt::DisplayRole,qApp->translate("XMLPreviewTrafficConfiguration","Traffic Class",0));
            trfClass->setData(1,Qt::DisplayRole,QString::fromStdString(def->findTrafficClass(tp->getTrafficClass())));

            unsigned int typeInj = tp->getInjectionType();
            QTreeWidgetItem* tpInj = new QTreeWidgetItem(trafficPattern);
            tpInj->setData(0,Qt::DisplayRole,qApp->translate("XMLPreviewTrafficConfiguration","Type Injection",0));
            tpInj->setData(1,Qt::DisplayRole,QString::fromStdString(def->findTypeInjection(typeInj)));

            QTreeWidgetItem* swtTec = new QTreeWidgetItem(trafficPattern);
            swtTec->setData(0,Qt::DisplayRole,qApp->translate("XMLPreviewTrafficConfiguration","Switching Technique",0));
            swtTec->setData(1,Qt::DisplayRole,QString::fromStdString(def->findSwitchingTechnique(tp->getSwitchingTechnique())));

            QTreeWidgetItem* nppf = new QTreeWidgetItem(trafficPattern);
            nppf->setData(0,Qt::DisplayRole,qApp->translate("XMLPreviewTrafficConfiguration","Number Packets Per Flow",0));
            nppf->setData(1,Qt::DisplayRole,QString::number(tp->getPackageToSend()));

            QTreeWidgetItem* dead = new QTreeWidgetItem(trafficPattern);
            dead->setData(0,Qt::DisplayRole,qApp->translate("XMLPreviewTrafficConfiguration","Deadline",0));
            dead->setData(1,Qt::DisplayRole,QString::number(tp->getDeadline()));

            QTreeWidgetItem* rqBw = new QTreeWidgetItem(trafficPattern);
            rqBw->setData(0,Qt::DisplayRole,qApp->translate("XMLPreviewTrafficConfiguration","Required Bandwidth",0));
            rqBw->setData(1,Qt::DisplayRole,QString::number(tp->getRequiredBandwidth()));


            if(typeInj == 0 || typeInj == 1 || typeInj == 4 || typeInj == 5) {
                QTreeWidgetItem* msgSize = new QTreeWidgetItem(trafficPattern);
                msgSize->setData(0,Qt::DisplayRole,qApp->translate("XMLPreviewTrafficConfiguration","Message size",0));
                msgSize->setData(1,Qt::DisplayRole,QString::number(tp->getMessageSize()));
            } else if(typeInj == 2) {
                QTreeWidgetItem* idlTime = new QTreeWidgetItem(trafficPattern);
                idlTime->setData(0,Qt::DisplayRole,qApp->translate("XMLPreviewTrafficConfiguration","Idle Time",0));
                idlTime->setData(1,Qt::DisplayRole,QString::number(tp->getIdleTime()));
            }
            if(typeInj == 3 || typeInj == 5) {
                QTreeWidgetItem* msgInt = new QTreeWidgetItem(trafficPattern);
                msgInt->setData(0,Qt::DisplayRole,qApp->translate("XMLPreviewTrafficConfiguration","Message Interval",0));
                msgInt->setData(1,Qt::DisplayRole,QString::number(tp->getIntervalTime()));
            }

            if(typeInj != 0) {
                unsigned int funcProb = tp->getProbabilityFunction();
                QTreeWidgetItem* fProb = new QTreeWidgetItem(trafficPattern);
                fProb->setData(0,Qt::DisplayRole,qApp->translate("XMLPreviewTrafficConfiguration","Function Probability",0));
                fProb->setData(1,Qt::DisplayRole,QString::fromStdString(def->findFunctionProbability(funcProb)));
                if(funcProb == 0) {
                    QTreeWidgetItem* stdDev = new QTreeWidgetItem(trafficPattern);
                    stdDev->setData(0,Qt::DisplayRole,qApp->translate("XMLPreviewTrafficConfiguration","Standard Deviation",0));
                    stdDev->setData(1,Qt::DisplayRole,QString::number(tp->getRequiredBandwidthStdDeviation()));
                } else if(funcProb == 2) {
                    QTreeWidgetItem* alfaOn = new QTreeWidgetItem(trafficPattern);
                    alfaOn->setData(0,Qt::DisplayRole,qApp->translate("XMLPreviewTrafficConfiguration","Alfa On",0));
                    alfaOn->setData(1,Qt::DisplayRole,QString::number(tp->getAlfaOn()));
                    QTreeWidgetItem* alfaOff = new QTreeWidgetItem(trafficPattern);
                    alfaOff->setData(0,Qt::DisplayRole,qApp->translate("XMLPreviewTrafficConfiguration","Alfa Off",0));
                    alfaOff->setData(1,Qt::DisplayRole,QString::number(tp->getAlfaOff()));
                }
            }
        }
    }

}
