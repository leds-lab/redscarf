/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Control.cpp
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
* 20/11/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
* ----------------------------------------------------------------------------
*
*/

#include "Main.h"

// Qt/C++
#include <QFile>
#include <QDir>
#include <QThread>
#include <QThreadPool>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QCloseEvent>
#include <QElapsedTimer>
#include <QTextStream>
#include <QSettings>
#include <QDateTime>
#include <math.h>

// Control
#include "Control/Control.h"
//#include "Control/XmlConfigParser.h"
#include "Control/SimulationPerformer.h"
#include "Control/ThreadManager.h"
#include "Control/WaveformViewer.h"
#include "Control/ExternalWaveformViewer.h"
#include "Control/Analyzer.h"
#include "Control/EnvironmentConfiguration.h"
#include "Control/FolderCompressor.h"

// View
#include "View/MainWindow.h"
#include "View/TrafficConfigurationDialog.h"
#include "View/AnalysisOptions.h"
#include "View/ReportDialog.h"
#include "View/GetSelectedItemsDialog.h"
#include "View/ConfigDialog.h"
#include "View/CustomPlotter.h"
#include <QInputDialog>

// Model
#include "Model/System/SystemParameters.h"
#include "Model/System/SystemOperation.h"
#include "Model/System/Experiment.h"
#include "Model/System/SystemDefines.h"
#include "Model/Traffic/TrafficModelGenerator.h"
#include "Model/TimeOperation.h"
#include "Model/Analysis/DataReport.h"
#include "Model/Analysis/ReportReader.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

Control::Control(QObject *parent) :
    QObject(parent) {

#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Control/Control" << std::endl;
#endif

    this->mainWindow = (MainWindow* ) parent;

    this->systemParameters      = new SystemParameters();

    this->exes = new QList<SimulationPerformer *>();
    this->timer = new QElapsedTimer();

    this->configFile = NULL;
    this->conf = new EnvironmentConfiguration(this,true);

    this->simulationFolders = NULL;
    this->threadManager = NULL;

    this->analysisOk = false;
}

Control::~Control() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Control/Control" << std::endl;
#endif

    for( int i = 0; i < exes->size(); i++ ) {
        SimulationPerformer* exe = exes->at(i);
        if( exe != NULL ) {
            exe->cancel();
            delete exe;
        }
    }
    exes->clear();

    delete this->systemParameters;
    delete this->exes;
    delete this->timer;
    conf->writeSetup();
    delete conf;
    if( this->threadManager != NULL ) {
        threadManager->stopThreads();
        delete threadManager;
    }
    if(simulationFolders != NULL) {
        simulationFolders->clear();
        delete simulationFolders;
    }
    SystemDefines* defSis = SystemDefines::getInstance();
    delete defSis;
}

void Control::establishConnections() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::establishConnections" << std::endl;
#endif
    connect(this->mainWindow,SIGNAL(newSystem()),this,SLOT(newSystem()));
    connect(this->mainWindow,SIGNAL(loadSystem()),this,SLOT(loadSystem()));
    connect(this->mainWindow,SIGNAL(loadDefaultSystem()),this,SLOT(loadDefaultSystem()));
    connect(this->mainWindow,SIGNAL(saveSystem()),this,SLOT(saveSystem()));
    connect(this->mainWindow,SIGNAL(saveAsSystem()),this,SLOT(saveAsSystem()));
    connect(this->mainWindow,SIGNAL(saveAsDefaultSystem()),this,SLOT(saveAsDefaultSystem()));
    connect(this->mainWindow,SIGNAL(exitApplication(QCloseEvent*)),this,SLOT(exitApplication(QCloseEvent*)));
    connect(this->mainWindow,SIGNAL(editOptions()),this,SLOT(editOptions()));
    connect(this->mainWindow,SIGNAL(changeLanguage(QString)),this,SLOT(changeLanguage(QString)));
    connect(this->conf,SIGNAL(languageChanged(QStringList)),mainWindow,SLOT(translate(QStringList)) );
    connect(this->mainWindow,SIGNAL(loadSimulationResults()),this,SLOT(loadSimulationResults()));
    connect(this->mainWindow,SIGNAL(saveSimulationResults()),this,SLOT(saveSimulationResults()));
    connect(this->mainWindow,SIGNAL(generateCSVSimulationReport(AnalysisOptions*)),this,SLOT(generateCSVSimulationReport(AnalysisOptions*)));

    // TODO TEMP
    connect(this->mainWindow,&MainWindow::generateTCF,this,&Control::generateTrafficConfigurationFile);

    connect(this->mainWindow,&MainWindow::runSimulation,this,&Control::runSimulations);

    connect(this->mainWindow,SIGNAL(cancel()),this,SLOT(cancelSimulation()));

    connect(this->mainWindow,SIGNAL(generateAnalysis(float,float)),this,SLOT(generateAnalysis(float,float)));
    connect(this->mainWindow,SIGNAL(viewWaveform()),this,SLOT(viewWaveform()));
    connect(this->mainWindow,SIGNAL(viewGraphic(AnalysisOptions*)),this,SLOT(viewGraphic(AnalysisOptions*)));
    connect(this->mainWindow,SIGNAL(viewReport(AnalysisOptions*)),this,SLOT(viewReport(AnalysisOptions*)));

}

#ifdef Q_OS_MAC
void Control::macOpenFile(QString macOpenFile) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::macOpenFile" << std::endl;
#endif

    if( !macOpenFile.isNull() ) {
        if( macOpenFile.endsWith(".rsf") ) {
            configFile = new QFile(macOpenFile);
            loadConfiguration();
            this->mainWindow->initConsole();
        }
    }

}
#endif

void Control::startApp() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::startApp" << std::endl;
#endif

    if(!mainWindow) {
        mainWindow = new MainWindow();
        this->setParent(mainWindow);
    }

    this->mainWindow->configureLanguages(conf);

    this->establishConnections();

    QStringList files = QApplication::arguments().filter(".rsf");
    if( files.size() > 0 ) {
        QString file = files.at(0);
        configFile = new QFile(file);
        bool carregou = loadConfiguration();
        QDir::setCurrent(qApp->applicationDirPath());
        if( !carregou ) {
            loadDefaultSystem();
        }
    } else {
        QDir::setCurrent(qApp->applicationDirPath());
        this->loadDefaultSystem();
    }

    QSettings windowSettings;
    mainWindow->restoreGeometry(windowSettings.value("mainWindowGeometry").toByteArray());
    mainWindow->restoreState(windowSettings.value("mainWindowState").toByteArray());

    this->mainWindow->clearConsole();
    this->mainWindow->initConsole();

    if( conf->getSimulatorLocation().isEmpty() ) {
        QString simulatorLocation = this->mainWindow->dialogLoadFile(trUtf8("Select Simulator executable"),"");
        simulatorLocation = simulatorLocation.trimmed();
        if(simulatorLocation.isEmpty()) {
            this->mainWindow->printConsole(trUtf8("Simulator executable not configured\n"
                                                  "The simulation wouldn't run!"),Qt::red);
        } else {
            conf->setSimulatorLocation( simulatorLocation );
        }
    }

    if( conf->getPluginsFolder().isEmpty() ) {
        conf->setPluginsFolder( this->dirSetup(
            trUtf8("Select simulator plugins folder"),
            trUtf8("Plugins folder not configured."
                   "\nMaybe the simulator wouldn't run!")) );
    }

    this->mainWindow->setWindowModified(false);
    this->mainWindow->show();

}

QString Control::dirSetup(QString selection,QString msgNotConfigured) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::dirSetup" << std::endl;
#endif

    QString conteudo = this->mainWindow->selectSystemFolder(selection);

    conteudo = conteudo.trimmed();
    if(conteudo.endsWith("/")) {
        conteudo.chop(1);
    }

    if(conteudo.isEmpty()) {
        this->mainWindow->printConsole(msgNotConfigured);
    }

    return conteudo;
}

