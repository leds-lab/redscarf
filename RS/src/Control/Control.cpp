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
*
*/

#include "include/Main.h"

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
#include "include/Control/Control.h"
#include "include/Control/XmlConfigParser.h"
#include "include/Control/Builder.h"
#include "include/Control/SimulationPerformer.h"
#include "include/Control/ThreadManager.h"
#include "include/Control/WaveformViewer.h"
#include "include/Control/ExternalWaveformViewer.h"
#include "include/Control/Plotter.h"
#include "include/Control/GnuPlotPlotter.h"
#include "include/Control/Analyzer.h"
#include "include/Control/EnvironmentConfiguration.h"
#include "include/Control/FolderCompressor.h"

// View
#include "include/View/MainWindow.h"
#include "include/View/TrafficConfigurationDialog.h"
#include "include/View/DefaultPreviewTrafficConfiguration.h"
#include "include/View/XMLPreviewTrafficConfiguration.h"
#include "include/View/AnalysisOptions.h"
#include "include/View/ReportDialog.h"
#include "include/View/GetSelectedItemsDialog.h"
#include "include/View/ConfigDialog.h"
#include "include/View/QwtPlotter.h"
#include <QInputDialog>

// Model
#include "include/Model/System/SystemParameters.h"
#include "include/Model/System/Experiment.h"
#include "include/Model/System/ExperimentManager.h"
#include "include/Model/System/SystemDefines.h"
#include "include/Model/Traffic/TrafficPatternManager.h"
#include "include/Model/Traffic/TrafficPatternDefines.h"
#include "include/Model/Util.h"
#include "include/Model/Analysis/DataReport.h"
#include "include/Model/Analysis/ReportReader.h"

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
    this->trafficPatternManager = new TrafficPatternManager();
    this->experimentManager     = new ExperimentManager();
    experimentManager->insertExperiment(1, new Experiment());

    this->exes = new QList<SimulationPerformer *>();
    this->timer = new QElapsedTimer();

    this->configFile = NULL;
    this->conf = new EnvironmentConfiguration(this,true);

    this->builder = NULL;
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
        }
    }
    exes->clear();

    delete this->systemParameters;
    delete this->trafficPatternManager;
    delete this->experimentManager;
    delete this->exes;
    delete this->timer;
    conf->writeSetup();
    delete conf;
    if( this->threadManager != NULL ) {
        threadManager->stopThreads();
        delete threadManager;
    }
    if(simulationFolders != NULL) {
        for(int i = 0; i < simulationFolders->size(); i++) {
            QString* dir = simulationFolders->at(i);
            delete dir;
        }
        simulationFolders->clear();
        delete simulationFolders;
    }

    SystemDefines* defSis = SystemDefines::getInstance();
    delete defSis;
    TrafficPatternDefines* defPTr = TrafficPatternDefines::getInstance();
    delete defPTr;
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
    connect(this->mainWindow,SIGNAL(clearSystem()),this,SLOT(clearSystem()));
    connect(this->mainWindow,SIGNAL(exitApplication(QCloseEvent*)),this,SLOT(exitApplication(QCloseEvent*)));
    connect(this->mainWindow,SIGNAL(editOptions()),this,SLOT(editOptions()));
    connect(this->mainWindow,SIGNAL(changeLanguage(QString)),this,SLOT(changeLanguage(QString)));
    connect(this->conf,SIGNAL(languageChanged(QStringList)),mainWindow,SLOT(translate(QStringList)) );
    connect(this->mainWindow,SIGNAL(loadSimulationResults()),this,SLOT(loadSimulationResults()));
    connect(this->mainWindow,SIGNAL(saveSimulationResults()),this,SLOT(saveSimulationResults()));
    connect(this->mainWindow,SIGNAL(generateCSVSimulationReport(AnalysisOptions*)),this,SLOT(generateCSVSimulationReport(AnalysisOptions*)));

    connect(this->mainWindow,SIGNAL(channelWidthUpdated(unsigned int)),this,SLOT(updateChannelWidth(unsigned int)));
    connect(this->mainWindow,SIGNAL(sizeUpdated(uint,uint)),this,SLOT(updateSizeSystem(uint,uint)));
    connect(this->mainWindow,SIGNAL(nodeSelected(uint,uint)),this,SLOT(nodeSelected(uint,uint)));
    connect(this->mainWindow,SIGNAL(trafficPatternUpdate(uint,uint,uint,bool)),this,SLOT(trafficPatternStatusChanged(uint,uint,uint,bool)));
    connect(this->mainWindow,SIGNAL(buttonEditClicked(uint,uint,uint)),this,SLOT(editTrafficPattern(uint,uint,uint)));
    connect(this->mainWindow,SIGNAL(previewTrafficConfiguration(int)),this,SLOT(previewTrafficConfiguration(int)));

    connect(this->mainWindow,SIGNAL(experimentStateChanged(int,bool)),this,SLOT(experimentStateChanged(int,bool)));
    connect(this->mainWindow,SIGNAL(configurationExperimentChanged(int,int,int)),this,SLOT(configurationExperimentChanged(int,int,int)));

    connect(this->mainWindow,SIGNAL(stopOptionChanged(int)),this,SLOT(stopOptionUpdated(int)));
    connect(this->mainWindow,SIGNAL(stopTimeNsChanged(int)),this,SLOT(stopTimeNsUpdated(int)));
    connect(this->mainWindow,SIGNAL(stopTimeCyclesChanged(int)),this,SLOT(stopTimeCyclesUpdated(int)));
    connect(this->mainWindow,SIGNAL(vcdOptionChanged(int)),this,SLOT(vcdOptionUpdated(int)));
    connect(this->mainWindow,SIGNAL(fClkFirstChanged(double)),this,SLOT(fClkFirstUpdated(double)));
    connect(this->mainWindow,SIGNAL(fClkLastChanged(double)),this,SLOT(fClkLastUpdated(double)));
    connect(this->mainWindow,SIGNAL(fClkStepChanged(double)),this,SLOT(fClkStepUpdated(double)));
    connect(this->mainWindow,SIGNAL(fClkStepTypeChanged(int)),this,SLOT(fClkStepTypeUpdated(int)));

    connect(this->mainWindow,SIGNAL(runSimulation()),this,SLOT(runBuilder()));
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

    if( conf->getSystemCFolder().isEmpty() ) {
        conf->setSystemCFolder( this->dirSetup(
                trUtf8("Select SystemC library folder"),
                trUtf8("SystemC folder not configured\nThe simulator is not run!")) );

    }
