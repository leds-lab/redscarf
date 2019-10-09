/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
*/
/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* ConfigDialog.cpp this file contains part of the source code from examples of the Qt Toolkit
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

#include <QtWidgets>

#include "ConfigDialog.h"
#include "Pages.h"

#include "Control/EnvironmentConfiguration.h"
#include "Control/Control.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

ConfigDialog::ConfigDialog(EnvironmentConfiguration *envConf, QWidget* parent) : QDialog(parent) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class View/ConfigDialog" << std::endl;
#endif
    this->setWindowFlags(this->windowFlags() ^ Qt::WindowContextHelpButtonHint);

    contentsWidget = new QListWidget(this);
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(96, 84));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(114);
    contentsWidget->setSpacing(12);

    pagesWidget = new QStackedWidget(this);
    // Add settings pages and connect your change signals with this class
    pagesWidget->addWidget(new GeneralConfigurationPage( envConf->getLanguageNames(),
                                                         envConf->getLanguageNameSelected(),
                                                         envConf->getThreadNumber(),this) );
    pagesWidget->addWidget(new FoldersConfigurationPage(envConf->getSimulatorLocation(),
                                                        envConf->getPluginsFolder(),
                                                        envConf->getWorkFolder(),
                                                        envConf->getWaveformTool(),this) );

    QFont font;
    font.setBold(true);
    font.setPointSize(16);
    menuSelected = new QLabel(this);
    menuSelected->setFont(font);

    QPushButton *cancelButton = new QPushButton(tr("Cancel"),this);
    QPushButton *okButton     = new QPushButton(tr("Ok"),this);
    okButton->setDefault(true);

    // Adicionar itens (icones) das páginas de configurações
    createIcons();
    contentsWidget->setCurrentRow(0);

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(okButton, SIGNAL(clicked()),this,SLOT(okButtonClicked()));

    QVBoxLayout *menuLayout = new QVBoxLayout;
    menuLayout->addWidget(menuSelected);
    menuLayout->addWidget(pagesWidget,1);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addLayout(menuLayout, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("RedScarf options"));

}

ConfigDialog::~ConfigDialog() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class View/ConfigDialog" << std::endl;
#endif

    delete pagesWidget;
    delete contentsWidget;
    delete menuSelected;
}

void ConfigDialog::createIcons() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/ConfigDialog::createIcons" << std::endl;
#endif

    QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
    configButton->setIcon(QIcon(":/icons/icons/icon_tool.png"));
    configButton->setText(tr("General"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *foldersButton = new QListWidgetItem(contentsWidget);
    foldersButton->setIcon(QIcon(":/icons/icons/icon_open.png"));
    foldersButton->setText(tr("Folders"));
    foldersButton->setTextAlignment(Qt::AlignHCenter);
    foldersButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}


void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/ConfigDialog::changePage" << std::endl;
#endif

    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
    menuSelected->setText( current->text() );
}


void ConfigDialog::okButtonClicked() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/ConfigDialog::okButtonClicked" << std::endl;
#endif

    EnvironmentConfiguration* env = new EnvironmentConfiguration(this);

    QString languageSelectedName;

    int count = pagesWidget->count();
    for( int i = 0; i < count; i++ ) {
        QWidget* page = pagesWidget->widget(i);
        // Get General Configuration options
        GeneralConfigurationPage* gcp = dynamic_cast<GeneralConfigurationPage*>(page);
        if( gcp ) {
            env->setThreadNumber( quint32(gcp->getThreadNumber()) );
            languageSelectedName = gcp->getSelectedLanguage();
            continue;
        }

        // Get Folders Configuration options
        FoldersConfigurationPage* fcp = dynamic_cast<FoldersConfigurationPage*>(page);
        if( fcp ) {
            env->setSimulatorLocation( fcp->getSimulatorLocation() );
            env->setPluginsFolder( fcp->getPluginsFolder() );
            env->setWorkFolder( fcp->getWorkFolder() );
            env->setWaveformTool( fcp->getWaveformTool() );
            continue;
        }
    }

    emit applyConfiguration(env,languageSelectedName);
    accept();
    delete env;

}
