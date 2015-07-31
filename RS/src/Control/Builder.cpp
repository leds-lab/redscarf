/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Builder.cpp
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

#include "include/Control/Builder.h"

#include "include/Model/System/SystemParameters.h"
#include "include/Model/System/ExperimentManager.h"
#include "include/Model/System/GSoC.h"
#include "include/Model/System/GSoCINStrategy.h"
#include "include/Model/System/GSoCIN2VcStrategy.h"
#include "include/Model/System/SystemDefines.h"
#include "include/Model/Traffic/TrafficModelGenerator.h"

#include <QTextStream>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QDateTime>
#include <QSettings>
#include <QApplication>


#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

Builder::Builder(SystemParameters *sp, ExperimentManager *ge,
                 TrafficPatternManager *gpt, unsigned int amountExperimentsExecute, QString diretorioTrabalho,
                 QString systemc_dir, QString compilerDir, QObject *parent)
    : QObject(parent),workDir(diretorioTrabalho),canceled(false),systemParameters(NULL) {

#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Control/Builder" << std::endl;
#endif

    this->systemParameters = new SystemParameters(*sp);
    this->experimentManager = ge;
    this->gpt = gpt;

    this->builder = new QProcess(this);
    this->systemcDir = systemc_dir;
    this->compilerDir = compilerDir;
    this->amountExperimentsExecute = amountExperimentsExecute;

    this->executionFolders = new QList<QString*>();

    connect(builder,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(builder,SIGNAL(finished(int)),builder,SLOT(kill()));
    connect(builder,SIGNAL(finished(int)),builder,SLOT(deleteLater()));
    connect(this,SIGNAL(finished()),builder,SLOT(kill()));
    connect(this,SIGNAL(finished()),builder,SLOT(deleteLater()));
}

bool Builder::generateMakefile() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Builder::generateMakefile" << std::endl;
#endif

    QDir dir(".");

    if(!dir.exists("system")) {
        dir.mkdir("system");
        this->sendMessage( trUtf8("Directory system created"));
    }
    QFile makefile(dir.path()+"/system/Makefile");
    if(makefile.exists()) {
        makefile.remove();
    }

    QDir sysCDir( this->systemcDir );
    QStringList dirs = sysCDir.entryList();
    dirs = dirs.filter("lib");
    if(dirs.isEmpty()) {
        emit sendMessage(trUtf8("<font color=red>Library not found in SystemC folder</font>"));
        return false;
    }

    QStringList cppSources;
    QSettings settings(qApp->applicationDirPath()+"/etc/system.ini",QSettings::IniFormat);
    int size = settings.beginReadArray("System_Sources");
    for( int i = 0; i < size; i++ ) {
        settings.setArrayIndex(i);
        cppSources.append( settings.value("CppFile").toString() );
    }
    settings.endArray();

    int cppCount = cppSources.size();
    if( cppCount == 0 ) {
        this->sendMessage(trUtf8("<font color=red>The etc/system.ini configuration file isn't defined. Please correct</font>"));
        return false;
    }


    if(!makefile.open(QIODevice::WriteOnly)) {
        this->sendMessage(trUtf8("<font color=red>Error in generate Makefile. Without write file permission</font>"));
        return false;
    }

    QTextStream ts(&makefile);
    ts << "SYSTEMC = "+ this->systemcDir;

    QString dirLib = "$(SYSTEMC)/" + dirs.at(0);
    ts << "\nLIBDIR += "+dirLib;
    ts << "\nLINKER += -Xlinker -rpath -Xlinker $(LIBDIR)";
    ts << "\nLIBS   += -L$(LIBDIR) -lsystemc $(LINKER)";
    ts << "\nINCDIR += -I. -I$(SYSTEMC)/include";

    // It include all CPP sources from SoCIN model to Makefile
    ts << "\nCPPSRCS  +=";
    for( int i = 0; i < cppCount; i++ ) {
        QString cppFile = cppSources.at(i) + ".cpp";
        ts << " " << cppFile;
    }

    ts << "\n\nCPPOBJS   = $(CPPSRCS:.cpp=.o)";
    ts << "\nOBJS      = $(CPPOBJS)";
    ts << "\nCXX       = g++";

    ts << "\n\nsystem.x: $(OBJS)";
    ts << "\n\t$(QUIET)$(CXX) $(CFLAGS) -o $@ $(OBJS) $(LIBS) 2>&1 | c++filt";
    ts << "\n\t@echo $(ECHO)";
    ts << "\n\t@echo \"  [$(RED)LINK$(ENDCOLOR)]     :  $@\"$(ECHO)";
    ts << "\n\t@echo $(ECHO)";

    ts << "\n\n%.o:    %.cpp";
    ts << "\n\t$(QUIET)$(CXX) $(CFLAGS) $(INCDIR) -c $<";
    ts << "\n\t@echo \"  [$(BLUE)CXX$(ENDCOLOR)]        :  $@\"$(ECHO)";

    ts << "\n\nclean:";
#ifdef Q_OS_UNIX
    ts << "\n\trm -f ";
#else
    ts << "\n\terase ";
#endif
    ts << "*.x *.o";

    ts << "\n\n############## Color Codes";
    ts << "\nRED          = <font color=red>";
    ts << "\nBLUE         = <font color=blue>";
    ts << "\nENDCOLOR     = </font>";

    ts << "\n\nQUIET = @";
    ts << "\nECHO = ";

    makefile.close();
    this->sendMessage(trUtf8("- Makefile successfully created"));
    return true;

}