bool Control::loadConfiguration() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::loadConfiguration" << std::endl;
#endif

    if( !configFile ) {
        QString fileName = this->mainWindow->dialogLoadFile(trUtf8("Open a setup file"),
                                                            trUtf8("RedScarf(*.rsf)"),
                                                            QDir::homePath());
        if(fileName.isEmpty()) {
            return false;
        }
        this->configFile = new QFile(fileName);
    }

    if( !configFile->open(QIODevice::ReadOnly | QIODevice::Text) ) {
        this->mainWindow->openFileError(configFile->errorString());
        delete this->configFile;
        this->configFile = NULL;
        return false;
    }
/*
    /// Carregar
    XmlConfigParser* parser = new XmlConfigParser(systemParameters,trafficPatternManager,experimentManager);
    parser->loadXML(configFile);

    this->mainWindow->updateView( this->systemParameters->getXSize(),
            this->systemParameters->getYSize(),
                                  this->systemParameters->getZSize(),
            this->systemParameters->getDataWidth(),
            this->experimentManager,parser->getStopOption(),
            parser->getStopTime_ns(),parser->getStopTime_cycles(),parser->getVcdOption(),
            parser->getFClk1(),parser->getFClk2(),parser->getFClkStepType(), parser->getFClkStep() );
    delete parser;

*/
    configFile->close();
    this->mainWindow->printConsole(trUtf8("Configuration file successfuly loaded"));

    return true;
}

bool Control::saveConfiguration() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::saveConfiguration" << std::endl;
#endif

    if( !configFile ) {
        QString fileName = this->mainWindow->dialogSaveFile(trUtf8("Save a setup file"),
                                                            trUtf8("RedScarf(*.rsf)"),
                                                            QDir::homePath());
        if(fileName.isEmpty()) {
            return false;
        }
        if(!fileName.endsWith(".rsf")) {
            fileName += ".rsf";
        }
        this->configFile = new QFile(fileName,this);
    }

    if( !configFile->open(QIODevice::WriteOnly | QIODevice::Text) ) {
        this->mainWindow->openFileError(configFile->errorString());
        delete this->configFile;
        this->configFile = NULL;
        return false;
    }
/*
    /// Salvar
    XmlConfigParser* parser = new XmlConfigParser(systemParameters,trafficPatternManager,experimentManager);
    parser->saveXML(configFile);
    delete parser;
*/
    this->configFile->close();
    this->mainWindow->printConsole(trUtf8("Configuration file successfully saved"));
    return true;
}

bool Control::saveAsConfiguration() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::saveAsConfiguration" << std::endl;
#endif

    QFile* tmp = configFile;
    configFile = NULL;

    if( this->saveConfiguration() ) {
        delete tmp;
        return true;
    } else {
        configFile = tmp;
        return false;
    }
}

/**
 * @brief Verify if:
 *  (1) There are some traffic pattern configured and active;
 *  (2) There are some identical experiment configuration;
 *  (3) The system parameters are Ok
 */
bool Control::inputsOk() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::inputsOk" << std::endl;
#endif

    bool padroesAtivos = false;
    QSettings settings(qApp->applicationDirPath()+"/etc/system.ini",QSettings::IniFormat);
    settings.beginGroup("Traffic_Parameters");
    bool useDefault = settings.value("useDefault",true).toBool();
    QString alternativeFile = settings.value("alternative",QString()).toString();
    settings.endGroup();

    if(useDefault) {
        // Verificar se há padrões de tráfico ativos
// TODO fazer
//        for(unsigned int x = 0; x < this->systemParameters->getXSize(); x++) {
//            for(unsigned int y = 0; y < this->systemParameters->getYSize(); y++) {
//                for(unsigned int z = 0; z < this->systemParameters->getZSize(); z++) {
//                    Node* no = this->trafficPatternManager->getNode(COORDINATE_3D_TO_ID(x,y,z,
//                                                                                        systemParameters->getXSize(),
//                                                                                        systemParameters->getYSize()));
//                    if( no != NULL ) {
//                        for(unsigned int pattern = 0; pattern < MAX_PATTERNS; pattern++) {
//                            if( no->isPatternActive(pattern) ) {
//                                padroesAtivos = true;
//                                break;
//                            }
//                        }
//                    }
//                    if(padroesAtivos){
//                       break;
//                    }
//                }
//                if(padroesAtivos) {
//                    break;
//                }
//            }
//            if(padroesAtivos){
//               break;
//            }
//        }

        if(!padroesAtivos) {
            this->mainWindow->printConsole(trUtf8("<font color=red>There is no traffic pattern defined</font>"));
            return false;
        }
    } else {
        if(alternativeFile.isNull() || alternativeFile.isEmpty()) {
            this->mainWindow->printConsole(trUtf8("<font color=red>The alternative file isn't defined.<br />Please verify the section 'Traffic_Parameters' in system.ini file</font>"));
            return false;
        } else {
            QFile file(alternativeFile);
            if( !file.exists() ) {
                this->mainWindow->printConsole(trUtf8("<font color=red>The alternative file defined don't exists.<br />Please verify the section 'Traffic_Parameters' in system.ini file</font>"));
                return false;
            }
        }
    }

    // Verificar se há algum experimento repetido
// TODO fazer
    for(unsigned int i = 1; i < 5; i++) {
        // TODO
        Experiment* exp1 = 0;// this->experimentManager->getExperiment(i);
        if(exp1 != NULL) {
            if(exp1->isActive()) {
                for(unsigned int x = i+1; x < 6; x++) {
                    // TODO
                    Experiment* exp2 = 0;//this->experimentManager->getExperiment(x);
                    if(exp2 != NULL) {
                        if( exp2->isActive() && exp1->equals(exp2) ) {
                            this->mainWindow->printConsole(trUtf8("<font color=red>Configuration #%1 is identical to configuration #%2 (this is not allowed)</font>").arg(i).arg(x));
                            return false;
                        }
                    }
                }
            }
        }
    }


    // Verificar se os parâmetros FClk estão OK
//    unsigned int fClkStepType = this->systemParameters->getfClkStepType();
//    float fClkFirst = this->systemParameters->getfClkFirst();
//    float fClkLast = this->systemParameters->getfClkLast();
//    float fClkStep = this->systemParameters->getfClkStep();
    unsigned int fClkStepType = this->systemOperation->fClkStepType;
    float fClkFirst = this->systemOperation->fClkFirst;
    float fClkLast = this->systemOperation->fClkLast;
    float fClkStep = this->systemOperation->fClkStep;

    if ( fClkStepType == 0) {
        if (( fClkFirst <  fClkLast) && (fClkStep <  0)) {
            this->mainWindow->printConsole(trUtf8("<font color=red>If First_Fclk less than Last_Fclk, step value (inc) must be greather than 0</font>"));
            return false;
        }
        if ((fClkFirst >  fClkLast) && (fClkStep >  0)) {
            this->mainWindow->printConsole(trUtf8("<font color=red>If First_Fclk greather than Last_Fclk, step value (inc) must be less than 0</font>"));
            return false;
        }
        if ((fClkFirst != fClkLast) && (fClkStep == 0)) {
            this->mainWindow->printConsole(trUtf8("<font color=red>Step value (inc) must be different of 0</font>"));
            return false;
        }
    } else if(fClkStepType == 1) {
        if ((fClkFirst <  fClkLast) && (fClkStep >  0)) {
            this->mainWindow->printConsole(trUtf8("<font color=red>If First_Fclk less than Last_Fclk, step value (exp) must be less than 0</font>"));
            return false;
        }
        if ((fClkFirst >  fClkLast) && (fClkStep <  0)) {
            this->mainWindow->printConsole(trUtf8("<font color=red>If First_Fclk greather than Last_Fclk, step value (exp) must be greather than 0</font>"));
            return false;
        }
        if ((fClkFirst != fClkLast) && (fClkStep == 0)) {
            this->mainWindow->printConsole(trUtf8("<font color=red>Step value (exp) must be different of 0</font>"));
            return false;
        }
    }

    return true;
}

