/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* ReportDialog.cpp
* Copyright (C) 2014 - 2017 LEDS - Univali <zeferino@univali.br>
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
* 09/12/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*/

#include "ui_PreviewDialog.h"
#include "View/ReportDialog.h"
#include "Model/Analysis/DataReport.h"

#include <QLabel>
#include <QComboBox>


#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

ReportDialog::ReportDialog(QStringList legendas,
                           QVector<QList<DataReport *> *> *_dados,
                           QWidget *parent)
    : QDialog(parent), ui(new Ui::PreviewDialog) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class View/ReportDialog" << std::endl;
#endif
    ui->setupUi(this);

    QLabel* label = new QLabel(qApp->translate("ReportDialog","Experiment: "),this);
    combo = new QComboBox(this);
    combo->addItems(legendas);
    connect(combo,SIGNAL(currentIndexChanged(int)),this,SLOT(changeReport(int)));

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setObjectName("newHorizontalLayout");
    hLayout->addWidget(label);
    hLayout->addWidget(combo);
    hLayout->addSpacing(40);

    this->ui->verticalLayout->addLayout(hLayout);

    this->dados = _dados;

    this->setWindowTitle(qApp->translate("ReportDialog","Report"));

    QStringList header;

    header << qApp->translate("ReportDialog","Fclk\n(MHz)")
           << qApp->translate("ReportDialog","Analyzed\nPackets")
           << qApp->translate("ReportDialog","Offered\nTraffic\n(norm)")
           << qApp->translate("ReportDialog","Accepted\nTraffic\n(norm)")
           << qApp->translate("ReportDialog","Ideal\nAvg\nLatency\n(cycles)")
           << qApp->translate("ReportDialog","Avg\nLatency\n(cycles)")
           << qApp->translate("ReportDialog","Min\nLatency\n(cycles)")
           << qApp->translate("ReportDialog","Max\nLatency\n(cycles)")
           << qApp->translate("ReportDialog","Std Dev\nLatency")
           << qApp->translate("ReportDialog","RT0 Met\nDeadlines\n(%)")
           << qApp->translate("ReportDialog","RT1 Met\nDeadlines\n(%)")
           << qApp->translate("ReportDialog","nRT0 Met\nDeadlines\n(%)")
           << qApp->translate("ReportDialog","nRT1 Met\nDeadlines\n(%)")
           << qApp->translate("ReportDialog","Offered\nTraffic\n(Mbps/n)")
           << qApp->translate("ReportDialog","Accepted\nTraffic\n(Mbps/n)")
           << qApp->translate("ReportDialog","Ideal\nAvg\nLatency\n(ns)")
           << qApp->translate("ReportDialog","Avg\nLatency\n(ns)")
           << qApp->translate("ReportDialog","Min\nLatency\n(ns)")
           << qApp->translate("ReportDialog","Max\nLatency\n(ns)");;

    this->ui->treeView->setHeaderLabels(header);

    for(int i = 0; i < header.size(); i++) {
        this->ui->treeView->headerItem()->setTextAlignment(i,Qt::AlignCenter);
        this->ui->treeView->resizeColumnToContents(i);
    }

    this->changeReport(0);

    this->setModal(false);

}

void ReportDialog::changeReport(int pos) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/ReportDialog::changeReport" << std::endl;
#endif

    ui->treeView->clear();

    QList<DataReport *> *listData = this->dados->at(pos);
    int size = listData->size();
    for( int i = 0; i < size; i++ ) {
        DataReport* dado = listData->at(i);

        QTreeWidgetItem* item = new QTreeWidgetItem(this->ui->treeView);
        item->setData(0,Qt::DisplayRole, QString::number(dado->fClk,'f',0) );
        item->setData(1,Qt::DisplayRole, QString::number(dado->accNbOfPck,'f',0) );
        item->setData(2,Qt::DisplayRole, QString::number(dado->avgRequiredBwNorm,'f',4) );
        item->setData(3,Qt::DisplayRole, QString::number(dado->acceptedTrafficFlits,'f',4) );
        item->setData(4,Qt::DisplayRole, QString::number(dado->idealAvgLatency,'f',0) );
        item->setData(5,Qt::DisplayRole, QString::number(dado->avgLatencyCycles,'f',1) );
        item->setData(6,Qt::DisplayRole, QString::number(dado->minLatency,'f',0) );
        item->setData(7,Qt::DisplayRole, QString::number(dado->maxLatency,'f',0) );
        item->setData(8,Qt::DisplayRole, QString::number(dado->stdevLatency,'f',1) );
        item->setData(9,Qt::DisplayRole, (dado->metDeadlinesPer0 == -1 ? QString():QString::number(dado->metDeadlinesPer0,'g',4)) );
        item->setData(10,Qt::DisplayRole, (dado->metDeadlinesPer1 == -1 ? QString():QString::number(dado->metDeadlinesPer1,'g',4)) );
        item->setData(11,Qt::DisplayRole, (dado->metDeadlinesPer2 == -1 ? QString():QString::number(dado->metDeadlinesPer2,'g',4)) );
        item->setData(12,Qt::DisplayRole, (dado->metDeadlinesPer3 == -1 ? QString():QString::number(dado->metDeadlinesPer3,'g',4)) );
        item->setData(13,Qt::DisplayRole, QString::number(dado->avgRequiredBwBps,'f',1) );
        item->setData(14,Qt::DisplayRole, QString::number(dado->acceptedTrafficBps,'f',1) );
        item->setData(15,Qt::DisplayRole, QString::number(dado->idealAvgLatencyNs,'f',1) );
        item->setData(16,Qt::DisplayRole, QString::number(dado->avgLatencyNs,'f',1) );
        item->setData(17,Qt::DisplayRole, QString::number(dado->minLatencyNs,'f',1) );
        item->setData(18,Qt::DisplayRole, QString::number(dado->maxLatencyNs,'f',1) );
        for( int x = 0; x < ui->treeView->columnCount(); x++ ) {
            item->setTextAlignment(x,Qt::AlignCenter);
        }
    }

    for(int i = 0; i < ui->treeView->columnCount(); i++) {
        this->ui->treeView->resizeColumnToContents(i);
    }

}

ReportDialog::~ReportDialog() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class View/ReportDialog" << std::endl;
#endif

    for( int x = 0; x < dados->size(); x++ ) {
        QList<DataReport* >* listData = dados->at(x);
        if( listData != NULL ) {
            for( int z = 0; z < listData->size(); z++ ) {
                DataReport* d = listData->at(z);
                delete d;
            }
            listData->clear();
            delete listData;
        }
    }
    this->dados->clear();
    delete dados;

    delete combo;

    delete ui;
}
