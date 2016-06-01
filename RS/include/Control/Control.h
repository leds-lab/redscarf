/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Control.h
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


#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>

// Forward declaration classes
// View
class MainWindow;
class AnalysisOptions;
// Model
class SystemParameters;
class TrafficPatternManager;
class TrafficParameters;
class ExperimentManager;
class DataReport;
// Control
class Builder;
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
const QString RESULT_SIMLATION_SETUP_FILENAME = "Configuration.rsf";
const QString RESULT_SIMULATION_DIRS_FILENAME = "SimulationFolders.dir";

class Control : public QObject {
    Q_OBJECT

private:
    // View
    MainWindow* mainWindow;
    // Model
    SystemParameters*      systemParameters;
    TrafficPatternManager* trafficPatternManager;
    ExperimentManager*     experimentManager;
    // Model copy system parameters for analysis
    unsigned int xSize;
    unsigned int ySize;
    unsigned int dataWidth;
    unsigned int vcdOption;
    bool analysisOk;
    // Control
    QFile* configFile;
    EnvironmentConfiguration* conf;
    Builder* builder;
    bool buildSuccessfully;

    QList<SimulationPerformer *>* exes;
    ThreadManager* threadManager;
    enum FinishCode{ Success = 0,ExecuteFailed, BuildFailed, InputsError, Cancel};

    // Dirs and legends of experiments performed
    QList<QString*>* simulationFolders;
    QStringList legends;

    QElapsedTimer* timer;

    // Control( (this) with view signals)
    void establishConnections();

    bool saveConfiguration();
    bool saveAsConfiguration();
    bool loadConfiguration();
    bool clearTrafficPatterns();

    QString dirSetup(QString selection, QString msgNotConfigured);

    bool inputsOk();
    unsigned int calcAmountExperimentsExecutions();

    void copySystemParameters();

    void finishSimulation(FinishCode code);

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
    void clearSystem();
    void editOptions();
    void changeLanguage(QString languageName);
    void exitApplication(QCloseEvent* event);
    void loadSimulationResults();
    void saveSimulationResults();
    void generateCSVSimulationReport(AnalysisOptions* aop);

    void folderCompressorWorkCompleted(bool success,int opType);

    // System and traffic configuration
    void updateSizeSystem(unsigned int xSize,unsigned int ySize);
    void updateChannelWidth(unsigned int width);
    void nodeSelected(unsigned int posX,unsigned int posY);
    void trafficPatternStatusChanged(unsigned int posX,unsigned int posY, unsigned int trafficNum, bool state);
    void editTrafficPattern(unsigned int xPos,unsigned int yPos,unsigned int trafficNum);
    void applyTrafficConfiguration(TrafficParameters* configuration,unsigned int trafficNum);
    void applyAndReplicateTrafficConfiguration(TrafficParameters* configuration,unsigned int trafficNum);
    void previewTrafficConfiguration(int typePreview);

    // System simulation
    void experimentStateChanged(int numExperiment,bool state);
    void configurationExperimentChanged(int opCode,int numExperiment,int newValue);
    void stopOptionUpdated(int indice);
    void stopTimeNsUpdated(int newTime);
    void stopTimeCyclesUpdated(int newTime);
    void vcdOptionUpdated(int indice);
    void fClkFirstUpdated(double newValue);
    void fClkLastUpdated(double newValue);
    void fClkStepUpdated(double newValue);
    void fClkStepTypeUpdated(int newValue);
    void runBuilder();
    void buildUnsuccessfull();
    void finishBuilding();
    void receiveDirsExecution(QList<QString*>* dirs);
    void cancelSimulation();
    void removeExe(QObject*);
    void finishExecution();
    void updateStatusExecution(int status);
    void executeUnsuccessful();

    // Performance analysis
    void generateAnalysis(float lower,float upper);
    void viewWaveform();
    void viewGraphic(AnalysisOptions* aop);
    void viewReport(AnalysisOptions* aop);
    void analysisEnd(bool success);


    // Environment Options
    void applySettings(EnvironmentConfiguration*, QString languageName);
};

#endif // CONTROL_H
