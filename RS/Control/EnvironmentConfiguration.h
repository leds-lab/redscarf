/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* EnvironmentConfiguration.h
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

#ifndef __ENVIRONMENTCONFIGURATION_H__
#define __ENVIRONMENTCONFIGURATION_H__

#include <QObject>
#include <QMap>
#include <QStringList>

/*!
 * \brief The EnvironmentConfiguration class contains the main options
 * to environment setup.
 *
 * The options configurable are Simulator location, plugins folder, waveform tool
 * and environment language.
 *
 * This class also is responsible by load language options available.
 *
 * Too read and write setup os environment.
 */
class EnvironmentConfiguration : public QObject {
    Q_OBJECT

private:
    QString simulatorLocation;
    QString pluginsFolders;
    QString waveformTool;
    QString workFolder;
    QPair<QString,QStringList> language;
    QMap<QString,QStringList> availableLanguages;

    unsigned int threadNumber;

    /*!
     * \brief readSetup Read settings file for load configurations
     * \return True if read successfully executed, otherwise false
     */
    void readSetup();
public:
    /*!
     * \brief EnvironmentConfiguration constructor with parent and option for load or not settings
     * \param parent Parent of this object
     * \param loadConf True if is need load settings of file config.ini, otherwise false
     */
    explicit EnvironmentConfiguration(QObject *parent = 0,bool loadConf = false);

    /*!
     * \brief getSimulatorLocation Get a string with absolute path of simulator location.
     * \return Simulator location.
     */
    inline QString getSimulatorLocation() const { return this->simulatorLocation; }
    /*!
     * \brief getMinGWFolder Get a string with absolute path of MinGW root folder (only Windows)
     * \return MinGW root folder
     */
    inline QString getPluginsFolder() const { return this->pluginsFolders; }
    /*!
     * \brief getWaveformTool Get a string with absolute path of Waveform tool executable
     * \return Path of executable Waveform tool
     */
    inline QString getWaveformTool() const { return this->waveformTool; }
    /*!
     * \brief getWorkFolder Get a string with path for work directory that will be contain
     * the simulation, logs and reports files.
     * \return Path to work directory
     */
    inline QString getWorkFolder() const { return this->workFolder; }

    /*!
     * \brief getThreadNumber Get the thread number for the simulation
     * \return Number of thread to simulation
     */
    inline unsigned int getThreadNumber() const { return this->threadNumber; }
    /*!
     * \brief getLanguageFiles Get the filenames in translations
     * directory of \p languageName parameter.
     * \param languageName The language name, show in view
     * \return Filenames of the language in translations directory
     */
    QStringList getLanguageFiles(QString languageName) const;
    /*!
     * \brief getLanguageFilesSelected Get the filenames of selected language
     * \return Language filenames
     */
    QStringList getLanguageFilesSelected() const;

    /*!
     * \brief getLanguageNameSelected Get the language name currently selected
     * \return The language name
     */
    QString getLanguageNameSelected() const;
    /*!
     * \brief getLanguageNames Get language name of all languages loaded on language.ini file
     * \return String list of all available languages
     */
    QStringList getLanguageNames() const;

    /*!
     * \brief setSimulatorLocation Set the simulator location
     * \param location New simulator location
     */
    inline void setSimulatorLocation(const QString location) { this->simulatorLocation = location; }
    /*!
     * \brief setMinGWFolder Set the MinGW root folder (only Windows)
     * \param folder MinGW folder
     */
    inline void setPluginsFolder( const QString folder ) { this->pluginsFolders = folder; }
    /*!
     * \brief setWaveformTool Set the path for WaveformTool executable
     * \param executablePath Executable path
     */
    inline void setWaveformTool( const QString executablePath ) { this->waveformTool = executablePath; }
    /*!
     * \brief setWorkFolder Set the path to work folder
     * \param folder Work folder
     */
    inline void setWorkFolder(const QString folder) { this->workFolder = folder; }
    /*!
     * \brief setLanguageSelected Set language selected
     * \param languageName Language selected
     */
    void setLanguageSelected(const QString languageName);

    /*!
     * \brief setThreadNumber Set the thread number for the parallel simulation
     * \param number Number of threads for the simulation
     */
    inline void setThreadNumber(unsigned int number) { this->threadNumber = number; }

signals:
    /*!
     * \brief languageChanged Signal emitted when the language is changed
     * \param languageFilenames The new language filenames
     */
    void languageChanged(QStringList languageFilenames);

public slots:
    /*!
     * \brief writeSetup Write settings file for store setup
     */
    void writeSetup();
};

#endif // ENVIRONMENTCONFIGURATION_H