unsigned int Control::calcAmountExperimentsExecutions() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::calcAmountExperimentsExecutions" << std::endl;
#endif

    unsigned int numExecucoesExperimento = 0;

// TODO verificar
//    unsigned int fClkStepType = this->systemParameters->getfClkStepType();
//    float fClkFirst = this->systemParameters->getfClkFirst();
//    float fClkLast = this->systemParameters->getfClkLast();
//    float fClkStep = this->systemParameters->getfClkStep();
    unsigned int fClkStepType = this->systemOperation->fClkStepType;
    float fClkFirst = this->systemOperation->fClkFirst;
    float fClkLast = this->systemOperation->fClkLast;
    float fClkStep = this->systemOperation->fClkStep;

    if(fClkStep == 0) {
        return 1u;
    }

    switch(fClkStepType) {
        case 0: // INC
            numExecucoesExperimento = (unsigned int) (abs(fClkLast-fClkFirst)) / abs(fClkStep) + 1;
            if((fClkFirst+fClkStep*numExecucoesExperimento) != fClkLast ){
                if((fClkFirst+fClkStep*(numExecucoesExperimento-1)) !=  fClkLast) {
                    numExecucoesExperimento++;
                }
            }
            break;
        case 1: { // EXP
            int i = 0;
            numExecucoesExperimento = 1;
            float fClk = fClkFirst;
            float fClkPrevious;
            unsigned int tmpInt;
            if(fClkFirst > fClkLast) {
                while(fClk >= fClkLast) {
                    fClkPrevious = fClk;
                    tmpInt = (unsigned int) (fClkFirst/(exp(i*fClkStep))*10);
                    fClk = ((float) tmpInt)/10;
                    if((fClk >= fClkLast) && (fClk != fClkPrevious) ){
                        numExecucoesExperimento++;
                    }
                    i++;
                }
            } else if(fClkFirst < fClkLast) {
                while(fClk <= fClkLast) {
                    fClkPrevious = fClk;
                    tmpInt = (unsigned int) (fClkFirst/(exp(i*fClkStep))*10);
                    fClk = ((float) tmpInt)/10;
                    if((fClk <= fClkLast) && (fClk != fClkPrevious)){
                        numExecucoesExperimento++;
                    }
                    i++;
                }
            }
            break;
        }
    }
    return numExecucoesExperimento;

}

////////////////////////////// Slots //////////////////////////////

void Control::newSystem() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::newSystem" << std::endl;
#endif

    if( this->mainWindow->isWindowModified() ) {
        switch(this->mainWindow->saveChanges(APPLICATION_NAME,
               trUtf8("The configuration has been modified.\nDo you want to save your changes?"))) {
            case 0: // Save
                if(!this->saveConfiguration()) {
                    return;
                }
            case 1: // Discard
                break;
            case 2: // Cancel
                return;
        }
    }
    if(configFile) {
       delete configFile;
    }
    configFile = new QFile(DEFAULT_CONFIGURATION_FILE);
    if( loadConfiguration() ) {
        delete configFile;
        configFile = NULL;
//        clearTrafficPatterns();
        this->mainWindow->setWindowModified(false);
    }

}

void Control::loadSystem() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::loadSystem" << std::endl;
#endif

    if(this->mainWindow->isWindowModified()) {
        switch(this->mainWindow->saveChanges(APPLICATION_NAME,trUtf8("The configuration has been modified.\nDo you want to save your changes?"))) {
            case 0: // Save
                if(!this->saveConfiguration()) {
                    return;
                }
            case 1: // Discard
                break;
            case 2: // Cancel
                return;
        }
    }
    QString fileName = this->mainWindow->dialogLoadFile(trUtf8("Open a setup file"),
                                                        trUtf8("RedScarf(*.rsf)"),
                                                        QDir::homePath());
    if(fileName.isEmpty()) {
        return;
    } else {
        if(configFile) {
           delete configFile;
        }
        configFile = new QFile(fileName);
        if( loadConfiguration() ) {
            this->mainWindow->setWindowModified(false);
        }
    }

}

void Control::loadDefaultSystem() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::loadDefaultSystem" << std::endl;
#endif

    if( this->mainWindow->isWindowModified() ) {
        switch(this->mainWindow->saveChanges(APPLICATION_NAME,trUtf8("The configuration has been modified.\nDo you want to save your changes?"))) {
            case 0: // Save
                if(!this->saveConfiguration()) {
                    return;
                }
            case 1: // Discard
                break;
            case 2: // Cancel
                return;
        }
    }
    QFile* tmp = configFile;
    QDir dir("etc");
    if(!dir.exists()) {
        dir.mkpath(".");
    }
    configFile = new QFile(DEFAULT_CONFIGURATION_FILE);
    if(!configFile->exists()) {
        saveAsDefaultSystem();
    }
    if(loadConfiguration()) {
        delete configFile;
        configFile = tmp;
        this->mainWindow->setWindowModified(false);
    }

}

void Control::saveSystem() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::saveSystem" << std::endl;
#endif

    if( saveConfiguration() ) {
        this->mainWindow->setWindowModified(false);
    }

}

void Control::saveAsSystem() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::saveAsSystem" << std::endl;
#endif

    if(saveAsConfiguration()) {
        this->mainWindow->setWindowModified(false);
    }
}

void Control::saveAsDefaultSystem() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::saveAsDefaultSystem" << std::endl;
#endif

    QFile* tmp = configFile;
    configFile = new QFile(DEFAULT_CONFIGURATION_FILE);
    QDir dir("etc");
    if(!dir.exists()) {
        dir.mkpath(".");

    }
    if( saveConfiguration() ) {
        delete configFile;
        configFile = NULL;
    }
    configFile = tmp;
}

void Control::exitApplication(QCloseEvent *event) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::exitApplication" << std::endl;
#endif

    if( this->mainWindow->isWindowModified() ) {
        switch(this->mainWindow->saveChanges(APPLICATION_NAME,trUtf8("The configuration has been modified.\nDo you want to save your changes?"))) {
            case 0: // Save
                if(!this->saveConfiguration()) {
                    event->ignore();
                    return;
                }
            case 1: // Discard
                break;
            case 2: // Cancel
                event->ignore();
                return;
        }
    }

    QSettings settings;
    settings.setValue("mainWindowGeometry",mainWindow->saveGeometry());
    settings.setValue("mainWindowState",mainWindow->saveState());

    event->accept();
}

void Control::updateStatusExecution(int status) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::updateStatusExecution" << std::endl;
#endif
    this->mainWindow->updateProgressBar(status);

}

void Control::finishExecution() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::finishExecution" << std::endl;
#endif

    finishSimulation(Control::Success);

}

void Control::executeUnsuccessful() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::executeUnsuccessful" << std::endl;
#endif

    this->finishSimulation(Control::ExecuteFailed);
}

void Control::finishSimulation(FinishCode code) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::finishSimulation" << std::endl;
#endif

    switch (code) {
        case Control::ExecuteFailed :
            mainWindow->printConsole(trUtf8("<font color=red>Execute Failed</font>"));
            if(simulationFolders != NULL) {
                simulationFolders->clear();
                delete simulationFolders;
                simulationFolders = NULL;
            }
            delete this->threadManager;
            threadManager = NULL;
            break;
        case Control::InputsError :
            mainWindow->printConsole(trUtf8("<font color=red>Inputs error</font>"));
            break;
        case Control::Success: {
            char* tempo = TimeOperation::formatTime( qlonglong(timer->elapsed()) );
            mainWindow->printConsole(trUtf8("<font color=blue><br />All the simulations were run in %1</font>").arg(tempo));
            delete this->threadManager;
            threadManager = NULL;
            delete[] tempo;
            this->mainWindow->setActionSaveSimulationEnabled(true);
            break;
        }
        case Control::Cancel:
            mainWindow->printConsole( trUtf8("<font color=red>Simulation canceled</font>") );
            for( int i = 0; i < exes->size(); i++ ) {
                SimulationPerformer* exe = exes->at(i);
                if( exe != NULL ) {
                    exe->cancel();
                }
            }
            delete threadManager;
            threadManager = NULL;
            break;
    }

    this->exes->clear();
    this->mainWindow->enableRun();
    if( code == Control::Success ) {
        this->mainWindow->runAnalysis(); // Run analysis after simulation
    }

}