void Builder::removeObjects(const QString path) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Builder::removeObjects" << std::endl;
#endif
    QDir dir(path);
    dir.setNameFilters(QStringList() << "*.o");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList()) {
        dir.remove(dirFile);
    }
}

void Builder::execute() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Builder::execute" << std::endl;
#endif

    builder->setWorkingDirectory("./system");
    QString currentPath = QDir::currentPath();
    this->removeObjects( currentPath+"/system/" );
    builder->setProcessChannelMode( QProcess::MergedChannels );

#ifndef Q_OS_UNIX
/// Configure PATH compiler environment variable for windows
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PATH",env.value("Path") + ";"+compilerDir+"\\bin");
    builder->setProcessEnvironment(env);
#endif

    if( !this->generateMakefile() ) {
        QFile::remove(currentPath+"/system/Makefile");
        emit this->unsuccessful();
        emit finished();
        return;
    }

    SystemDefines* def = SystemDefines::getInstance();

    // Get date and time
    QDateTime dateTime = QDateTime::currentDateTime();
    // Get a string with date and time in ISO format
    QString dateTimeDir = dateTime.toString(Qt::ISODate);
    dateTimeDir.replace(':','-');
    // Para manipular os diretórios a serem utilizados
    QDir dirWork = QDir(workDir + "/" + dateTimeDir);
    // Criar diretório de trabalho
    if(!dirWork.exists()) {
        dirWork.mkpath(".");
        emit sendMessage(trUtf8("- Work directory created"));
    }

    // Para cada experimento gera o simulador -- system.x
    for( unsigned int i = 1; i <= 5; i++ ) {
        Experiment* exp = this->experimentManager->getExperiment(i);
        if(exp != NULL) {
            if(exp->isActive()) {
                GSoC* gSoC = new GSoC(systemParameters,exp);
                GNoC* gNoC = NULL;
                switch (exp->getRouterArchitecture()) {
                    case 0: // ParIS       -- De acordo com a posição no combo box da view
                        gNoC = new GSoCINStrategy(systemParameters,exp);
                        break;
                    case 1: // ParIS 2
                    case 2: // ParIS 2.ag
                        gNoC = new GSoCIN2VcStrategy(systemParameters,exp);
                        break;
                }
                emit sendMessage( trUtf8("<font color=green><br />Building the simulator for Configuration #%1</font>").arg(i) );
                // Tentando gerar a NoC
                try {
                    gNoC->generateNoc();
                    delete gNoC;
                    emit sendMessage(trUtf8("- NoC Model generated"));
                } catch(const char* exception) {
                    emit sendMessage(trUtf8("Error in generate NoC: %1 of experiment: %2")
                                     .arg(QString::fromStdString(exception))
                                     .arg(i));
                    this->removeObjects( currentPath+"/system/" );

                    emit this->unsuccessful();
                    QFile::remove(currentPath+"/system/Makefile");
                    delete gNoC;
                    emit finished();
                    return;
                }
                // Tentando gerar o SoC
                try {
                    gSoC->generateSoC();
                    delete gSoC;
                    emit sendMessage(trUtf8("- SoC model generated"));
                } catch(const char* exception) {
                    emit sendMessage(trUtf8("Error in generate SoC: %1 of experiment: %2")
                                     .arg(QString::fromStdString(exception))
                                     .arg(i));
                    this->removeObjects( currentPath+"/system/" );

                    emit this->unsuccessful();
                    emit finished();
                    QFile::remove(currentPath+"/system/Makefile");
                    delete gSoC;
                    return;
                }
                this->copyFromCache(exp);
                this->sendMessage(trUtf8("- Compiling the simulator . . ."));
#ifdef Q_OS_UNIX
                builder->start("make");
#else
                builder->start(compilerDir+"\\bin\\mingw32-make");
#endif
                if(!builder->waitForFinished(-1)) {
                    emit this->sendMessage( trUtf8("Make failed in experiment %1: %2").arg(i).arg(builder->errorString()) );
                    this->removeObjects( currentPath+"/system/" );

                    emit this->unsuccessful();
                    emit finished();
                    QFile::remove(currentPath+"/system/Makefile");
                    return;
                }

                if( builder->exitCode() != 0 || canceled ) {
                    this->removeObjects( currentPath+"/system/" );

                    emit this->unsuccessful();
                    emit finished();
                    QFile::remove(currentPath+"/system/Makefile");
                    return;
                }
                this->sendMessage(trUtf8("- Simulator generated"));

                QString dirGenerated = QString("dir_%1_%2_%3_%4_IN%5_OUT%6")
                        .arg(QString::fromStdString(def->findRouterArchitectures(exp->getRouterArchitecture())))
                        .arg(QString::fromStdString(def->findRoutingAlgorithms(exp->getRoutingAlgorithm())))
                        .arg(QString::fromStdString(def->findFlowControls(exp->getFlowControl())))
                        .arg(QString::fromStdString(def->findArbiterTypes(exp->getArbiterType())))
                        .arg(exp->getInputBufferSize())
                        .arg(exp->getOutputBufferSize());
                QString diretorioExperimento = dirWork.absolutePath() + "/" + dirGenerated;
                dirWork.mkdir( diretorioExperimento );
///{ Removendo o antigo e copiando novo. OBS: Se for armazenar, refatorar
                QFile::remove( diretorioExperimento + "/system.x" );
                QFile::rename( currentPath + "/system/system.x",diretorioExperimento+"/system.x");

                QFile::remove( diretorioExperimento + "/list_nodes.sav" );
                QFile::rename( currentPath + "/system/list_nodes.sav",diretorioExperimento+"/list_nodes.sav");
///}

                this->copyToCache(exp);
                this->removeObjects( currentPath+"/system/" );

                /// Gerar arquivo do modelo de tráfego para este experimento
                if( this->generateTrafficModels( exp,diretorioExperimento ) == -1 ) {
                    this->removeObjects( currentPath+"/system/" );
                    emit unsuccessful();
                    emit finished();
                    QFile::remove(currentPath+"/system/Makefile");
                    return;
                }

            }
        }
    }

    QFile::remove( currentPath + "/system/Makefile" );

    emit sendDirs( this->executionFolders );
    emit sendMessage(trUtf8("<br /><font color=green>Launching the Simulators</font>"));
    emit this->finished();
}