#ifndef Q_OS_UNIX
    if( conf->getMinGWFolder().isEmpty() ) {
        conf->setMinGWFolder( this->dirSetup(
            trUtf8("Select MinGW compiler folder"),
            trUtf8("MinGW folder not configured\nThe simulator is not run!")) );
    }
#endif

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

    /// Carregar
    XmlConfigParser* parser = new XmlConfigParser(systemParameters,trafficPatternManager,experimentManager);
    parser->loadXML(configFile);

    this->mainWindow->updateView( this->systemParameters->getXSize(),
            this->systemParameters->getYSize(),
            this->systemParameters->getDataWidth(),
            this->experimentManager,parser->getStopOption(),
            parser->getStopTime_ns(),parser->getStopTime_cycles(),parser->getVcdOption(),
            parser->getFClk1(),parser->getFClk2(),parser->getFClkStepType(), parser->getFClkStep() );
    delete parser;

    this->mainWindow->printConsole(trUtf8("Configuration file successfuly loaded"));

    configFile->close();

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
    /// Salvar
    XmlConfigParser* parser = new XmlConfigParser(systemParameters,trafficPatternManager,experimentManager);
    parser->saveXML(configFile);
    delete parser;

    this->mainWindow->printConsole(trUtf8("Configuration file successfully saved"));
    this->configFile->close();
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

