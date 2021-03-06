﻿/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Control.h
* Copyright (C) 2014 - 2016 LEDS - Univali <zeferino@univali.br>
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
* 31/05/2016 - 1.1     - Eduardo Alves da Silva      | First refactoring
*    ||      - ||      - Sérgio Vargas Júnior        |      ||
* ----------------------------------------------------------------------------
* 10/12/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*
*/


#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <QObject>
#include <QColor>

// Forward declaration classes
// View
class MainWindow;
class AnalysisOptions;
// Model
class SystemConfiguration;
class Experiment;
class SystemOperation;
class SystemExecution;
class DataReport;
// Control
class SimulationPerformer;
class ThreadManager;
class EnvironmentConfiguration;

// Qt
QT_BEGIN_NAMESPACE
class QFile;
class QXmlStreamWriter;
class QXmlStreamReader;
class QCloseEvent;
class QElapsedTimer;
QT_END_NAMESPACE

// String Constants
const QString DEFAULT_CONFIGURATION_FILE = "etc/DefaultConfiguration.drsf";
const QString APPLICATION_NAME = "RedScarf";
const QString WORK_DIR = "RedScarf_workspace";
const QString SIMULATOR_CONF_FILE = "simconf.conf";
const QString RESULT_SIMLATION_SETUP_FILENAME = "Configuration.rsf";
const QString RESULT_SIMULATION_DIRS_FILENAME = "SimulationFolders.dir";

class Control : public QObject {
    Q_OBJECT
private:
    // View
    MainWindow* mainWindow;

    // Control
    QFile* configFile;
    EnvironmentConfiguration* environmentConfiguration;

    QList<SystemExecution> executions;

    QList<SimulationPerformer *>* exes;
    ThreadManager* threadManager;
    enum FinishCode{ Success = 0,ExecuteFailed, InputsError, Cancel};

    bool analysisOk;
    // Dirs and legends of experiments performed
    QStringList simulationFolders;
    QStringList legends;

    QElapsedTimer* timer;

    QColor informationColor;
    QColor warningColor;
    QColor errorColor;

    // Control( (this) with view signals)
    void establishConnections();

    bool saveConfiguration();
    bool saveAsConfiguration();
    bool loadConfiguration();

    void saveSimulationSetupFiles(QString workDir);

    QString dirSetup(QString selection, QString msgNotConfigured);

    bool inputsOk(const QList<SystemConfiguration>& sysConfs,const QList<Experiment>& experiments,const SystemOperation& sysOp);

    void finishSimulation(FinishCode code);

    /**
     * @brief getReportData Get the simulation analyzed report data
     * Read the logs of simulation according type of analysis defined
     * in \p aop parameter. If latency distribution analysis read logs according
     * operation frequencies selected, otherwise read the the general logs.
     *
     * @param aop Options to be analyzed, e.g if the analysis is latency distribution or not.
     * @return A vector with a list of data of analysis performed successfully, otherwise return NULL.
     * If latency distribution analysis, each item of data is an object
     * with \p latencyCycle, \p packetCount and \p packetPercentage.
     * If general analysis, each item of data is an object with all other
     * information as \p fClk (clock frenquency), \p accNbOfPck (number os packets analyzed) etc.
     * An exempla of the structure format returned is:
     * --- Latency Distribution ---
     * Vector[0] (experiment '...' @ '...' MHz) with 100 packets
     *   List[0] -> latencyCycle: 12, packetCount: 50 (frequency), packetPercentage: 50% (relative frequency of all packets)
     *   List[1] -> latencyCycle: 17, packetCount: 37 (frequency), packetPercentage: 37% (relative frequency of all packets)
     *   List[2] -> latencyCycle: 28, packetCount: 12 (frequency), packetPercentage: 12% (relative frequency of all packets)
     *   List[3] -> latencyCycle: 50, packetCount: 1  (frequency), packetPercentage: 1 % (relative frequency of all packets)
     * Vector[1] (experiment '...' @ '...' MHz) with 100 packets
     *   List[0] -> latencyCycle: 17, packetCount: 30 (frequency), packetPercentage: 30% (relative frequency of all packets)
     *   List[1] -> latencyCycle: 20, packetCount: 40 (frequency), packetPercentage: 40% (relative frequency of all packets)
     *   List[2] -> latencyCycle: 25, packetCount: 20 (frequency), packetPercentage: 20% (relative frequency of all packets)
     *   List[3] -> latencyCycle: 30, packetCount: 10 (frequency), packetPercentage: 10% (relative frequency of all packets)
     * .... if more options selected
     *
     * --- General Report ---
     * Vector[0] (exp 'a')
     *   List[0] -> fClk: 100MHz | accNbOfPck: 250 | idealAvgLatency: 20 | avgLatencyCycles: 31 | .....
     *   List[1] -> fClk: 90 MHz | accNbOfPck: 250 | idealAvgLatency: 20 | avgLatencyCycles: 35 | .....
     *   List[2] -> fClk: 80 MHz | accNbOfPck: 250 | idealAvgLatency: 20 | avgLatencyCycles: 38 | .....
     *   List[3] -> fClk: 70 MHz | accNbOfPck: 250 | idealAvgLatency: 20 | avgLatencyCycles: 40 | .....
     *   .... if more frequencies simulated
     * Vector[0] (exp 'b')
     *   List[0] -> fClk: 100MHz | accNbOfPck: 250 | idealAvgLatency: 20 | avgLatencyCycles: 31 | .....
     *   List[1] -> fClk: 90 MHz | accNbOfPck: 250 | idealAvgLatency: 20 | avgLatencyCycles: 35 | .....
     *   List[2] -> fClk: 80 MHz | accNbOfPck: 250 | idealAvgLatency: 20 | avgLatencyCycles: 38 | .....
     *   List[3] -> fClk: 70 MHz | accNbOfPck: 250 | idealAvgLatency: 20 | avgLatencyCycles: 40 | .....
     *   .... if more frequencies simulated
     * ..... if more experiments performed
     */
    QVector<QList<DataReport* >* >* getReportData(AnalysisOptions* aop);

public:

    explicit Control(QObject *parent = 0);

    void startApp();
    ~Control();

#ifdef Q_OS_MAC
    void macOpenFile(QString macOpenFile);
#endif

    
private slots:
    
    // Actions
    void newSystem();
    void loadSystem();
    void loadDefaultSystem();
    void saveSystem();
    void saveAsSystem();
    void saveAsDefaultSystem();
    void editOptions();
    void changeLanguage(QString languageName);
    void exitApplication(QCloseEvent* event);

    void loadSimulationResults();
    void saveSimulationResults();
    void generateCSVSimulationReport(AnalysisOptions* aop);
    void loadSimulationDir();

    // System and traffic configuration
    void generateTrafficConfigurationFile();

    // System simulation
    void runSimulations();
    void cancelSimulation();
    void removeExe(QObject*);
    void finishExecution();
    void updateStatusExecution(int status);
    void executeUnsuccessful();

    // Performance analysis
    void generateAnalysis(float lower,float upper,int type);
    void viewWaveform();
    void viewGraphic(AnalysisOptions* aop);
    void viewReport(AnalysisOptions* aop);
    void analysisEnd(bool success);

    // Environment Options
    void applySettings(EnvironmentConfiguration*, QString languageName);

public slots:
    void folderCompressorWorkCompleted(bool success,int opType,QString workDest);

};

#endif // CONTROL_H