int Builder::generateTrafficModels(Experiment* experiment,QString dir) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Builder::generateTrafficModels" << std::endl;
#endif

    unsigned int experimentoAtual = 0;

    SystemParameters* temp = this->systemParameters;
    this->systemParameters = new SystemParameters(*temp);

    float fclk = this->systemParameters->getfClkFirst();
    float fclkPrevious = fclk;
    int i = 0;

    /*
     * Gerando stopsim.par
     */
    QFile* stopSimPar = new QFile( dir+"/stopsim.par");
    if(!stopSimPar->open(QIODevice::WriteOnly)) {
        emit this->sendMessage(trUtf8("Impossible to open file  %1. Aborted. ").arg(stopSimPar->fileName()));
        delete systemParameters;
        systemParameters = temp;
        return -1;
    }

    switch(systemParameters->getStopOption()) {
        case 0:
            systemParameters->setStopTime_ns(0);
            systemParameters->setStopTime_cycles(0);
            break;
        case 1:
            systemParameters->setStopTime_cycles( (unsigned long int) systemParameters->getStopTime_ns()/systemParameters->getTClk() );
            break;
        case 2:
            systemParameters->setStopTime_ns( (unsigned long int) systemParameters->getStopTime_cycles()*systemParameters->getTClk());
            break;
    }
    QString stopSimParContent = QString("%1\t%2").arg(systemParameters->getStopTime_cycles()).arg(systemParameters->getStopTime_ns());
    stopSimPar->write(stopSimParContent.toUtf8());
    stopSimPar->close();
    delete stopSimPar;
    /*
     * Fim geração stopsim.par
     */
    QSettings settings(qApp->applicationDirPath()+"/etc/system.ini",QSettings::IniFormat);
    settings.beginGroup("Traffic_Parameters");
    bool useDefault = settings.value("useDefault",true).toBool();
    QString alternativeFile = settings.value("alternative",QString()).toString();
    settings.endGroup();

    while(experimentoAtual < amountExperimentsExecute) {

        /*
         * Inicio geração arquivo traffic.tcf
         */
        QDir diretorioAtual = QDir( dir ) ;
        diretorioAtual.mkdir(QString("%1MHz").arg(fclk));
        diretorioAtual.cd(QString("%1MHz").arg(fclk));
        QString diretorio = diretorioAtual.absolutePath();
        TrafficModelGenerator* trafficGen = new TrafficModelGenerator(this->systemParameters,experiment,this->gpt);
        try {
            if( useDefault ) {
                trafficGen->generateTraffic( diretorio.toStdString().c_str() );
            } else {
                QFile::copy(alternativeFile,diretorio+"/traffic.tcf");
            }
            this->executionFolders->append( new QString(diretorio) );
            delete trafficGen;
//            emit sendMessage(trUtf8("Traffic Model successfully generated for %1MHz").arg(fclk));
        } catch (const char* exception) {
            emit sendMessage(trUtf8("Error in generate traffic model: %1").arg(QString::fromStdString(exception)));
            delete trafficGen;
            delete systemParameters;
            systemParameters = temp;
            return -1;
        }
        /*
         * Fim geração traffic.tcf
         */
        if( systemParameters->getfClkStepType() == 0 ) {
            fclk += systemParameters->getfClkStep();
            if( (systemParameters->getfClkFirst() > systemParameters->getfClkLast() ) && (fclk < systemParameters->getfClkLast()) ) {
                fclk = systemParameters->getfClkLast();
            }
            if( (systemParameters->getfClkFirst() < systemParameters->getfClkLast()) && (fclk > systemParameters->getfClkLast()) ) {
                fclk = systemParameters->getfClkLast();
            }
        } else {
            do {
                fclkPrevious = fclk;
                int tmp = (unsigned int) (systemParameters->getfClkFirst() / ( exp(i*systemParameters->getfClkStep()) )*10 );
                fclk = ((float) tmp) / 10.0;
                i++;
            } while (fclk == fclkPrevious);
        }

        systemParameters->setTClk( (1.0/fclk)*1000.0 );
        systemParameters->setChannelBandwidth( fclk * systemParameters->getDataWidth() );

        experimentoAtual++;
    }
    emit this->sendMessage(trUtf8("<font color=blue>- Traffic Models generated</font>"));

    delete systemParameters;
    this->systemParameters = temp;
    return 0;
}

