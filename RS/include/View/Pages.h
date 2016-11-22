/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Pages.h
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
* 20/11/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*
*/

#ifndef __PAGES_H__
#define __PAGES_H__

#include <QWidget>

namespace Ui {
    class GeneralConfigurationPage;
    class FoldersConfigurationPage;
}

class EnvironmentConfiguration;

class GeneralConfigurationPage : public QWidget {
    Q_OBJECT

private:
    Ui::GeneralConfigurationPage* ui;
public:
    explicit GeneralConfigurationPage(QStringList availableLanguages,
            QString selectedLanguage,int numThreads, QWidget *parent = 0);
    ~GeneralConfigurationPage();

    const QString getSelectedLanguage();
    int getThreadNumber();

};

class FoldersConfigurationPage : public QWidget {
    Q_OBJECT

private:
    Ui::FoldersConfigurationPage* ui;
public:
    explicit FoldersConfigurationPage(QString simulator, QString plugins,
            QString work, QString waveformTool, QWidget* parent = 0);

    const QString getSimulatorLocation();
    const QString getPluginsFolder();
    const QString getWorkFolder();
    const QString getWaveformTool();

    ~FoldersConfigurationPage();


private slots:
    void selectSimulator();
    void selectPluginsFolder();
    void selectWorkDir();
    void selectWaveformTool();

};

#endif // PAGES_H