bool Control::clearTrafficPatterns() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::clearTrafficPatterns" << std::endl;
#endif

    /// Limpar padroes de trafico
    this->trafficPatternManager->clear();
    for(unsigned int i = 0; i < MAX_PATTERNS; i++) {
        this->mainWindow->setPatternState(i,false);
    }
    this->mainWindow->printConsole(trUtf8("Traffic patterns clean"));
    return true;
}

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
        for(unsigned int i = 0; i < this->systemParameters->getXSize(); i++) {
            for(unsigned int x = 0; x < this->systemParameters->getYSize(); x++) {
                Node* no = this->trafficPatternManager->getNode(i,x);
                if( no != NULL ) {
                    for(unsigned int z = 0; z < MAX_PATTERNS; z++) {
                        if( no->isPatternActive(z) ) {
                            padroesAtivos = true;
                            break;
                        }
                    }
                }
                if(padroesAtivos){
                   break;
                }
            }
            if(padroesAtivos){
               break;
            }
        }

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
    for(unsigned int i = 1; i < 5; i++) {
        Experiment* exp1 = this->experimentManager->getExperiment(i);
        if(exp1 != NULL) {
            if(exp1->isActive()) {
                for(unsigned int x = i+1; x < 6; x++) {
                    Experiment* exp2 = this->experimentManager->getExperiment(x);
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
    unsigned int fClkStepType = this->systemParameters->getfClkStepType();
    float fClkFirst = this->systemParameters->getfClkFirst();
    float fClkLast = this->systemParameters->getfClkLast();
    float fClkStep = this->systemParameters->getfClkStep();

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

    unsigned int fClkStepType = this->systemParameters->getfClkStepType();
    float fClkFirst = this->systemParameters->getfClkFirst();
    float fClkLast = this->systemParameters->getfClkLast();
    float fClkStep = this->systemParameters->getfClkStep();

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
        clearTrafficPatterns();
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

void Control::clearSystem() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::clearSystem" << std::endl;
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

    this->clearTrafficPatterns();
    this->mainWindow->setWindowModified(true);
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

void Control::updateChannelWidth(unsigned int width) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::updateChannelWidth" << std::endl;
#endif

    this->systemParameters->setChannelBandwidth( (float) systemParameters->getfClkFirst() * width );
    this->systemParameters->setDataWidth(width);
    this->mainWindow->setWindowModified(true);
}

void Control::updateSizeSystem(unsigned int xSize, unsigned int ySize) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::updateSizeSystem" << std::endl;
#endif

    this->systemParameters->setXSize(xSize);
    this->systemParameters->setYSize(ySize);
    this->mainWindow->setWindowModified(true);

}

void Control::nodeSelected(unsigned int posX, unsigned int posY) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::nodeSelected" << std::endl;
#endif

    Node* no = this->trafficPatternManager->getNode(posX,posY);

    if(no != NULL) {
        for(unsigned int i = 0; i < MAX_PATTERNS; i++) {
            this->mainWindow->setPatternState(i,no->isPatternActive(i));
        }
    } else {
        for(unsigned int i = 0; i < MAX_PATTERNS; i++) {
            this->mainWindow->setPatternState(i,false);
        }
    }

}

void Control::trafficPatternStatusChanged(unsigned int posX, unsigned int posY, unsigned int trafficNum, bool state) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::trafficPatternStatusChanged" << std::endl;
#endif

    Node* no = this->trafficPatternManager->getNode(posX,posY);

    if(no) {
        TrafficParameters* tp = no->getTrafficPattern(trafficNum);
        if(tp) {
            no->setPatternActive(state,trafficNum);
        }
    }

    this->mainWindow->setWindowModified(true);
}