void Builder::cancel() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Builder::cancel" << std::endl;
#endif

    canceled = true;
    if( builder->state() != QProcess::NotRunning ) {
        builder->kill();
    }
}

void Builder::readyRead() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Builder::readyRead" << std::endl;
#endif

    emit this->sendMessage( QString::fromUtf8(builder->readAll()) );
}

void Builder::copyToCache(Experiment *exp) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Builder::copyToCache" << std::endl;
#endif

    QString systemDir = QDir::currentPath() + "/system/";

    QDir dirCache(systemDir+"cache");
    if( !dirCache.exists() ) {
        dirCache.mkpath(".");
    }

    QString cacheDir = dirCache.absolutePath() + "/";

    unsigned int dataWidth = this->systemParameters->getDataWidth();
    unsigned int ribWidth = this->systemParameters->getDestinationAddressWidth();

    QSettings settings(qApp->applicationDirPath()+"/etc/system.ini",QSettings::IniFormat);
    int size = settings.beginReadArray("System_Sources");
    for( int i = 0; i < size; i++ ) {
        settings.setArrayIndex(i);
        QString filename = settings.value("CppFile").toString();
        QString cppFilename = filename + ".o";
        QStringList cacheOption = settings.value("CacheOptions").toStringList();

        /*! Cache options:
         *   % noCache: It don't performs cache
         *   % noOptions: It performs cache without options
         *   % dataWidth: It performs cache with dataWidth option
         *   % inputBufferSize: It performs cache with inputBufferSize option
         *   % outputBufferSize: It performs cache with outputBufferSize option
         *   % ribWidth: It performs cache with ribWidth option
         *   % safOption: It performs cache with safOption option
         *   % flowControl: It performs cache with flowControl option
         *   % ageWidth: It performs cache with ageWidth option
         *
         * OBS: The options can be mixed
         */
        if(cacheOption.contains("noCache")) {
            continue;
        }
        if(cacheOption.contains("dataWidth")) {
            filename += QString("_%1").arg(dataWidth);
        }
        if( cacheOption.contains("inputBufferSize") ) {
            filename += QString("_%1").arg(exp->getInputBufferSize());
        }
        if(cacheOption.contains("outputBufferSize")) {
            filename += QString("_%1").arg(exp->getOutputBufferSize());
        }
        if(cacheOption.contains("ribWidth")) {
            filename += QString("_%1").arg(ribWidth);
        }
        if(cacheOption.contains("safOption")) {
            filename += QString("_%1").arg(systemParameters->getSafOption());
        }
        if(cacheOption.contains("flowControl")) {
            filename += QString("_%1").arg(exp->getFlowControl());
        }
        if(cacheOption.contains("ageWidth")) {
            filename += QString("_%1").arg(AGE_WIDTH);
        }

        filename += ".o";

        QFile::copy( systemDir+cppFilename,cacheDir+filename );
    }
    settings.endArray();

}

