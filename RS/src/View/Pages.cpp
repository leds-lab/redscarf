/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Pages.cpp
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

#include "ui_GeneralConfigurationPage.h"
#include "ui_FoldersConfigurationPage.h"

#include "include/View/Pages.h"

#include "include/Control/EnvironmentConfiguration.h"

#include <QCompleter>
#include <QDirModel>
#include <QFileDialog>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

////////////////////////// GeneralConfigurationPage //////////////////////////
GeneralConfigurationPage::GeneralConfigurationPage(QStringList availableLanguages,
        QString selectedLanguage, int numThreads, QWidget *parent) :
    QWidget(parent),ui(new Ui::GeneralConfigurationPage) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class View/GeneralConfigurationPage (Pages.h)" << std::endl;
#endif
    ui->setupUi(this);

    ui->comboBoxLanguage->addItems(availableLanguages);
    ui->comboBoxLanguage->setCurrentText( selectedLanguage );
    ui->spinBoxThreads->setValue( numThreads );

}

const QString GeneralConfigurationPage::getSelectedLanguage() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/GeneralConfigurationPage::getSelectedLanguage (Pages.h)" << std::endl;
#endif
    return ui->comboBoxLanguage->currentText();
}

int GeneralConfigurationPage::getThreadNumber() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/GeneralConfigurationPage::getThreadNumber (Pages.h)" << std::endl;
#endif
    return ui->spinBoxThreads->value();
}

GeneralConfigurationPage::~GeneralConfigurationPage() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class View/GeneralConfigurationPage (Pages.h)" << std::endl;
#endif

    delete ui;

}

////////////////////////// GeneralConfigurationPage //////////////////////////

///------------------------------------------------------------------------------

////////////////////////// FoldersConfigurationPage //////////////////////////
FoldersConfigurationPage::FoldersConfigurationPage(QString systemc,QString mingw,QString work,
                                                   QString waveformTool, QWidget *parent)
            :QWidget(parent), ui(new Ui::FoldersConfigurationPage) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class View/FoldersConfigurationPage (Pages.h)" << std::endl;
#endif
    ui->setupUi(this);

    QCompleter* folderCompleter = new QCompleter(this);
    folderCompleter->setModel( new QDirModel(folderCompleter) );
    ui->lineEditSystemCFolder->setCompleter( folderCompleter );
    ui->lineEditMinGWFolder->setCompleter( folderCompleter );
    ui->lineEditWorkFolder->setCompleter( folderCompleter );
    ui->lineEditWaveformTool->setCompleter(folderCompleter);

#ifdef Q_OS_UNIX
    ui->widgetMinGWOption->setEnabled(false);
#endif

    ui->lineEditMinGWFolder->setText( mingw );
    ui->lineEditSystemCFolder->setText( systemc );
    ui->lineEditWorkFolder->setText( work );
    ui->lineEditWaveformTool->setText(waveformTool);

    connect(ui->toolSystemCFolderSelect,SIGNAL(clicked()),this,SLOT(selectSystemCDir()));
    connect(ui->toolMinGWFolderSelect,SIGNAL(clicked()),this,SLOT(selectMinGWDir()));
    connect(ui->toolButtonWorkFolder,SIGNAL(clicked()),this,SLOT(selectWorkDir()));
    connect(ui->toolWaveformToolSelect,SIGNAL(clicked()),this,SLOT(selectWaveformTool()));
}

FoldersConfigurationPage::~FoldersConfigurationPage() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class View/FoldersConfigurationPage (Pages.h)" << std::endl;
#endif
    delete ui;
}

void FoldersConfigurationPage::selectSystemCDir() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/FoldersConfigurationPage::selectSystemCDir (Pages.h)" << std::endl;
#endif

    ui->lineEditSystemCFolder->setText(QFileDialog::getExistingDirectory(this,trUtf8("Select the SystemC folder")));

}

void FoldersConfigurationPage::selectMinGWDir() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/FoldersConfigurationPage::selectMinGWDir (Pages.h)" << std::endl;
#endif

    ui->lineEditMinGWFolder->setText(QFileDialog::getExistingDirectory(this,trUtf8("Select the MinGW folder")));

}

void FoldersConfigurationPage::selectWaveformTool() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/FoldersConfigurationPage::selectWaveformTool (Pages.h)" << std::endl;
#endif

    QFileDialog dial(this);
    dial.setFilter(QDir::Files | QDir::Executable);
    ui->lineEditWaveformTool->setText( dial.getOpenFileName(this,trUtf8("Select the Waveform viewer")) );
}

void FoldersConfigurationPage::selectWorkDir() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/FoldersConfigurationPage::selectWorkDir (Pages.h)" << std::endl;
#endif

    ui->lineEditWorkFolder->setText(QFileDialog::getExistingDirectory(this,trUtf8("Select a Work folder"),QDir::homePath()));

}

const QString FoldersConfigurationPage::getMinGWFolder() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/FoldersConfigurationPage::getMinGWFolder (Pages.h)" << std::endl;
#endif
    return ui->lineEditMinGWFolder->text();
}

const QString FoldersConfigurationPage::getWaveformTool() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/FoldersConfigurationPage::getWaveformTool (Pages.h)" << std::endl;
#endif
    return this->ui->lineEditWaveformTool->text();
}

const QString FoldersConfigurationPage::getSystemCFolder() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/FoldersConfigurationPage::getSystemCFolder (Pages.h)" << std::endl;
#endif
    return ui->lineEditSystemCFolder->text();
}

const QString FoldersConfigurationPage::getWorkFolder() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/FoldersConfigurationPage::getWorkFolder (Pages.h)" << std::endl;
#endif
    return ui->lineEditWorkFolder->text();
}

////////////////////////// FoldersConfigurationPage //////////////////////////