void Control::cancelSimulation() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::cancelSimulation" << std::endl;
#endif
    finishSimulation( Control::Cancel );
}

void Control::removeExe(QObject *obj) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::removeExe" << std::endl;
#endif

    SimulationPerformer* exe = (SimulationPerformer *) obj;
    this->exes->removeOne( exe );
}

void Control::generateAnalysis(float lower, float upper) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::generateAnalysis" << std::endl;
#endif
    if(simulationFolders == NULL) { // No simulation results
        this->mainWindow->printConsole( trUtf8("<font color=red>Impossible generate analysis because there is no system simulation results</font>") );
        return;
    }

    int size = simulationFolders->size();
    QStringList items;
    for(int i = 0; i < size; i++) {
        items.append( simulationFolders->at(i) );
    }

    for( int i = 0; i < size; i++ ) {
        QString item = items.at(i);
        int lio = item.lastIndexOf("/");
        QString diretorioExperimento = item.left( lio );
        items.replace(i, diretorioExperimento);
    }

    items.removeDuplicates();

    size = items.size();
    for( int i = 0; i < size; i++ ) {
        QString item = items.at(i);
        QDir dirResults = item + "/Results";
        bool removeOk = dirResults.removeRecursively();
        if( !removeOk ) {
            this->mainWindow->printConsole("<font color=yellow>Could not delete the previous analysis, the results will be added to the existing results</font>");
        }
    }

    this->analysisOk = true;

    this->mainWindow->setAnalysisOptionsEnabled(false);
    this->mainWindow->setActionSaveSimulationEnabled(false);
    this->mainWindow->setActionGenerateCSVEnabled(false);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    this->mainWindow->printConsole(trUtf8("<font color=green><br />Analyzing simulation results</font>"));
    // TODO enviar numero de elementos para o analisador e dataWidth
    Analyzer* analyzer = new Analyzer(this->simulationFolders,16,32,lower,upper);
    QThread* threadAnalisador = new QThread(this);
    threadAnalisador->setObjectName("Analyzer");
    connect(threadAnalisador,SIGNAL(started()),analyzer,SLOT(analyze()));
    connect(threadAnalisador,SIGNAL(finished()),analyzer,SLOT(deleteLater()));
    connect(threadAnalisador,SIGNAL(finished()),threadAnalisador,SLOT(deleteLater()));
    connect(analyzer,SIGNAL(finished(bool)),this,SLOT(analysisEnd(bool)));
    connect(analyzer,SIGNAL(finished(bool)),threadAnalisador,SLOT(quit()));
    connect(analyzer,SIGNAL(finished(bool)),analyzer,SLOT(deleteLater()));
    connect(analyzer,SIGNAL(sendMessage(QString)),mainWindow,SLOT(printConsole(QString)));
    analyzer->moveToThread(threadAnalisador);
    threadAnalisador->start();

}

void Control::analysisEnd(bool success) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::analysisEnd" << std::endl;
#endif

    this->analysisOk = success;
    this->mainWindow->setActionGenerateCSVEnabled(success);
    this->mainWindow->setAnalysisOptionsEnabled(true);
    this->mainWindow->setActionSaveSimulationEnabled(true);
    if( success ) {
        this->mainWindow->printConsole(trUtf8("<br /><font color=blue><b>DONE !!!</b></font>"));
        this->mainWindow->printConsole(trUtf8("<font color=blue>Now you can use the performance analysis tools</font> <br /> <br />"));
    }
    QApplication::beep();
    QApplication::alert(mainWindow);
    QApplication::restoreOverrideCursor();
}

void Control::viewWaveform() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::viewWaveform" << std::endl;
#endif
// TODO verificar
//    if( this->vcdOption == 0 ) {
    if( this->systemOperation->vcdOption == 0 ) {
        this->mainWindow->printConsole(trUtf8("<font color=red>No VCD file available</font>"));
    } else {
        if( simulationFolders != NULL ) {
            int size = simulationFolders->size();
            if( size == 0 ) {
                this->mainWindow->printConsole( trUtf8("<font color=red>Impossible view waveform because there is not system simulation results</font>") );
                return;
            }
            QString item;
            bool ok = true;
            if( size == 1 ) {
                item = simulationFolders->at(0);
            } else {
                QStringList dirs;
                for(int i = 0; i < size; i++) {
                    dirs.append( simulationFolders->at(i) );
                }
                QStringList items = AnalysisOptions::getVisibleStrings(dirs);
                item = QInputDialog::getItem(mainWindow,trUtf8("Choice Simulation"),trUtf8("Choice the simulation you want view"),items,0,false,&ok);
                item = dirs.at(items.indexOf(item));
            }

            if(ok && !item.isEmpty()) {
                QString waveTool = conf->getWaveformTool();
                QStringList args = waveTool.split(" ");
                args.removeFirst();
                WaveformViewer* waveViewer = new ExternalWaveformViewer(waveTool,args,this);
                connect(waveViewer,SIGNAL(finished(int,QProcess::ExitStatus)),waveViewer,SLOT(deleteLater()));
                connect(waveViewer,SIGNAL(sendMessage(QString)),mainWindow,SLOT(printConsole(QString)));
                waveViewer->viewWaveform(item);
            }

        } else {
            this->mainWindow->printConsole( trUtf8("<font color=red>Impossible view waveform because there is not system simulation results</font>") );
        }
    }


}

void Control::viewGraphic(AnalysisOptions *aop) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::viewGraphic" << std::endl;
#endif

    if( !analysisOk ) {
        this->mainWindow->printConsole( trUtf8("<font color=red>Impossible plot graphic because there is not analysis results</font>") );
    } else {

        QVector<QList<DataReport *> *> *data = getReportData(aop);
        if(data == NULL) {
            mainWindow->printConsole( trUtf8("<font color=red>Report file unavailable or this flow is null (no packet was transfered)</font>"));
            return;
        }

        // Selected graphic
        Plotter* plotter = NULL;
        plotter =  new CustomPlotter(mainWindow);

        connect(plotter,SIGNAL(sendMessage(QString)),mainWindow,SLOT(printConsole(QString)));
        connect(plotter,SIGNAL(finished(int)),plotter,SLOT(deleteLater()));
        plotter->viewGraphic(data,aop,legends);

        // Deallocating data memory
        for( int i = 0; i < data->size(); i++ ) {
            QList<DataReport *>* items = data->at(i);
            for (int j = 0; j < items->size(); ++j) {
                DataReport* d = items->at(j);
                delete d;
            }
            items->clear();
            delete items;
        }
        data->clear();
        delete data;
    }
    delete aop;

}