void Builder::copyFromCache(Experiment *exp) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/Builder::copyFromCache" << std::endl;
#endif

    QString systemDir = QDir::currentPath() + "/system/";

    QDir dirCache(systemDir+"cache");
    if( !dirCache.exists() ) {
        return;
    }

    QString cacheDir = dirCache.absolutePath() + "/";

    unsigned int dataWidth = this->systemParameters->getDataWidth();
    unsigned int ribWidth = this->systemParameters->getDestinationAddressWidth();

    QSettings settings(qApp->applicationDirPath()+"/etc/system.ini",QSettings::IniFormat);
    int size = settings.beginReadArray("System_Sources");
    for( int i = 0; i < size; i++ ) {
        settings.setArrayIndex(i);
        QString filename = settings.value("CppFile").toString();
        QString cppFilename = filename + ".o";
        QStringList cacheOptions = settings.value("CacheOptions").toStringList();

        /*! Cache options:
         *   % noCache: It don't rescues from cache
         *   % noOptions: It rescues from cache without options
         *   % dataWidth: It rescues from cache with dataWidth option
         *   % inputBufferSize: It rescues from cache with inputBufferSize option
         *   % outputBufferSize: It rescues from cache with outputBufferSize option
         *   % ribWidth: It rescues from cache with ribWidth option
         *   % safOption: It rescues from cache with safOption option
         *   % flowControl: It rescues from cache with flowControl option
         *   % ageWidth: It rescues from cache with ageWidth option
         *
         * OBS: The options can be mixed
         */
        if(cacheOptions.contains("noCache")) {
            continue;
        }
        if(cacheOptions.contains("dataWidth")) {
            filename += QString("_%1").arg(dataWidth);
        }
        if( cacheOptions.contains("inputBufferSize") ) {
            filename += QString("_%1").arg(exp->getInputBufferSize());
        }
        if(cacheOptions.contains("outputBufferSize")) {
            filename += QString("_%1").arg(exp->getOutputBufferSize());
        }
        if(cacheOptions.contains("ribWidth")) {
            filename += QString("_%1").arg(ribWidth);
        }
        if(cacheOptions.contains("safOption")) {
            filename += QString("_%1").arg(systemParameters->getSafOption());
        }
        if(cacheOptions.contains("flowControl")) {
            filename += QString("_%1").arg(exp->getFlowControl());
        }
        if(cacheOptions.contains("ageWidth")) {
            filename += QString("_%1").arg(AGE_WIDTH);
        }

        filename += ".o";

        QFile::copy( cacheDir+filename,systemDir+cppFilename);
    }
    settings.endArray();

}

Builder::~Builder() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Control/Builder" << std::endl;
#endif
    if( this->systemParameters != NULL ) {
        delete this->systemParameters;
    }
}