void Control::editTrafficPattern(unsigned int xPos, unsigned int yPos, unsigned int trafficNum) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::editTrafficPattern" << std::endl;
#endif

    /// Caso já exista um padrao de tráfico para esta posicao e trafficNum = carrega a tela com os valores!
    Node* no = this->trafficPatternManager->getNode(xPos,yPos);

    TrafficParameters* tp = NULL;
    // Verifica se o "no" é não nulo
    if( no ) {
        tp = no->getTrafficPattern(trafficNum);
    }

    TrafficConfigurationDialog* tf = new TrafficConfigurationDialog(this->mainWindow,xPos,yPos,trafficNum,
                                                                this->systemParameters->getXSize(),
                                                                systemParameters->getYSize(),
                                                                this->systemParameters->getDataWidth());
    tf->setLocale( mainWindow->locale() );

    if( tp ) {
        tf->setConfiguration( tp );
    }

    connect(tf,SIGNAL(apply(TrafficParameters*,uint)),this,SLOT(applyTrafficConfiguration(TrafficParameters*,uint)));
    connect(tf,SIGNAL(applyAndReplicate(TrafficParameters*,uint)),this,SLOT(applyAndReplicateTrafficConfiguration(TrafficParameters*,uint)));
    tf->exec();

    delete tf;

}

void Control::applyTrafficConfiguration(TrafficParameters *configuration, unsigned int trafficNum) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::applyTrafficConfiguration" << std::endl;
#endif

    // Aplicar ao nodo atual
    unsigned int xPos = configuration->getSourceNodeX();
    unsigned int yPos = configuration->getSourceNodeY();

    Node* no = this->trafficPatternManager->getNode(xPos,yPos);

    if(!no) {
        no = new Node(xPos,yPos);
        this->trafficPatternManager->insertNode(no);
    }
    no->setTrafficPattern(configuration,trafficNum);
    this->mainWindow->setWindowModified(true);
    this->mainWindow->printConsole(trUtf8("Traffic configuration applied"));
}

void Control::applyAndReplicateTrafficConfiguration(TrafficParameters *configuration, unsigned int trafficNum) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::applyAndReplicateTrafficConfiguration" << std::endl;
#endif

    unsigned int xSize = systemParameters->getXSize();
    unsigned int ySize = systemParameters->getYSize();

    for(unsigned int x = 0; x < xSize; x++) {
        for(unsigned int y = 0; y < ySize; y++) {
            Node* no = this->trafficPatternManager->getNode(x,y);
            if(!no) {
                no = new Node(x,y);
                this->trafficPatternManager->insertNode(no);
            }
            TrafficParameters* clone = new TrafficParameters(*configuration);
            clone->setSourceNodeX(x);
            clone->setSourceNodeY(y);
            no->setTrafficPattern( clone , trafficNum ); // first argument is clone of object
        }
    }

    delete configuration;
    this->mainWindow->setWindowModified(true);
    this->mainWindow->printConsole(trUtf8("Traffic configuration applied and replicated"));
}

void Control::previewTrafficConfiguration(int typePreview) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::previewTrafficConfiguration" << std::endl;
#endif

    PreviewDialog* preview = NULL;

    switch( typePreview ) {
        case 1:
            preview = new DefaultPreviewTrafficConfiguration(this->mainWindow);
            break;
        case 2:
            preview = new XMLPreviewTrafficConfiguration(this->mainWindow);
            break;
    }
    preview->setLocale( mainWindow->locale() );
    unsigned int xSize = this->systemParameters->getXSize();
    unsigned int ySize = this->systemParameters->getYSize();

    for( unsigned int i = 0; i < xSize; i++ ) {
        for( unsigned int x = 0; x < ySize; x++ ) {
            Node* nodo = this->trafficPatternManager->getNode(i,x);
            if( nodo != NULL ) {
                preview->addNode(nodo);
            }
        }
    }

    preview->exec();

    delete preview;

}

void Control::experimentStateChanged(int numExperiment, bool state) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::experimentStateChanged" << std::endl;
#endif
    Experiment* exp = experimentManager->getExperiment( quint32(numExperiment));;
   if( exp == NULL ) {
       exp = new Experiment();
       experimentManager->insertExperiment(quint32(numExperiment),exp);
   }
   exp->setActive(state);
   this->mainWindow->setWindowModified(true);

}