QVector<QList<DataReport* >* >* Control::getReportData(AnalysisOptions *aop) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::getReportData" << std::endl;
#endif

    int size = simulationFolders->size();
    // Get simulation folders
    QStringList folders;
    for(int i = 0; i < size; i++) {
        folders.append( simulationFolders->at(i) );
    }

    // Identifying what directories must be analyzed according with analysis type
    if( aop->isLatencyDistribution() ) {
        // Latency distribution (Histogram)
        if( folders.size() > 1 ) {
            GetSelectedItemsDialog* selectHistogramItemsWindow = new GetSelectedItemsDialog(folders,mainWindow);
            selectHistogramItemsWindow->setWindowTitle( trUtf8("Select items for latency distribution") );
            if( selectHistogramItemsWindow->exec() == QDialog::Accepted ) {
                folders = selectHistogramItemsWindow->getSelectedItems();
                if(folders.isEmpty()) {
                    this->mainWindow->printConsole(trUtf8("<font color=red>No selected items</font>"));
                    delete selectHistogramItemsWindow;
                    return NULL;
                }
                delete selectHistogramItemsWindow;
            } else {
                delete selectHistogramItemsWindow;
                return NULL;
            }
        }
    } else {
        // Normal report
        for( int i = 0; i < size; i++ ) {
            QString folder = folders.at(i);
            int lio = folder.lastIndexOf("/");
            QString expFolder = folder.left( lio );
            folders.replace(i, expFolder);
        }

        folders.removeDuplicates();
    }

    size = folders.size();
    legends.clear();

    QVector<QList<DataReport* >* >* dados = new QVector<QList<DataReport* >* >();

    for( int i = 0; i < size; i++ ) {
        // Building legends
        QString dir = folders.at(i);

        QString legend;
        if( aop->isLatencyDistribution() ) {
            int lio = dir.lastIndexOf("/");
            QString expFolder = dir.left( lio );
            legend = aop->getLegend( expFolder );
        } else {
            legend = aop->getLegend( dir );
        }



        if( legend.isEmpty() ) {
            if(aop->isLatencyDistribution()) {
                legend = QString("Histogram#%1").arg(i+1);
            } else {
                legend = QString("Config#%1").arg(i+1);
            }
        }

        if( legends.contains( legend ) ) {
            legend += QString("#%1").arg(i+1);
        }

        if( aop->isLatencyDistribution() ) {
            int lio = dir.lastIndexOf("/") + 1;
            legend += QString(" @ %1").arg( dir.mid(lio) );
        }

        legends.append(legend);


        // Identifying file to be read
        QString filename = dir+"/Results";
        switch( aop->getFlowOp() ) {
            case AnalysisOptions::AllFlows:
                filename += "/summary";
                break;
            case AnalysisOptions::RT0:
            case AnalysisOptions::RT1:
            case AnalysisOptions::nRT0:
            case AnalysisOptions::nRT1:
                filename += QString("/class_%1").arg(aop->getFlowOp() - 1);
                break;
            case AnalysisOptions::Specified:
                filename += QString("/flow_%1_%2_%3")
                        .arg(aop->getSource())
                        .arg(aop->getDestination())
                        .arg(0); // TODO Obter flowID
                break;

        }
        if(aop->isLatencyDistribution()) {
            filename += "_latency_histogram";
        }

        // Normalizing filename to be read
        QByteArray byteArrayReport = filename.toUtf8();
        const char* strFile = byteArrayReport.constData();

        // Reading report file
        ReportReader* reader = new ReportReader();
        std::list<DataReport* >* dataReport = NULL;
        if( aop->isLatencyDistribution() ) {
            dataReport = reader->readLatencyDistributionReport(strFile);
        } else {
            dataReport = reader->readReport(strFile);
        }

        delete reader;

        QList<DataReport*>* data = NULL;
        if(dataReport != NULL) {
            // Converting C stl standard list to QList
            data = new QList<DataReport *>( QList<DataReport *>::fromStdList(*dataReport) );
        }

        if( data == NULL ) {
            size = dados->size();
            // Deallocating data already read
            size = dados->size();
            for( int x = 0; x < size; x++ ) {
                data = dados->at(x);
                if( data != NULL ) {
                    for( int z = 0; z < data->size(); z++ ) {
                        DataReport* d = data->at(z);
                        delete d;
                    }

                    data->clear();
                    delete data;
                }
            }
            dados->clear();
            delete dados;
            return NULL;
        }

        dados->append(data);
    }
    return dados;

}

void Control::viewReport(AnalysisOptions *aop) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::viewReport" << std::endl;
#endif

    if( !analysisOk ) {
        this->mainWindow->printConsole( trUtf8("<font color=red>Impossible show report because there is not analysis results</font>") );
    } else {
        QVector<QList<DataReport* >* >* data = getReportData(aop);
        if( data == NULL) {
            this->mainWindow->printConsole(trUtf8("<font color=red>Report file unavailable or this flow is null (no packet was transfered)</font>"));
            return;
        } else {
            ReportDialog* repDial = new ReportDialog(legends,data,mainWindow);
            connect(repDial,SIGNAL(rejected()),repDial,SLOT(deleteLater()));
            repDial->setWindowModality(Qt::NonModal);
            repDial->show();
        }
    }
    delete aop;
}

void Control::editOptions() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::editOptions" << std::endl;
#endif

    ConfigDialog* configDial = new ConfigDialog(conf,mainWindow);
    configDial->setLocale( mainWindow->locale() );
    connect(configDial,SIGNAL(applyConfiguration(EnvironmentConfiguration*,QString)),
            this,SLOT(applySettings(EnvironmentConfiguration*,QString)));
    configDial->exec();

    delete configDial;

}

void Control::changeLanguage(QString languageName) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::editOptions" << std::endl;
#endif

    conf->setLanguageSelected(languageName);

}

void Control::applySettings(EnvironmentConfiguration *env,QString languageName) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::applySettings" << std::endl;
#endif

    conf->setLanguageSelected( languageName );
    conf->setPluginsFolder( env->getPluginsFolder() );
    conf->setSimulatorLocation( env->getSimulatorLocation() );
    conf->setThreadNumber( env->getThreadNumber() );
    conf->setWorkFolder( env->getWorkFolder() );
    conf->setWaveformTool(env->getWaveformTool());
    conf->writeSetup();

}

void Control::loadSimulationResults() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::loadSimulationResults" << std::endl;
#endif
    // TODO: Refatorar e criar classe para tratar apenas do arquivamento e carregamento de simulações realizadas

    if( this->mainWindow->isWindowModified() ) {
        switch(this->mainWindow->saveChanges(APPLICATION_NAME,trUtf8("The configuration has been modified.\nDo you want to save your changes?"))) {
            case 0: // Save
                if(!this->saveConfiguration()) {
                    return;
                }
            case 1: // Discard
                break;
            case 2: // Cancel
                return;
        }
    }

    this->mainWindow->setWindowModified(false);

    QString filename = this->mainWindow->dialogLoadFile(trUtf8("Open a simulation file"),
                                                        trUtf8("RedScarf simulation file (*.redsim)"),
                                                        QDir::homePath());
    if(filename.isEmpty()) {
        return;
    }

    // Get date and time
    QDateTime dateTime = QDateTime::currentDateTime();
    // Get a string with date and time in ISO format
    QString dateTimeDir = dateTime.toString(Qt::ISODate);
    dateTimeDir.replace(':','-');
    workDirSimulationLoaded = conf->getWorkFolder() + "/SimulationsLoaded/@" + dateTimeDir + "/";
    FolderCompressor* fc = new FolderCompressor( FolderCompressor::Decompress,
                        filename, workDirSimulationLoaded );
    connect(fc,SIGNAL(completed(bool,int)),this,SLOT(folderCompressorWorkCompleted(bool,int)));

    QApplication::setOverrideCursor(Qt::WaitCursor);
    this->mainWindow->printConsole(trUtf8("<b>Wait . . .</b>"));
    this->mainWindow->setAnalysisOptionsEnabled(false);
    this->mainWindow->setOptionsSimulationEnabled(false);
    QThreadPool::globalInstance()->start(fc);

}

