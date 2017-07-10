/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* GetSelectedItemsDialog.cpp
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
*
*/

#include "ui_PreviewDialog.h"

#include "AnalysisOptions.h"
#include "GetSelectedItemsDialog.h"

#include <QDialogButtonBox>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

GetSelectedItemsDialog::GetSelectedItemsDialog(QStringList items, QWidget *parent)
    : QDialog(parent), ui(new Ui::PreviewDialog) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class View/GetSelectedItemsDialog" << std::endl;
#endif

    ui->setupUi(this);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );

    ui->treeView->setHeaderLabel( qApp->translate("GetSelectedItemsDialog","Items") );

    QStringList visibleItems = AnalysisOptions::getVisibleStrings(items);

    for(int i = 0; i < items.size(); i++) {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeView);
        QString dir = items.at(i);
        item->setData(0,Qt::UserRole,dir);
        item->setData(0,Qt::DisplayRole, visibleItems.at(i));
    }
    connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
    connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));

    ui->verticalLayout->addWidget(buttonBox);

}

QStringList GetSelectedItemsDialog::getSelectedItems() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/GetSelectedItemsDialog::getSelectedItems" << std::endl;
#endif

    QList<QTreeWidgetItem *> selected = this->ui->treeView->selectedItems();

    QStringList items = QStringList();

    for( int i = 0; i < selected.size(); i++ ) {
        QString item = selected.at(i)->data(0,Qt::UserRole).toString();
        items.append( item );
    }

    return items;

}

GetSelectedItemsDialog::~GetSelectedItemsDialog() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class View/GetSelectedItemsDialog" << std::endl;
#endif
}