void Control::configurationExperimentChanged(int opCode, int numExperiment, int newValue) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::configurationExperimentChanged" << std::endl;
#endif
    Experiment* exp = experimentManager->getExperiment(quint32(numExperiment));
    if(!exp) {
        return;
    }
    switch(opCode) {
        case 1: // Router architecture changed
            exp->setRouterArchitecture( quint32(newValue) );
            break;
        case 2: // Routing algorithm changed
            exp->setRoutingAlgorithm( quint32(newValue) );
            break;
        case 3: // Flow Control (switching) changed
            exp->setFlowControl(quint32(newValue));
            break;
        case 4: // Arbiter type changed
            exp->setArbiterType(quint32(newValue));
            break;
        case 5: // Input Buffers changed
            exp->setInputBufferSize( newValue );
            break;
        case 6: // Output Buffers changed
            exp->setOutputBufferSize(newValue);
            break;
    }

    this->mainWindow->setWindowModified(true);

}

void Control::stopOptionUpdated(int indice) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::stopOptionUpdated" << std::endl;
#endif
    this->systemParameters->setStopOption( quint32(indice) );
    this->mainWindow->setWindowModified(true);
}

void Control::stopTimeNsUpdated(int newTime) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::stopTimeNsUpdated" << std::endl;
#endif

    this->systemParameters->setStopTime_ns( quint32(newTime) );
    this->mainWindow->setWindowModified(true);

}

void Control::stopTimeCyclesUpdated(int newTime) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::stopTimeCyclesUpdated" << std::endl;
#endif

    this->systemParameters->setStopTime_cycles( quint32(newTime) );
    this->mainWindow->setWindowModified(true);

}

void Control::vcdOptionUpdated(int indice) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::vcdOptionsUpdated" << std::endl;
#endif
    this->systemParameters->setVcdOption( quint32(indice) );
    this->mainWindow->setWindowModified(true);

}

void Control::fClkFirstUpdated(double newValue) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::fClkFirstUpdated" << std::endl;
#endif
    this->systemParameters->setfClkFirst( (float)newValue );
    this->systemParameters->setTClk( (1.0 / newValue ) * 1000.0 );
    this->systemParameters->setChannelBandwidth( newValue * this->systemParameters->getDataWidth());
    this->mainWindow->setWindowModified(true);

}

void Control::fClkLastUpdated(double newValue) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::fClkLastUpdated" << std::endl;
#endif

    this->systemParameters->setfClkLast( (float)newValue );
    this->mainWindow->setWindowModified(true);

}

void Control::fClkStepUpdated(double newValue) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::fClkStepUpdated" << std::endl;
#endif
    this->systemParameters->setfClkStep( (float)newValue );
    this->mainWindow->setWindowModified(true);

}

void Control::fClkStepTypeUpdated(int newValue) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::fClkStepTypeUpdated" << std::endl;
#endif
    this->systemParameters->setfClkStepType( quint32(newValue) );
    this->mainWindow->setWindowModified(true);

}

void Control::copySystemParameters() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::copySystemObjects" << std::endl;
#endif

    this->xSize = this->systemParameters->getXSize();
    this->ySize = this->systemParameters->getYSize();
    this->dataWidth = this->systemParameters->getDataWidth();

    this->vcdOption = this->systemParameters->getVcdOption();

}

void Control::runBuilder() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::runBuilder" << std::endl;
#endif

    /** Verifica se:
     *  (1) Há algum padrão de tráfico ativo;
     *  (2) Existe experimento repetido;
     *  (3) os parâmetros do sistema estão OK
     */
    if( !inputsOk() ) {
        finishSimulation( Control::InputsError );
        return;
    }

    this->mainWindow->printConsole(trUtf8("<font color='green'>Building simulator(s)</font>"));

    unsigned int numExecucoesExperimento = this->calcAmountExperimentsExecutions();

#ifndef Q_OS_UNIX // Para plataforma windows é necessário o diretório do compilador
    builder = new Builder(systemParameters,experimentManager,
                          trafficPatternManager,numExecucoesExperimento,
                          conf->getWorkFolder(),conf->getSystemCFolder(),conf->getMinGWFolder());
#else
    builder = new Builder(systemParameters,experimentManager,
                          trafficPatternManager,numExecucoesExperimento,
                          conf->getWorkFolder(),conf->getSystemCFolder());