void Control::saveSimulationResults() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::saveSimulationResults" << std::endl;
#endif

    if( this->simulationFolders == NULL ) { // There are no simulation results
        return;
    }

    QString dir0 = this->simulationFolders->at(0);
    // Up 2 level in directory tree
    dir0 = dir0.left( dir0.lastIndexOf("/") );
    dir0 = dir0.left( dir0.lastIndexOf("/") );

    QString workDir = dir0 + "/";

    QString filename = mainWindow->dialogSaveFile(trUtf8("Save a simulation file"),
                                                  trUtf8("RedScarf simulation(*.redsim)"),
                                                  QDir::homePath());
    if(filename.isEmpty()) {
        return;
    }
    if(!filename.endsWith(".redsim")) {
        filename += ".redsim";
    }

    QFile* tmp = configFile;
    configFile = new QFile( workDir + RESULT_SIMLATION_SETUP_FILENAME );

    if( !this->saveConfiguration() ) {
        this->mainWindow->printConsole(trUtf8("<font color=red>It is not possible save results.</font>"));
        configFile = tmp;
        return;
    }
    delete configFile;
    configFile = tmp;

    tmp = new QFile( workDir + RESULT_SIMULATION_DIRS_FILENAME );

    if( !tmp->open(QIODevice::WriteOnly | QIODevice::Text) ) {
        this->mainWindow->printConsole(trUtf8("<font color=red>It is not possible save results.</font>"));
        delete tmp;
        return;
    }

    QTextStream ts(*&tmp);
    int dirsSize = this->simulationFolders->size();
    for( int i = 0; i < dirsSize; i++) {
        QString dir = simulationFolders->at(i);
        dir.remove( workDir );
        ts << dir;
        ts << "\n";
    }
    ts << (analysisOk?"Analyzed":"NotAnalyzed");

    tmp->close();
    delete tmp;

    FolderCompressor* fc = new FolderCompressor( FolderCompressor::Compress, workDir, filename);
    connect(fc,SIGNAL(completed(bool,int)),this,SLOT(folderCompressorWorkCompleted(bool,int)));

    QApplication::setOverrideCursor(Qt::WaitCursor);
    this->mainWindow->printConsole(trUtf8("<b>Saving the experiment results. This can take some minutes. Do not close RedScarf!</b>"));
    this->mainWindow->setAnalysisOptionsEnabled(false);
    this->mainWindow->setOptionsSimulationEnabled(false);
    QThreadPool::globalInstance()->start(fc);

}

void Control::folderCompressorWorkCompleted(bool success,int opType) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::folderCompressorWorkCompleted" << std::endl;
#endif

    QString successMsg;
    QString failureMsg;
    switch (opType) {
        case 1:{ // Compress operation
            successMsg = trUtf8("<font color=blue>Simulation results file successfully generated</font>");
            failureMsg = trUtf8("<font color=red>Problem ocurred in generate simulation results file."
                                "<br />Please verify the work folder permissions and try again.</font>");
            break;
        }
        case 2:{ // Decompress operation
            successMsg = trUtf8("<font color=blue>Simulation results successfully loaded to work folder</font>");
            failureMsg = trUtf8("<font color=red>Problem occurred in load simulation results."
                                "<br />Please verify the work folder permissions and try again.</font>");
            break;
        }
    }

    this->mainWindow->printConsole( (success ? successMsg : failureMsg) );

    if( opType == 2 ) { // Decompress operation

        if( configFile ) {
            delete configFile;
        }
        configFile = new QFile( workDirSimulationLoaded + RESULT_SIMLATION_SETUP_FILENAME );

        bool recoverFolders = true;

        if( this->loadConfiguration() ) {
            this->mainWindow->setWindowModified(false);
        } else {
            this->mainWindow->printConsole(trUtf8("<font color=red>The parameters load failed.</font>"));
            recoverFolders = false;
        }
        delete configFile;
        configFile = NULL;

        QFile* tmp = new QFile( workDirSimulationLoaded + RESULT_SIMULATION_DIRS_FILENAME );

        if( !tmp->open(QIODevice::ReadOnly | QIODevice::Text) ) {
            this->mainWindow->printConsole(trUtf8("<font color=red>It is not possible load results.</font>"));
            recoverFolders = false;
        }

        if(recoverFolders)  {
            QTextStream ts(*&tmp);
            QString line;
            QStringList dirs;
            do {
                line = ts.readLine();
                dirs.append(line);
            } while(!line.isNull());
            dirs.removeLast();
            if( dirs.last() == "NotAnalyzed" ) {
                this->analysisOk = false;
            } else {
                this->analysisOk = true;
            }
            dirs.removeLast();

            if(simulationFolders != NULL) {
                simulationFolders->clear();
                delete simulationFolders;
                simulationFolders = NULL;
            }
            simulationFolders = new QList<QString>();
            for( int i = 0; i < dirs.size(); i++ ) {
                QString dir = QString( workDirSimulationLoaded + dirs.at(i) );
                simulationFolders->append( dir );
            }
        }
        tmp->close();
// TODO verificar
//        this->copySystemParameters();
        delete tmp;
    }

    this->mainWindow->setAnalysisOptionsEnabled(true);
    this->mainWindow->setOptionsSimulationEnabled(true);
    this->mainWindow->setActionGenerateCSVEnabled(analysisOk);
    QApplication::restoreOverrideCursor();
}

