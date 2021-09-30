/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* EnvironmentConfiguration.cpp
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

#include "EnvironmentConfiguration.h"
#include "Control.h"

#include <QApplication>
#include <QThread>

#include <QSettings>
#include <QDir>
#include <QPair>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

EnvironmentConfiguration::EnvironmentConfiguration(QObject *parent, bool loadConf) :
    QObject(parent) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Control/EnvironmentConfiguration" << std::endl;
#endif

    if(loadConf) {
        this->readSetup();
    }

}

void EnvironmentConfiguration::readSetup() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/EnvironmentConfiguration::readConfigurations" << std::endl;
#endif

    QSettings settings(qApp->applicationDirPath()+"/etc/config.ini",QSettings::IniFormat);

    settings.beginGroup("General");
    this->threadNumber = settings.value("Threads", QThread::idealThreadCount() ).toUInt();
    settings.endGroup();

    settings.beginGroup("Dirs");
    this->simulatorLocation = settings.value("Simulator").toString();
    this->pluginsFolders = settings.value("Plugins").toString();
    this->workFolder = settings.value("Work",QDir::homePath()+"/"+WORK_DIR).toString();
    this->waveformTool = settings.value("WaveformTool","").toString();
    settings.endGroup();

    int size = settings.beginReadArray("AvailableLanguages");
    for(int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        availableLanguages.insert( QString::fromUtf8(settings.value("Language").toString().toStdString().c_str()),settings.value("Files").toStringList() );
    }
    settings.endArray();
    if( availableLanguages.isEmpty() ) {
        QPair<QString,QStringList> englishDefault = QPair<QString,QStringList>(tr("English"),QStringList("RedScarf_en"));
        this->availableLanguages.insert( englishDefault.first, englishDefault.second );
    }

    settings.beginGroup("Language");
    QString selectedName = settings.value("SelectedLanguage","English").toString();
    QStringList selectedValues = settings.value("SelectedFiles","RedScarf_en").toStringList();
    language = QPair<QString,QStringList>(selectedName,selectedValues);
    settings.endGroup();

}

void EnvironmentConfiguration::writeSetup() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/EnvironmentConfiguration::writeSetup" << std::endl;
#endif

    QSettings settings(qApp->applicationDirPath()+"/etc/config.ini",QSettings::IniFormat);

    settings.beginGroup("General");
    settings.setValue("Threads", this->threadNumber);
    settings.endGroup();

    settings.beginGroup("Dirs");
    settings.setValue("Simulator",this->simulatorLocation);
    settings.setValue("Plugins",this->pluginsFolders);
    settings.setValue("Work",this->workFolder);
    settings.setValue("WaveformTool",this->waveformTool);
    settings.endGroup();

    settings.beginWriteArray("AvailableLanguages");
    QMapIterator<QString,QStringList> it(availableLanguages);
    for( int i = 0; it.hasNext(); i++ ) {
        it.next();
        settings.setArrayIndex(i);
        settings.setValue("Language",it.key());
        settings.setValue("Files",it.value());
    }
    settings.endArray();

    settings.beginGroup("Language");
    settings.setValue("SelectedLanguage",language.first);
    settings.setValue("SelectedFiles",language.second);
    settings.endGroup();

}

QStringList EnvironmentConfiguration::getLanguageFiles(QString languageName) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/EnvironmentConfiguration::getLanguageFiles" << std::endl;
#endif

    return availableLanguages.find(languageName).value();

}

QStringList EnvironmentConfiguration::getLanguageFilesSelected() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/EnvironmentConfiguration::getLanguageFilesSelected" << std::endl;
#endif

    return language.second;

}

QStringList EnvironmentConfiguration::getLanguageNames() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/EnvironmentConfiguration::getLanguageNames" << std::endl;
#endif

    QStringList names(availableLanguages.keys());
    return names;

}

QString EnvironmentConfiguration::getLanguageNameSelected() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/EnvironmentConfiguration::getLanguageNameSelected" << std::endl;
#endif

    return language.first;

}

void EnvironmentConfiguration::setLanguageSelected(const QString languageName) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/EnvironmentConfiguration::setLanguageSelected" << std::endl;
#endif

    QMap<QString,QStringList>::iterator it = availableLanguages.find( languageName );
    language = QPair<QString,QStringList>(it.key(),it.value());

    emit languageChanged(language.second);
}