#endif

    this->copySystemParameters();

    QThread* threadConstrucao = new QThread(this);
    threadConstrucao->setObjectName("Builder");
    connect(threadConstrucao,SIGNAL(started()),builder,SLOT(execute()));
    connect(threadConstrucao,SIGNAL(finished()),builder,SLOT(deleteLater()));
    connect(threadConstrucao,SIGNAL(finished()),threadConstrucao,SLOT(deleteLater()));
    connect(builder,SIGNAL(finished()),threadConstrucao,SLOT(quit()));
    connect(builder,SIGNAL(finished()),builder,SLOT(deleteLater()));
    connect(builder,SIGNAL(finished()),this,SLOT(finishBuilding()));
    connect(builder,SIGNAL(sendMessage(QString)),mainWindow,SLOT(printConsole(QString)));
    connect(builder,SIGNAL(unsuccessful()),this,SLOT(buildUnsuccessfull()));
    connect(builder,SIGNAL(sendDirs(QList<QString*>*)),this,SLOT(receiveDirsExecution(QList<QString*>*)));
    builder->moveToThread(threadConstrucao);
    buildSuccessfully = true;
    analysisOk = false;
    // Iniciar a construção dos simuladores
    this->mainWindow->setLimitsProgressBar(0,0);
    this->mainWindow->setActionSaveSimulationEnabled(false);
    this->mainWindow->setActionGenerateCSVEnabled(false);
    threadConstrucao->start();

}

void Control::buildUnsuccessfull() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::buildUnsuccessfull" << std::endl;
#endif
    buildSuccessfully = false;
}

void Control::receiveDirsExecution(QList<QString *> *dirs) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::receiveDirsExecution" << std::endl;
#endif

    if(simulationFolders != NULL) {
        for(int i = 0; i < simulationFolders->size(); i++) {
            QString* dir = simulationFolders->at(i);
            delete dir;
        }
        simulationFolders->clear();
        delete simulationFolders;
    }

    this->simulationFolders = dirs;
}