void Control::generateCSVSimulationReport(AnalysisOptions *aop) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::generateCSVSimulationReport" << std::endl;
#endif

    QVector<QList<DataReport* >* >* data = getReportData(aop);

    if(data == NULL) {
        this->mainWindow->printConsole(trUtf8("<font color=red>Report file unavailable or this flow is null (no packet was transfered)."
                                                              "</br >Therefore can not generate the CSV report.</font>"));
        delete aop;
        return;
    }

    QString filename = mainWindow->dialogSaveFile(trUtf8("Save a CSV report file"),
                                                  trUtf8("RedScarf CSV Report(*.csv)"),
                                                  QDir::homePath());
    if(!filename.isEmpty())  {
        if(!filename.endsWith(".csv")) {
            filename += ".csv";
        }

        QFile file(filename);

        if( !file.open(QIODevice::WriteOnly | QIODevice::Text) ) {
            this->mainWindow->printConsole(trUtf8("<font color=red>It is not possible generate CSV."
                                                     "<br />Please verify write permissions in selected folder.</font>"));
        } else {
            QString csvText;
            QTextStream ts(&csvText);

            QLocale local = QLocale::system();
            QChar decPoint = local.decimalPoint();
            char separator = ',';
            if( decPoint == ',' ) {
                separator = ';';
            }
            ts << trUtf8("RedScarf CSV Report") << separator
               << separator << separator << separator << separator << separator << separator
               << separator << separator << separator << separator << separator << separator
               << separator << separator << separator << separator << separator << separator
               << separator << separator << "\n";
            // Varrer e escrever CSV
            for( int x = 0; x < data->size(); x++ ) {
                QList<DataReport* >* listData = data->at(x);
                if( listData != NULL ) {
                    // Legenda
                    ts << separator << separator << separator << separator << separator << separator
                       << separator << separator << separator << separator << separator << separator
                       << separator << separator << separator << separator << separator << separator
                       << separator << separator << separator << "\n";
                    ts << separator << trUtf8("Experiment: ") << separator << legends.at(x)
                       << separator << separator << separator << separator << separator << separator
                       << separator << separator << separator << separator << separator << separator
                       << separator << separator << separator << separator << separator << separator
                       << separator << "\n";
                    ts << separator << separator <<trUtf8("Clock Frequency(MHz)")
                       << separator << trUtf8("Analyzed Packets")
                       << separator << trUtf8("Offered Traffic(norm)") << separator
                       << trUtf8("Accepted Traffic(norm)") << separator
                       << trUtf8("Ideal Average Latency(cycles)") << separator
                       << trUtf8("Average Latency(cycles)") << separator
                       << trUtf8("Standard Deviation Latency") << separator
                       << trUtf8("Minimum Latency(cycles)") << separator
                       << trUtf8("Maximum Latency(cycles)") << separator
                       << trUtf8("Offered Traffic(Mbps/n)") << separator
                       << trUtf8("Accepted Traffic(Mbps/n)") << separator
                       << trUtf8("Ideal Average Latency(ns)") << separator
                       << trUtf8("Average Latency(ns)") << separator
                       << trUtf8("Minimun Latency(ns)") << separator
                       << trUtf8("Maximum Latency(ns)") << separator
                       << trUtf8("RT0 Met Deadlines(%)") << separator
                       << trUtf8("RT1 Met Deadlines(%)") << separator
                       << trUtf8("nRT0 Met Deadlines(%)") << separator
                       << trUtf8("nRT1 Met Deadlines(%)") << "\n";
                    for( int z = 0; z < listData->size(); z++ ) {
                        // Linha de dado
                        DataReport* d = listData->at(z);
                        ts << separator << separator
                           << QString::number(d->fClk,'f',0) << separator
                           << QString::number(d->accNbOfPck,'f',0) << separator
                           << QString::number(d->avgRequiredBwNorm,'f',4) << separator
                           << QString::number(d->acceptedTrafficFlits,'f',4) << separator
                           << QString::number(d->idealAvgLatency,'f',0) << separator
                           << QString::number(d->avgLatencyCycles,'f',1) << separator
                           << QString::number(d->stdevLatency,'f',1) << separator
                           << QString::number(d->minLatency,'f',0) << separator
                           << QString::number(d->maxLatency,'f',0) << separator
                           << QString::number(d->avgRequiredBwBps,'f',0) << separator
                           << QString::number(d->acceptedTrafficBps,'f',0) << separator
                           << QString::number(d->idealAvgLatencyNs,'f',1) << separator
                           << QString::number(d->avgLatencyNs,'f',1) << separator
                           << QString::number(d->minLatencyNs,'f',1) << separator
                           << QString::number(d->maxLatencyNs,'f',1) << separator
                           << (d->metDeadlinesPer0 == -1 ? QString():QString::number(d->metDeadlinesPer0)) << separator
                           << (d->metDeadlinesPer1 == -1 ? QString():QString::number(d->metDeadlinesPer1)) << separator
                           << (d->metDeadlinesPer2 == -1 ? QString():QString::number(d->metDeadlinesPer2)) << separator
                           << (d->metDeadlinesPer3 == -1 ? QString():QString::number(d->metDeadlinesPer3)) << "\n";
                    }
                }
            }
            if( decPoint == ',' ) {
                csvText.replace(".",","); // Change decimal separator to ','
            }

            QTextStream csvTs(&file);
            csvTs << csvText;
            file.close();
            this->mainWindow->printConsole(trUtf8("<font color=blue>RedScarf CSV Report successfully generated!</font>"));
        }
    }

    // Desalocando memória
    for( int x = 0; x < data->size(); x++ ) {
        QList<DataReport* >* listData = data->at(x);
        if( listData != NULL ) {
            for( int z = 0; z < listData->size(); z++ ) {
                DataReport* d = listData->at(z);
                delete d;
            }
            listData->clear();
            delete listData;
        }
    }
    data->clear();
    delete data;

    delete aop;
}

void Control::generateTrafficConfigurationFile() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::generateCSVSimulationReport" << std::endl;
#endif

    QString outDir = this->mainWindow->selectSystemFolder(trUtf8("Select the output dir to traffic conf"));

    if(outDir.isEmpty()) {
        return;
    }

// TODO Verificar
    Experiment* e = 0; // experimentManager->getExperiment(1);
    TrafficModelGenerator* tmg = new TrafficModelGenerator(systemParameters,e);

    tmg->generateTraffic( outDir.toStdString().c_str() );

    this->mainWindow->printConsole("Traffic File Generated",Qt::blue);

    delete tmg;
}

void Control::runSimulations() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::runSimulations" << std::endl;
#endif

    if( !inputsOk() ) {
        finishSimulation( Control::InputsError );
        return;
    }

