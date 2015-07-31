/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* DefaultPreviewTrafficConfiguration.cpp
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

#include "include/View/DefaultPreviewTrafficConfiguration.h"
#include "include/Model/Traffic/TrafficPatternDefines.h"
#include "include/Model/Traffic/Node.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

DefaultPreviewTrafficConfiguration::DefaultPreviewTrafficConfiguration(QWidget *parent) : PreviewDialog(parent) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class View/DefaultPreviewTrafficConfiguration" << std::endl;
#endif

    QStringList header;

    header << qApp->translate("DefaultPreviewTrafficConfiguration","Source\nNode",0)
          << qApp->translate("DefaultPreviewTrafficConfiguration","Destination\nNode",0)
          << qApp->translate("DefaultPreviewTrafficConfiguration","Traffic\nClass",0)
          << qApp->translate("DefaultPreviewTrafficConfiguration","Injection\nType",0)
          << qApp->translate("DefaultPreviewTrafficConfiguration","Switching\nType",0)
          << qApp->translate("DefaultPreviewTrafficConfiguration","Packets\nto send",0)
          << qApp->translate("DefaultPreviewTrafficConfiguration","Deadline\n(ns)",0)
          << qApp->translate("DefaultPreviewTrafficConfiguration","Required\nBandwidth\n(Mbps)",0)
          << qApp->translate("DefaultPreviewTrafficConfiguration","Message\nSize\n(bits)",0)
          << qApp->translate("DefaultPreviewTrafficConfiguration","Idle\nTime\n(ns)",0)
          << qApp->translate("DefaultPreviewTrafficConfiguration","Message\nInterval\n(ns)",0)
          << qApp->translate("DefaultPreviewTrafficConfiguration","Probability\nFunction",0)
          << qApp->translate("DefaultPreviewTrafficConfiguration","Standard\nDeviation\n(% Req. BW)",0)
          << qApp->translate("DefaultPreviewTrafficConfiguration","Alfa\nOn",0);

    this->ui->treeView->setHeaderLabels(header);

}

void DefaultPreviewTrafficConfiguration::addNode(Node *nodo) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/DefaultPreviewTrafficConfiguration::addNode" << std::endl;
#endif

    TrafficPatternDefines* def = TrafficPatternDefines::getInstance();
    for( int i = 0; i < MAX_PATTERNS; i++) {
        if(nodo->isPatternActive(i)) {
            QTreeWidgetItem* item = new QTreeWidgetItem(this->ui->treeView);
            TrafficParameters* tp = nodo->getTrafficPattern(i);
            item->setData(0,Qt::DisplayRole,QString("(%1,%2): %3").arg(nodo->getPosicaoX()).arg(nodo->getPosicaoY()).arg(QString::number(i)));
            unsigned int spDist = tp->getSpatialDistribution();
            if(spDist == 0) {
                item->setData(1,Qt::DisplayRole,QString("(%1,%2)").arg(QString::number(tp->getDestinationNodeX())).arg(QString::number(tp->getDestinationNodeY())));
            } else {
                item->setData(1,Qt::DisplayRole,QString::fromStdString(def->findSpatialDistribution(tp->getSpatialDistribution())));
            }
            QString trafficClass = QString::fromStdString(def->findTrafficClass(tp->getTrafficClass()));
            item->setData(2,Qt::DisplayRole, trafficClass.left(trafficClass.indexOf("-") - 1) );

            unsigned int typeInjec = tp->getInjectionType();
            item->setData(3,Qt::DisplayRole,QString::fromStdString(def->findTypeInjection(typeInjec)));
            QString switchingTech = QString::fromStdString(def->findSwitchingTechnique(tp->getSwitchingTechnique()));
            switchingTech.remove(" Switching");
            item->setData(4,Qt::DisplayRole,switchingTech);
            item->setData(5,Qt::DisplayRole,QString::number(tp->getPackageToSend()));
            item->setData(6,Qt::DisplayRole,QString::number(tp->getDeadline()));
            item->setData(7,Qt::DisplayRole,QString::number(tp->getRequiredBandwidth()));

            if(typeInjec == 0 || typeInjec == 1 || typeInjec == 4 || typeInjec == 5) {
                item->setData(8,Qt::DisplayRole,QString::number(tp->getMessageSize()));
            } else if(typeInjec == 2) {
                item->setData(9,Qt::DisplayRole,QString::number(tp->getIdleTime()));
            }
            if(typeInjec == 3 || typeInjec == 5) {
                item->setData(10,Qt::DisplayRole,QString::number(tp->getIntervalTime()));
            }

            if(typeInjec != 0) {
                unsigned int funcProb = tp->getProbabilityFunction();
                item->setData(11,Qt::DisplayRole,QString::fromStdString(def->findFunctionProbability(funcProb)));
                if(funcProb == 0) {
                    item->setData(12,Qt::DisplayRole,QString::number(tp->getRequiredBandwidthStdDeviation()));
                } else if(funcProb == 2) {
                    item->setData(13,Qt::DisplayRole,QString::number(tp->getAlfaOn()));
                }
            }

        }
    }

    int numColunas = ui->treeView->columnCount();
    for(int i = 0; i < numColunas; i++) {
        ui->treeView->resizeColumnToContents(i);
    }
}