void Control::finishBuilding() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::finishBuilding" << std::endl;
#endif

    this->builder = NULL;
    // Final da construção dos simuladores
    if( buildSuccessfully ) {
        unsigned int numExperimentos = 0;
        for(unsigned int i = 0; i < 5; i++) {
            Experiment* exp = this->experimentManager->getExperiment(i);
            if(exp != NULL) {
                if(exp->isActive()) {
                    numExperimentos++;
                }
            }
        }

        unsigned int numExecucoes = simulationFolders->size();
        this->mainWindow->setLimitsProgressBar( 0,numExecucoes );

        this->mainWindow->printConsole( trUtf8("- Number of simulations to be run: %1").arg(numExecucoes) );

        /// Gerenciador de threads da execução -- o primeiro parâmetro é o número de threads para serem executadas paralelamente
        threadManager = new ThreadManager( (int) conf->getThreadNumber() , this );

        connect(threadManager,SIGNAL(allFinished()),this,SLOT(finishExecution()));
        connect(threadManager,SIGNAL(threadFinished(int)),this,SLOT(updateStatusExecution(int)));

        for(unsigned int i = 0; i < numExecucoes; i++) {
            QString* diretorio = simulationFolders->at(i);
            int indiceUltimoDiretorio = diretorio->lastIndexOf("/")+1;                              // - MHz == -3 chars
            int FClk = diretorio->mid( indiceUltimoDiretorio, diretorio->size() - indiceUltimoDiretorio - 3  ).toInt();
            float TClk = (1.0/ (float) FClk) * 1000.0;
#ifdef Q_OS_WIN32
            SimulationPerformer* exeSystem = new SimulationPerformer(TClk,*diretorio,conf->getMinGWFolder());
#else
            SimulationPerformer* exeSystem = new SimulationPerformer(TClk,*diretorio);
#endif
            exeSystem->setObjectName(QString("Execution%1").arg(i));
            QThread* threadExecucao = new QThread(this);
            threadExecucao->setObjectName(QString("Execution%1").arg(*diretorio));
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
        }
        timer->start();
        threadManager->runThreads();
    } else {
        this->finishSimulation(Control::BuildFailed);
    }

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
        case Control::BuildFailed :
            mainWindow->printConsole( trUtf8("<font color=red>Build Failed</font>") );
            break;
        case Control::ExecuteFailed :
            mainWindow->printConsole(trUtf8("<font color=red>Execute Failed</font>"));
            delete this->threadManager;
            if(simulationFolders != NULL) {
                for(int i = 0; i < simulationFolders->size(); i++) {
                    QString* dir = simulationFolders->at(i);
                    delete dir;
                }
                simulationFolders->clear();
                delete simulationFolders;
                simulationFolders = NULL;
            }
            threadManager = NULL;
            break;
        case Control::InputsError :
            mainWindow->printConsole(trUtf8("<font color=red>Inputs error</font>"));
            break;
        case Control::Success: {
            char* tempo = Util::formatTime( qlonglong(timer->elapsed()) );
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

    if( builder ) {
        // Se tiver no processo de construção do simulador, cancela a construção
        builder->cancel();
        builder->thread()->quit();
    } else {
        // Se não, é pq esta na simulação. Cancela portanto a simulação
        finishSimulation( Control::Cancel );
    }


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
    // Ainda não há resultados de simulações
    if(simulationFolders == NULL) {
        this->mainWindow->printConsole( trUtf8("<font color=red>Impossible generate analysis because there is no system simulation results</font>") );
        return;
    }

    int size = simulationFolders->size();
    QStringList items;
    for(int i = 0; i < size; i++) {
        items.append(* simulationFolders->at(i) );
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
    Analyzer* analyzer = new Analyzer(this->simulationFolders,xSize,ySize,dataWidth,lower,upper);
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

    if( this->vcdOption == 0 ) {
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
                item = *simulationFolders->at(0);
            } else {
                QStringList dirs;
                for(int i = 0; i < size; i++) {
                    dirs.append(* simulationFolders->at(i) );
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

//#define GNUPLOT
void Control::viewGraphic(AnalysisOptions *aop) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Control::viewGraphic" << std::endl;
#endif

    if( !analysisOk ) {
        this->mainWindow->printConsole( trUtf8("<font color=red>Impossible plot graphic because there is not analysis results</font>") );
        delete aop;
        return;
    } else {

        QVector<QList<DataReport *> *> *data = getReportData(aop);
        if(data == NULL) {
            mainWindow->printConsole( trUtf8("<font color=red>Report file unavailable or this flow is null (no packet was transfered)</font>"));
            return;
        }

        // Selected graphic
        Plotter* plotter = NULL;
#ifdef GNUPLOT
        plotter = new GnuPlotPlotter(this);
#else
//        plotter =  new CustomPlotter(mainWindow);
        plotter =  new QwtPlotter(mainWindow);
#endif
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
        folders.append(* simulationFolders->at(i) );
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
                filename += QString("/flow_%1_%2_%3_%4_%5")
                        .arg(aop->getXSrc())
                        .arg(aop->getYSrc())
                        .arg(aop->getXDest())
                        .arg(aop->getYDest())
                        .arg( aop->getTrafficPattern() );
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
            delete aop;
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
    conf->setMinGWFolder( env->getMinGWFolder() );
    conf->setSystemCFolder( env->getSystemCFolder() );
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

    if( this->simulationFolders == NULL ) {
        return; // Não há resultado válido
    }

    QString dir0 = *this->simulationFolders->at(0);
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
        QString dir = *simulationFolders->at(i);
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
                for(int i = 0; i < simulationFolders->size(); i++) {
                    QString* dir = simulationFolders->at(i);
                    delete dir;
                }
                simulationFolders->clear();
                delete simulationFolders;
                simulationFolders = NULL;
            }
            simulationFolders = new QList<QString*>();
            for( int i = 0; i < dirs.size(); i++ ) {
                QString* dir = new QString( workDirSimulationLoaded + dirs.at(i) );
                simulationFolders->append( dir );
            }
        }
        tmp->close();
        this->copySystemParameters();
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