// TODO verificar
//    this->copySystemParameters();

    if( simulationFolders != NULL ) {
        simulationFolders->clear();
        delete simulationFolders;
        simulationFolders = NULL;
    }

    this->simulationFolders = new QList<QString>();

    // Get date and time
    QDateTime dateTime = QDateTime::currentDateTime();
    // Get a string with date and time in ISO format
    QString dateTimeDir = dateTime.toString(Qt::ISODate);
    dateTimeDir.replace(':','-');
    // To manipulate the folders that will be used
    QDir dirWork = QDir(conf->getWorkFolder() + "/" + dateTimeDir);
    // Criar diretório de trabalho
    if(!dirWork.exists()) {
        dirWork.mkpath(".");
        this->mainWindow->printConsole(trUtf8("- Work directory created"));
    }

    threadManager = new ThreadManager( (int) conf->getThreadNumber() , this );

    connect(threadManager,SIGNAL(allFinished()),this,SLOT(finishExecution()));
    connect(threadManager,SIGNAL(threadFinished(int)),this,SLOT(updateStatusExecution(int)));

    int numberOfExperiments = 0;
    unsigned int runCountPerExperiment = this->calcAmountExperimentsExecutions();

    QSettings settings(qApp->applicationDirPath()+"/etc/system.ini",QSettings::IniFormat);
    settings.beginGroup("Traffic_Parameters");
    bool useDefault = settings.value("useDefault",true).toBool();
    QString alternativeFile = settings.value("alternative",QString()).toString();
    settings.endGroup();

    settings.beginGroup("Input_Buffers");
    QString memPlugin = settings.value("plugin").toString();
    settings.endGroup();


    SystemDefines* def = SystemDefines::getInstance();

    // For each experiment - create a folder
    for( unsigned int i = 1; i <= 5; i++ ) {
// TODO Verificar
//        float fClk = this->systemParameters->getfClkFirst();
        float fClk = this->systemOperation->fClkFirst;
        float fClkPrevious = fClk;
// TODO
        Experiment* experiment = 0; //this->experimentManager->getExperiment(i);
        if(experiment != NULL) {
            if(experiment->isActive()) {
                numberOfExperiments++;

                SystemDefines::Topology topology = def->findTopology(experiment->getTopology());
                SystemDefines::Routing routing = def->findRoutingAlgorithm(topology.getTopology());
                SystemDefines::FlowControl flowControl = def->findFlowControl(experiment->getFlowControl());
                SystemDefines::PriorityGenerator pg = def->findArbiterPG(experiment->getArbiterType());
                QString vcOption = def->findVcOption(experiment->getVCOption());
                QString strRouting = routing.getRoutingAlgorithm( experiment->getRoutingAlgorithm() );

                QString dirGenerated = QString("dir_%1_%2_%3_%4_VC%5_IN%6_OUT%7")
                        .arg( topology.getTopology() )
                        .arg( strRouting )
                        .arg( flowControl.getFlowControl() )
                        .arg( pg.getPG() )
                        .arg( vcOption )
                        .arg(experiment->getInputBufferSize())
                        .arg(experiment->getOutputBufferSize());
                QString dirExperiment = dirWork.absolutePath() + "/" + dirGenerated;
                dirWork.mkdir( dirExperiment );

                /*
                 * Generate stopsim.par on work dir (dir to set of experiments - root experiments dir)
                 */
                QFile* stopSimPar = new QFile( dirWork.absolutePath()+"/stopsim.par");
                if(!stopSimPar->open(QIODevice::WriteOnly)) {
                    this->mainWindow->printConsole(trUtf8("Impossible to open file  %1. Aborted. ")
                                                        .arg(stopSimPar->fileName()),Qt::red);
                    return;
                }
// TODO verificar
//                switch(systemParameters->getStopOption()) {
//                    case 0:
//                        systemParameters->setStopTime_ns(0);
//                        systemParameters->setStopTime_cycles(0);
//                        break;
//                    case 1:
//                        systemParameters->setStopTime_cycles( (unsigned long int) systemParameters->getStopTime_ns()/systemParameters->getTClk() );
//                        break;
//                    case 2:
//                        systemParameters->setStopTime_ns( (unsigned long int) systemParameters->getStopTime_cycles()*systemParameters->getTClk());
//                        break;
//                }
                switch(systemOperation->stopOption) {
                case 0:
                    systemOperation->stopTime_ns = 0;
                    systemOperation->stopTime_cycles = 0;
                    break;
                case 1:
                    systemOperation->stopTime_cycles = (unsigned long int) systemOperation->stopTime_ns/systemOperation->tClk;
                    break;
                case 2:
                    systemOperation->stopTime_ns = (unsigned long int) systemOperation->stopTime_cycles*systemOperation->tClk;
                    break;
                }
                QString stopSimParContent = QString("%1\t%2")
                        .arg(systemOperation->stopTime_cycles)
                        .arg(systemOperation->stopTime_ns);
                stopSimPar->write(stopSimParContent.toUtf8());
                stopSimPar->close();
                delete stopSimPar;
                /*
                 * End stopsim.par
                 */

                int aux = 0;
                SystemOperation* sp = new SystemOperation(*systemOperation);
                for(unsigned int x = 0; x < runCountPerExperiment; x++) {
                    /*
                     * Generate Traffic conf file on simulation dir (root_experiment_dir/experiment/fClk/)
                     */
                    QDir dirSimulation = QDir( dirExperiment );
                    QString dirFreqOp = QString("%1MHz").arg(fClk);
                    dirSimulation.mkdir(dirFreqOp);
                    dirSimulation.cd(dirFreqOp);
                    QString strDirSimul = dirSimulation.absolutePath();
                    this->simulationFolders->append(strDirSimul);
                    // TODO verificar
                    TrafficModelGenerator* trafficGen =
                            new TrafficModelGenerator(systemParameters,experiment);
                    try {
                        if( useDefault ) {
                            trafficGen->generateTraffic( strDirSimul.toStdString().c_str() );
                        } else {
                            QFile::copy(alternativeFile,strDirSimul+"/traffic.tcf");
                        }
                        delete trafficGen;
                    } catch (const char* exception) {
                        this->mainWindow->printConsole(trUtf8("Error in generate traffic model: %1")
                                                       .arg(QString::fromStdString(exception)),Qt::red);
                        delete trafficGen;
                        delete sp;
                        return;
                    }
                    /*
                     * End generate traffic.tcf
                     */

                    /*
                     * Generate simconf.conf - file with specified plugins
                     */
                    QFile* simConf = new QFile( strDirSimul+"/"+SIMULATOR_CONF_FILE);
                    if(!simConf->open(QIODevice::WriteOnly)) {
                        this->mainWindow->printConsole(trUtf8("Impossible to open file  %1. Aborted. ")
                                                            .arg(simConf->fileName()),Qt::red);
                        return;
                    }


                    QString simConfContent =
                            QString("noc = %1\n"
                                    "router = %2\n"
                                    "routing = %3\n"
                                    "flowcontrol = %4\n"
                                    "memory = %5\n"
                                    "prioritygenerator = %6")
                            .arg(topology.getNoCPlugin())
                            .arg(topology.getRouterPlugin())
                            .arg(routing.getRoutingPlugin(experiment->getRoutingAlgorithm()))
                            .arg(flowControl.getPlugin())
                            .arg(memPlugin)
                            .arg(pg.getPlugin());

                    simConf->write(simConfContent.toUtf8());
                    simConf->close();
                    delete simConf;
                    /*
                     * End simconf.conf
                     */

                    /*
                     * Setup arguments to the simulator executable (passing through command-line arguments)
                     */
                    float TClk = (1.0/ (float) fClk) * 1000.0;
                    QStringList args;
                    args.append(QString("%1").arg(TClk));
                    args.append(QString("%1").arg(strDirSimul));
                    args.append( conf->getPluginsFolder() );
                    args.append("-xsize");
                    args.append(QString::number(systemParameters->getXSize()));
                    args.append("-ysize");
                    args.append(QString::number(systemParameters->getYSize()));
                    if(systemParameters->getZSize()>1) {
                        if(topology.getTopology().contains("3D")) { // WARNING - verify a safe alternative
                            args.append("-zsize");
                            args.append(QString::number(systemParameters->getZSize()));
                        }
                    }
                    args.append("-datawidth");
                    args.append(QString::number(systemParameters->getDataWidth()));
                    args.append("-fifoin");
                    args.append(QString::number(experiment->getInputBufferSize()));
                    args.append("-fifoout");
                    args.append(QString::number(experiment->getOutputBufferSize()));
                    if( experiment->getVCOption() > 0 ) {
                        args.append( "-vc" );
                        args.append( QString::number( pow(2,experiment->getVCOption()),'f',0 ) );
                    }
// TODO verificar
//                    if( systemParameters->getVcdOption() ) {
//                        args.append("-trace");
//                    }
                    if( systemOperation->vcdOption ) {
                        args.append("-trace");
                    }
                    /*
                     * End setup command-line arguments
                     */


                    /*
                     * Generate a simulation performer (that run the simulator) and its separated thread
                     */
                    SimulationPerformer* exeSystem = new SimulationPerformer(TClk,strDirSimul,
                                                                             conf->getSimulatorLocation(),
                                                                             args);

                    exeSystem->setObjectName(QString("Execution%1").arg(x));
                    QThread* threadExecucao = new QThread(this);
                    threadExecucao->setObjectName(QString("Execution%1").arg(strDirSimul));
                    connect(threadExecucao,SIGNAL(started()),exeSystem,SLOT(execute()));
                    connect(threadExecucao,SIGNAL(finished()),exeSystem,SLOT(deleteLater()));
                    connect(exeSystem,SIGNAL(destroyed(QObject*)),this,SLOT(removeExe(QObject*)));
                    connect(exeSystem,SIGNAL(finished()),threadExecucao,SLOT(quit()));
                    connect(exeSystem,SIGNAL(finished()),exeSystem,SLOT(deleteLater()));
        /// Dividir simuladores
                    connect(exeSystem,SIGNAL(sendMessage(QString)),mainWindow,SLOT(printConsole(QString)));
        /// Feedback individual para o usuário
                    connect(exeSystem,SIGNAL(unsuccessfullyExecution()),this,SLOT(executeUnsuccessful()));
                    exeSystem->moveToThread(threadExecucao);

                    threadManager->addPool( threadExecucao );
                    this->exes->append( exeSystem );
                    /*
                     * End generate simulation performer
                     */


                    /*
                     * Calculating
                     */
                    if( sp->fClkStepType == 0 ) {
                        fClk += sp->fClkStep;
                        if( (sp->fClkFirst > sp->fClkLast )
                                && (fClk < sp->fClkLast) ) {
                            fClk = sp->fClkLast;
                        }
                        if( (sp->fClkFirst < sp->fClkLast)
                                && (fClk > sp->fClkLast) ) {
                            fClk = sp->fClkLast;
                        }
                    } else {
                        do {
                            fClkPrevious = fClk;
                            int tmp = (unsigned int) (sp->fClkFirst
                                                      / ( exp(aux*sp->fClkStep) )*10 );
                            fClk = ((float) tmp) / 10.0;
                            aux++;
                        } while (fClk == fClkPrevious);
                    } // if(getfClkStepType == 0)
                    sp->tClk = (1.0f/fClk)*1000.0f;
                    sp->channelBandwidth = fClk * systemParameters->getDataWidth();

                } // for(unsigned int x = 0; x < runCountPerExperiment; x++)
                delete sp;
            } // if(experiment->isActive())
        } // if(experiment != NULL)
    } // for( unsigned int i = 1; i <= 5; i++ )

    unsigned int nExp = runCountPerExperiment*numberOfExperiments;
    this->mainWindow->setLimitsProgressBar( 0, nExp);

    this->mainWindow->printConsole( trUtf8("- Number of simulations to be run: %1").arg(nExp) );

    timer->start();
    threadManager->runThreads();

}
