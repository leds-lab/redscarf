/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* MainWindow.cpp
* Copyright (C) 2014 - 2017 LEDS - Univali <zeferino@univali.br>
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
* 22/11/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*
*/

#include "ui_MainWindow.h"
#include "View/MainWindow.h"

#include "View/TrafficConfigurationDialog.h"
#include "View/AboutWindow.h"
#include "View/AnalysisOptions.h"

#include "Model/System/Defines.h"
#include "Model/System/SystemDefines.h"
#include "Model/System/Experiment.h"
#include "Model/System/SystemParameters.h"
#include "Model/System/SystemOperation.h"
#include "Model/System/SystemConfiguration.h"
#include "Model/Traffic/TrafficParameters.h"

#include "Control/EnvironmentConfiguration.h"

#include <cmath>

#include <QMessageBox>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QColorDialog>
#include <QCloseEvent>
#include <QTextEdit>
#include <QSplitter>
#include <QDockWidget>
#include <QTranslator>
#include <QLibraryInfo>
#include <QSettings>


#define MAX_EXPERIMENTS 5

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class View/MainWindow" << std::endl;
#endif
    ui->setupUi(this); // Setup the mainwindown graphical form

    // Regisger type to use as variant value
    qRegisterMetaType< QList<QVariant> >( "QList<QVariant>" );

    // Center the window on the center - disabled - the OS manager define the most adequated position
//    this->centerWindow();
    this->configureToolBar();
    this->insertComboItens();
    this->setProperties();
    this->establishConnections();
    this->loadDefaultValues();
    this->configureWidgets();

}

MainWindow::~MainWindow() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class View/MainWindow" << std::endl;
#endif

    for( int i = 0; i < 5; i++ ) {
        delete this->graphLineColors[i];
    }
    delete ui;
}

void MainWindow::configureWidgets() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::configureWidgets" << std::endl;
#endif

    QAction* deleteItem = new QAction(ui->listConf);
    deleteItem->setShortcut(Qt::Key_Delete);
    ui->listConf->addAction(deleteItem);
    connect(deleteItem,SIGNAL(triggered(bool)),this,SLOT(removeSelectedItems()));

    // Disable spin boxes of stop options - The default option is until all packets are delivered
    ui->spinBoxStopTimeNs->setVisible(false);
    ui->spinBoxStopTimeCycles->setVisible(false);
    ui->spinBoxStopNumberOfPacketsDelivered->setVisible(false);

    // Desabilitando a barra de progresso da execuçao
    ui->progressBar->setVisible(false);
    // Colocando a barra de progresso na barra de status
    ui->statusBar->addPermanentWidget(ui->progressBar);

    // Desabilitando a opção de salvar a simulação que só fica disponível
    // após a execução de 1 experimento
    ui->actionSaveSimulation->setEnabled(false);

    // Desabilitando a opção de gerar o CSV que só fica disponível
    // após a execução de análise de 1 experimento
    ui->actionGenerateCSV->setEnabled(false);

    // Transformando o console em um Widget Dockable
    QDockWidget* consoleDockWidget = new QDockWidget(trUtf8("Console"),this);
    consoleDockWidget->setObjectName("consoleDock");
    consoleDockWidget->setWidget(ui->groupBoxConsole);
    addDockWidget(Qt::BottomDockWidgetArea,consoleDockWidget);

    // Adicionando ações ao menu dos componentes de visualização
    // opcional na tela | Barra de Ferramentas e Console
    ui->menuView->addActions( createPopupMenu()->actions() );

}

void MainWindow::configureToolBar() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::configureToolBar" << std::endl;
#endif
    ui->toolBar->setWindowTitle(trUtf8("Toolbar"));
    ui->toolBar->addAction( ui->actionNew );
    ui->toolBar->addAction( ui->actionOpen );
    ui->toolBar->addAction( ui->actionSave );
    ui->toolBar->addAction( ui->actionSaveAsDefault );
    ui->toolBar->addSeparator();
    ui->toolBar->addAction( ui->actionLoadSimulation );
    ui->toolBar->addAction( ui->actionSaveSimulation );
    ui->toolBar->addAction( ui->actionGenerateCSV );
}

void MainWindow::loadDefaultValues() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::loadDefaultValues" << std::endl;
#endif

    ui->comboTopology->setCurrentIndex(1); // Default is 2D-Orthogonal topology

    ui->spinBoxStopTimeNs->setValue( qint32(DefaultValuesSystem::DEFAULT_STOP_TIME_NS) );
    ui->spinBoxStopTimeCycles->setValue( qint32(DefaultValuesSystem::DEFAULT_STOP_TIME_CYCLES) );
    ui->comboInSimulationStopMethod->setCurrentIndex( qint32(DefaultValuesSystem::DEFAULT_STOP_OPTION) );

    ui->comboInGenerateVCD->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_VCD_OPTION));
    ui->doubleSpinInChannelFclkRange->setValue(qint32(DefaultValuesSystem::DEFAULT_FCLK_FIRST));
    ui->doubleSpinInChannelFclkRange_2->setValue(qint32(DefaultValuesSystem::DEFAULT_FCLK_LAST));
    ui->comboInStep->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_FCLK_STEP_TYPE));
    ui->doubleSpinInStep->setValue(qint32(DefaultValuesSystem::DEFAULT_FCLK_STEP_VALUE));
    ui->doubleSpinInChannelTclkRange->setValue(qint32(DefaultValuesSystem::DEFAULT_TCLK));
    ui->doubleSpinInChannelTclkRange_2->setValue(qint32(DefaultValuesSystem::DEFAULT_TCLK));
    ui->doubleSpinInChannelBWRange->setValue(qint32(DefaultValuesSystem::DEFAULT_CHANNEL_BW));
    ui->doubleSpinInChannelBWRange_2->setValue(qint32(DefaultValuesSystem::DEFAULT_CHANNEL_BW));

    ui->comboInTopologyExp1->setCurrentIndex( qint32(DefaultValuesSystem::DEFAULT_TOPOLOGY) );
    ui->comboInTopologyExp2->setCurrentIndex( qint32(DefaultValuesSystem::DEFAULT_TOPOLOGY) );
    ui->comboInTopologyExp3->setCurrentIndex( qint32(DefaultValuesSystem::DEFAULT_TOPOLOGY) );
    ui->comboInTopologyExp4->setCurrentIndex( qint32(DefaultValuesSystem::DEFAULT_TOPOLOGY) );
    ui->comboInTopologyExp5->setCurrentIndex( qint32(DefaultValuesSystem::DEFAULT_TOPOLOGY) );

    ui->comboInRoutingAlgorithmExp1->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE));
    ui->comboInRoutingAlgorithmExp2->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE));
    ui->comboInRoutingAlgorithmExp3->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE));
    ui->comboInRoutingAlgorithmExp4->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE));
    ui->comboInRoutingAlgorithmExp5->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE));

    ui->comboInSwitchingExp1->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_FC_TYPE));
    ui->comboInSwitchingExp2->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_FC_TYPE));
    ui->comboInSwitchingExp3->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_FC_TYPE));
    ui->comboInSwitchingExp4->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_FC_TYPE));
    ui->comboInSwitchingExp5->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_FC_TYPE));

    ui->comboInArbiterTypeExp1->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE));
    ui->comboInArbiterTypeExp2->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE));
    ui->comboInArbiterTypeExp3->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE));
    ui->comboInArbiterTypeExp4->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE));
    ui->comboInArbiterTypeExp5->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE));

    ui->comboInVCExp1->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_VC_OPTION));
    ui->comboInVCExp2->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_VC_OPTION));
    ui->comboInVCExp3->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_VC_OPTION));
    ui->comboInVCExp4->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_VC_OPTION));
    ui->comboInVCExp5->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_VC_OPTION));

    ui->spinInInputBuffersExp1->setValue(qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH));
    ui->spinInInputBuffersExp2->setValue(qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH));
    ui->spinInInputBuffersExp3->setValue(qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH));
    ui->spinInInputBuffersExp4->setValue(qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH));
    ui->spinInInputBuffersExp5->setValue(qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH));

    ui->spinInOutputBuffersExp1->setValue(qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH));
    ui->spinInOutputBuffersExp2->setValue(qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH));
    ui->spinInOutputBuffersExp3->setValue(qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH));
    ui->spinInOutputBuffersExp4->setValue(qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH));
    ui->spinInOutputBuffersExp5->setValue(qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH));

    this->graphLineColors[0] = new QColor( Qt::red );
    this->graphLineColors[1] = new QColor( Qt::blue );
    this->graphLineColors[2] = new QColor( Qt::black );
    this->graphLineColors[3] = new QColor( Qt::darkYellow );
    this->graphLineColors[4] = new QColor( Qt::darkGreen );

    QPalette palette = ui->frameCurve1->palette();
    palette.setColor( QPalette::Window, *this->graphLineColors[0] );
    ui->frameCurve1->setPalette(palette);

    palette.setColor( QPalette::Window, *this->graphLineColors[1] );
    ui->frameCurve2->setPalette(palette);

    palette.setColor( QPalette::Window, *this->graphLineColors[2] );
    ui->frameCurve3->setPalette(palette);

    palette.setColor( QPalette::Window, *this->graphLineColors[3] );
    ui->frameCurve4->setPalette(palette);

    palette.setColor( QPalette::Window, *this->graphLineColors[4] );
    ui->frameCurve5->setPalette(palette);

}

void MainWindow::setProperties() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::setProperties" << std::endl;
#endif

    ui->checkInExperiment2->setProperty("exp",2);
    ui->checkInExperiment3->setProperty("exp",3);
    ui->checkInExperiment4->setProperty("exp",4);
    ui->checkInExperiment5->setProperty("exp",5);

    ui->buttonCopyExp2->setProperty("exp",2);
    ui->buttonCopyExp3->setProperty("exp",3);
    ui->buttonCopyExp4->setProperty("exp",4);
    ui->buttonCopyExp5->setProperty("exp",5);

    ui->buttonDefaultExp1->setProperty("exp",1);
    ui->buttonDefaultExp2->setProperty("exp",2);
    ui->buttonDefaultExp3->setProperty("exp",3);
    ui->buttonDefaultExp4->setProperty("exp",4);
    ui->buttonDefaultExp5->setProperty("exp",5);

    ui->toolButtonColorCurve1->setProperty("curve",0); // Relativo ao vetor de cores
    ui->toolButtonColorCurve2->setProperty("curve",1);
    ui->toolButtonColorCurve3->setProperty("curve",2);
    ui->toolButtonColorCurve4->setProperty("curve",3);
    ui->toolButtonColorCurve5->setProperty("curve",4);

    QSettings settings(qApp->applicationDirPath()+"/etc/system.ini",QSettings::IniFormat);
    settings.beginGroup("Input_Buffers");
    int min = settings.value("minSize",ui->spinInInputBuffersExp1->minimum()).toInt();
    int max = settings.value("maxSize",ui->spinInInputBuffersExp1->maximum()).toInt();
    ui->spinInInputBuffersExp1->setMinimum( min );
    ui->spinInInputBuffersExp1->setMaximum( max );
    ui->spinInInputBuffersExp2->setMinimum( min );
    ui->spinInInputBuffersExp2->setMaximum( max );
    ui->spinInInputBuffersExp3->setMinimum( min );
    ui->spinInInputBuffersExp3->setMaximum( max );
    ui->spinInInputBuffersExp4->setMinimum( min );
    ui->spinInInputBuffersExp4->setMaximum( max );
    ui->spinInInputBuffersExp5->setMinimum( min );
    ui->spinInInputBuffersExp5->setMaximum( max );
    settings.endGroup();

    settings.beginGroup("Output_Buffers");
    min = settings.value("minSize",ui->spinInOutputBuffersExp1->minimum()).toInt();
    max = settings.value("maxSize",ui->spinInOutputBuffersExp1->maximum()).toInt();
    ui->spinInOutputBuffersExp1->setMinimum( min );
    ui->spinInOutputBuffersExp1->setMaximum( max );
    ui->spinInOutputBuffersExp2->setMinimum( min );
    ui->spinInOutputBuffersExp2->setMaximum( max );
    ui->spinInOutputBuffersExp3->setMinimum( min );
    ui->spinInOutputBuffersExp3->setMaximum( max );
    ui->spinInOutputBuffersExp4->setMinimum( min );
    ui->spinInOutputBuffersExp4->setMaximum( max );
    ui->spinInOutputBuffersExp5->setMinimum( min );
    ui->spinInOutputBuffersExp5->setMaximum( max );
    settings.endGroup();
}

void MainWindow::establishConnections() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::establishConnections" << std::endl;
#endif

    // System Configuration
    connect(ui->comboTopology,SIGNAL(currentIndexChanged(int)),this,SLOT(topologyChange(int)));
    connect(ui->buttonAddSystemConf,SIGNAL(clicked(bool)),this,SLOT(addSystemConfiguration()));
    connect(ui->buttonTrafficConfiguration,SIGNAL(clicked(bool)),this,SLOT(editTrafficPatterns()));
    connect(ui->buttonGenTCF,SIGNAL(clicked(bool)),this,SIGNAL(generateTCF()));

    // System Simulation
    connect(ui->checkInExperiment2,SIGNAL(stateChanged(int)),this,SLOT(experimentChangeState()));
    connect(ui->checkInExperiment3,SIGNAL(stateChanged(int)),this,SLOT(experimentChangeState()));
    connect(ui->checkInExperiment4,SIGNAL(stateChanged(int)),this,SLOT(experimentChangeState()));
    connect(ui->checkInExperiment5,SIGNAL(stateChanged(int)),this,SLOT(experimentChangeState()));

    connect(ui->buttonDefaultExp1,SIGNAL(clicked()),this,SLOT(loadDefaultValuesExperiment()));
    connect(ui->buttonDefaultExp2,SIGNAL(clicked()),this,SLOT(loadDefaultValuesExperiment()));
    connect(ui->buttonDefaultExp3,SIGNAL(clicked()),this,SLOT(loadDefaultValuesExperiment()));
    connect(ui->buttonDefaultExp4,SIGNAL(clicked()),this,SLOT(loadDefaultValuesExperiment()));
    connect(ui->buttonDefaultExp5,SIGNAL(clicked()),this,SLOT(loadDefaultValuesExperiment()));

    connect(ui->buttonCopyExp2,SIGNAL(clicked()),this,SLOT(copyPreviousExperimentActive()));
    connect(ui->buttonCopyExp3,SIGNAL(clicked()),this,SLOT(copyPreviousExperimentActive()));
    connect(ui->buttonCopyExp4,SIGNAL(clicked()),this,SLOT(copyPreviousExperimentActive()));
    connect(ui->buttonCopyExp5,SIGNAL(clicked()),this,SLOT(copyPreviousExperimentActive()));

    // Qt 4 approach
    // connect(ui->comboInTopologyExp1,SIGNAL(currentIndexChanged(int)),this,SLOT(topologyExpChange(int)));
    // Qt 5 approach - static_cast is needed because overloaded signal "currentIndexChanged(int|QString)" of combo box
    connect(ui->comboInTopologyExp1,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,&MainWindow::topologyExpChange);
    connect(ui->comboInTopologyExp2,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,&MainWindow::topologyExpChange);
    connect(ui->comboInTopologyExp3,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,&MainWindow::topologyExpChange);
    connect(ui->comboInTopologyExp4,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,&MainWindow::topologyExpChange);
    connect(ui->comboInTopologyExp5,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,&MainWindow::topologyExpChange);

    connect(ui->comboInSimulationStopMethod,SIGNAL(currentIndexChanged(int)),this,SLOT(stopOptionUpdate(int)));

    connect(ui->doubleSpinInChannelFclkRange,SIGNAL(valueChanged(double)),this,SLOT(fClkFirstUpdate(double)));
    connect(ui->doubleSpinInChannelFclkRange_2,SIGNAL(valueChanged(double)),this,SLOT(fClkLastUpdate(double)));

    connect(ui->buttonCancel,SIGNAL(clicked()),this,SIGNAL(cancel()));
    connect(ui->buttonRunSimulation,SIGNAL(clicked()),this,SLOT(run()));

    // Performance Analysis
    connect(ui->buttonRunAnalysis,SIGNAL(clicked()),this,SLOT(runAnalysis()));
    connect(ui->spinBoxInitPacketsAnalyze,SIGNAL(valueChanged(int)),this,SLOT(lowerLimitAnalysisChange(int)));
    connect(ui->spinBoxEndPacketsAnalyze,SIGNAL(valueChanged(int)),this,SLOT(upperLimitAnalysisChange(int)));
    connect(ui->comboFlowSelection,SIGNAL(currentIndexChanged(int)),this,SLOT(flowSelectionUpdated(int)));
    connect(ui->buttonCheckUncheckAll, SIGNAL(clicked()),this,SLOT(buttonCheckUncheckAllClicked()));
    connect(ui->toolButtonColorCurve1,SIGNAL(clicked()),this,SLOT(toolButtonCurveClicked()));
    connect(ui->toolButtonColorCurve2,SIGNAL(clicked()),this,SLOT(toolButtonCurveClicked()));
    connect(ui->toolButtonColorCurve3,SIGNAL(clicked()),this,SLOT(toolButtonCurveClicked()));
    connect(ui->toolButtonColorCurve4,SIGNAL(clicked()),this,SLOT(toolButtonCurveClicked()));
    connect(ui->toolButtonColorCurve5,SIGNAL(clicked()),this,SLOT(toolButtonCurveClicked()));
    connect(ui->buttonPlotWaveform,SIGNAL(clicked()),this, SIGNAL(viewWaveform()));
    connect(ui->buttonPlotGraphicSelected,SIGNAL(clicked()),this,SLOT(plotGraphic()));
    connect(ui->buttonPlotLatencyDistribution,SIGNAL(clicked()),this,SLOT(plotGraphic()));
    connect(ui->buttonViewReport,SIGNAL(clicked()),this,SLOT(report()));

    // Actions
    connect(ui->actionNew,SIGNAL(triggered()),this,SIGNAL(newSystem()));
    connect(ui->actionLoadDefault,SIGNAL(triggered()),this,SIGNAL(loadDefaultSystem()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SIGNAL(loadSystem()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SIGNAL(saveSystem()));
    connect(ui->actionSaveAs,SIGNAL(triggered()),this,SIGNAL(saveAsSystem()));
    connect(ui->actionSaveAsDefault,SIGNAL(triggered()),this,SIGNAL(saveAsDefaultSystem()));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->actionAbout_Qt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    connect(ui->actionOptions,SIGNAL(triggered()),this,SIGNAL(editOptions()));
    connect(ui->actionLoadSimulation,SIGNAL(triggered()),this,SIGNAL(loadSimulationResults()));
    connect(ui->actionSaveSimulation,SIGNAL(triggered()),this,SIGNAL(saveSimulationResults()));
    connect(ui->actionGenerateCSV,SIGNAL(triggered()),this,SLOT(generateCSVClicked()));

    connect(ui->toolButtonClearConsole,SIGNAL(clicked()),this,SLOT(initConsole()));


    // All changes that modify the configuration
    // Routing
    connect(ui->comboInRoutingAlgorithmExp1,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->comboInRoutingAlgorithmExp2,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->comboInRoutingAlgorithmExp3,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->comboInRoutingAlgorithmExp4,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->comboInRoutingAlgorithmExp5,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    // Flow Control
    connect(ui->comboInSwitchingExp1,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->comboInSwitchingExp2,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->comboInSwitchingExp3,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->comboInSwitchingExp4,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->comboInSwitchingExp5,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    // Arbiter
    connect(ui->comboInArbiterTypeExp1,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->comboInArbiterTypeExp2,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->comboInArbiterTypeExp3,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->comboInArbiterTypeExp4,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->comboInArbiterTypeExp5,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    // VC
    connect(ui->comboInVCExp1,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->comboInVCExp2,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->comboInVCExp3,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->comboInVCExp4,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->comboInVCExp5,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    // Buffers
    connect(ui->spinInInputBuffersExp1,SIGNAL(valueChanged(int)),this,SLOT(setAppModified()));
    connect(ui->spinInInputBuffersExp2,SIGNAL(valueChanged(int)),this,SLOT(setAppModified()));
    connect(ui->spinInInputBuffersExp3,SIGNAL(valueChanged(int)),this,SLOT(setAppModified()));
    connect(ui->spinInInputBuffersExp4,SIGNAL(valueChanged(int)),this,SLOT(setAppModified()));
    connect(ui->spinInInputBuffersExp5,SIGNAL(valueChanged(int)),this,SLOT(setAppModified()));
    connect(ui->spinInOutputBuffersExp1,SIGNAL(valueChanged(int)),this,SLOT(setAppModified()));
    connect(ui->spinInOutputBuffersExp2,SIGNAL(valueChanged(int)),this,SLOT(setAppModified()));
    connect(ui->spinInOutputBuffersExp3,SIGNAL(valueChanged(int)),this,SLOT(setAppModified()));
    connect(ui->spinInOutputBuffersExp4,SIGNAL(valueChanged(int)),this,SLOT(setAppModified()));
    connect(ui->spinInOutputBuffersExp5,SIGNAL(valueChanged(int)),this,SLOT(setAppModified()));
    // Others
    connect(ui->comboInSimulationStopMethod,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->spinBoxStopTimeCycles,SIGNAL(valueChanged(int)),this,SLOT(setAppModified()));
    connect(ui->spinBoxStopTimeNs,SIGNAL(valueChanged(int)),this,SLOT(setAppModified()));
    connect(ui->spinBoxStopNumberOfPacketsDelivered,SIGNAL(valueChanged(int)),this,SLOT(setAppModified()));;
    connect(ui->doubleSpinInChannelFclkRange,SIGNAL(valueChanged(double)),this,SLOT(setAppModified()));
    connect(ui->doubleSpinInChannelFclkRange_2,SIGNAL(valueChanged(double)),this,SLOT(setAppModified()));
    connect(ui->comboInStep,SIGNAL(currentIndexChanged(int)),this,SLOT(setAppModified()));
    connect(ui->doubleSpinInStep,SIGNAL(valueChanged(double)),this,SLOT(setAppModified()));
}

void MainWindow::setAppModified() {
    this->setWindowModified(true);
}

void MainWindow::configureLanguages(EnvironmentConfiguration *conf) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::configureLanguages" << std::endl;
#endif

    QStringList langs = conf->getLanguageNames();
    int size = langs.size();
    for( int i = 0; i < size; i++ ) {
        QString name = langs.at(i);
        QAction* action = new QAction(name,this);
        action->setObjectName(name);
        action->setStatusTip(trUtf8("Change environment language to %1").arg(name));
        connect(action,SIGNAL(triggered()),this,SLOT(translate()));
        ui->menuLanguage->addAction(action);
    }

    this->translate(conf->getLanguageFilesSelected());
}

/**
 * Center the main window on the system desktop
 */
void MainWindow::centerWindow() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::centerWindow" << std::endl;
#endif

    // Obtém a largura da janela principal do simulador
    int w = this->size().width(), h = this->size().height();
    int x, y;
    int screenWidth;
    int screenHeight;

    // Obtém a largura da tela
    QDesktopWidget *desktop = QApplication::desktop();
    screenWidth = desktop->width();
    screenHeight = desktop->height();

    // Calcula o posicionamento na tela do canto superior esquerdo do local onde a janela deverá ficar
    x = (screenWidth - w) / 2;
    y = (screenHeight - h) / 2;

    // Movimenta a janela para a posição indicada
    this->move( x, y );
}

void MainWindow::setAnalysisOptionsEnabled(bool enabled) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::setAnalysisOptionsEnabled" << std::endl;
#endif

    ui->buttonRunAnalysis->setEnabled(enabled);
    ui->buttonPlotGraphicSelected->setEnabled(enabled);
    ui->buttonPlotLatencyDistribution->setEnabled(enabled);
    ui->buttonPlotWaveform->setEnabled(enabled);
    ui->buttonViewReport->setEnabled(enabled);

    ui->buttonRunSimulation->setEnabled(enabled);

    ui->actionLoadSimulation->setEnabled(enabled); // Here to reduce code
}

void MainWindow::setActionSaveSimulationEnabled(bool enabled) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::setActionSaveSimulationEnabled" << std::endl;
#endif
    ui->actionSaveSimulation->setEnabled(enabled);

}

void MainWindow::setActionGenerateCSVEnabled(bool enabled) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::setActionGenerateCSVEnabled" << std::endl;
#endif
    ui->actionGenerateCSV->setEnabled(enabled);

}

void MainWindow::setOptionsSimulationEnabled(bool enabled) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::setOptionsSimulationEnabled" << std::endl;
#endif
    ui->actionLoadSimulation->setEnabled(enabled);
    ui->actionSaveSimulation->setEnabled(enabled);
    ui->actionGenerateCSV->setEnabled(enabled);

}

void MainWindow::insertComboItens() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::insertComboItens" << std::endl;
#endif

    unsigned int i; // Loop iterator
    SystemDefines* def = SystemDefines::getInstance();

    unsigned int topologyCount = def->sizeTopologies();
    for( i = 0; i < topologyCount; i++ ) {
        SystemDefines::Topology topology = def->findTopology(i);
        ui->comboInTopologyExp1->insertItem(qint32(i),topology.getTopology());
        ui->comboInTopologyExp2->insertItem(qint32(i),topology.getTopology());
        ui->comboInTopologyExp3->insertItem(qint32(i),topology.getTopology());
        ui->comboInTopologyExp4->insertItem(qint32(i),topology.getTopology());
        ui->comboInTopologyExp5->insertItem(qint32(i),topology.getTopology());
    }

    QString selectedTopologyExp1 = ui->comboInTopologyExp1->currentText();
    QString selectedTopologyExp2 = ui->comboInTopologyExp2->currentText();
    QString selectedTopologyExp3 = ui->comboInTopologyExp3->currentText();
    QString selectedTopologyExp4 = ui->comboInTopologyExp4->currentText();
    QString selectedTopologyExp5 = ui->comboInTopologyExp5->currentText();

    SystemDefines::Routing rAlg1 = def->findRoutingAlgorithm(selectedTopologyExp1);
    for( int x = 0; x < rAlg1.algorithmsCount(); x++ ) {
        QString alg = rAlg1.getRoutingAlgorithm(x);
        ui->comboInRoutingAlgorithmExp1->insertItem(qint32(x),alg);
    }

    SystemDefines::Routing rAlg2 = def->findRoutingAlgorithm(selectedTopologyExp2);
    for( int x = 0; x < rAlg2.algorithmsCount(); x++ ) {
        QString alg = rAlg2.getRoutingAlgorithm(x);
        ui->comboInRoutingAlgorithmExp2->insertItem(qint32(x),alg);
    }

    SystemDefines::Routing rAlg3 = def->findRoutingAlgorithm(selectedTopologyExp3);
    for( int x = 0; x < rAlg3.algorithmsCount(); x++ ) {
        QString alg = rAlg3.getRoutingAlgorithm(x);
        ui->comboInRoutingAlgorithmExp3->insertItem(qint32(x),alg);
    }

    SystemDefines::Routing rAlg4 = def->findRoutingAlgorithm(selectedTopologyExp4);
    for( int x = 0; x < rAlg4.algorithmsCount(); x++ ) {
        QString alg = rAlg4.getRoutingAlgorithm(x);
        ui->comboInRoutingAlgorithmExp4->insertItem(qint32(x),alg);
    }

    SystemDefines::Routing rAlg5 = def->findRoutingAlgorithm(selectedTopologyExp5);
    for( int x = 0; x < rAlg5.algorithmsCount(); x++ ) {
        QString alg = rAlg5.getRoutingAlgorithm(x);
        ui->comboInRoutingAlgorithmExp5->insertItem(qint32(x),alg);
    }

    unsigned int qtdFlowControls = def->sizeFlowControls();
    for( i = 0; i < qtdFlowControls; i++) {
        SystemDefines::FlowControl flowCtrl = def->findFlowControl(i);
        QString fCtrl = flowCtrl.getFlowControl();
        ui->comboInSwitchingExp1->insertItem(qint32(i),fCtrl);
        ui->comboInSwitchingExp2->insertItem(qint32(i),fCtrl);
        ui->comboInSwitchingExp3->insertItem(qint32(i),fCtrl);
        ui->comboInSwitchingExp4->insertItem(qint32(i),fCtrl);
        ui->comboInSwitchingExp5->insertItem(qint32(i),fCtrl);

    }

    unsigned int qtdArbiterTypes = def->sizeArbiterPGs();
    for( i = 0; i < qtdArbiterTypes; i++) {
        SystemDefines::PriorityGenerator priorityGenerator = def->findArbiterPG(i);
        QString arbType = priorityGenerator.getPG();
        ui->comboInArbiterTypeExp1->insertItem(qint32(i),arbType);
        ui->comboInArbiterTypeExp2->insertItem(qint32(i),arbType);
        ui->comboInArbiterTypeExp3->insertItem(qint32(i),arbType);
        ui->comboInArbiterTypeExp4->insertItem(qint32(i),arbType);
        ui->comboInArbiterTypeExp5->insertItem(qint32(i),arbType);
    }

    unsigned int vcOptionsSize = def->sizeVcOptions();
    for( i = 0; i < vcOptionsSize; i++) {
        QString vcOp = def->findVcOption(i);
        ui->comboInVCExp1->insertItem(qint32(i),vcOp);
        ui->comboInVCExp2->insertItem(qint32(i),vcOp);
        ui->comboInVCExp3->insertItem(qint32(i),vcOp);
        ui->comboInVCExp4->insertItem(qint32(i),vcOp);
        ui->comboInVCExp5->insertItem(qint32(i),vcOp);
    }

    QStringList stepType;
    stepType << "Inc: " << "Exp: ";
    ui->comboInStep->addItems(stepType);
}

void MainWindow::initConsole() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::initConsole" << std::endl;
#endif

    this->ui->console->clear();

    QTextCursor cursor(ui->console->textCursor());
    cursor.movePosition( QTextCursor::Start );

    QTextBlockFormat headFormat = cursor.blockFormat();
    headFormat.setAlignment(Qt::AlignHCenter);
    cursor.insertHtml(trUtf8("<b><font size=4>__/_/_/_/<font color=red> RedScarf </font>\\_\\_\\_\\__</font></b>"));
    cursor.insertHtml(trUtf8("<br /><font color=black>The <i>bagual</i> NoC Simulator</font>"));
    cursor.insertHtml(trUtf8("<br /><img src=\":/icons/icons/apresentacao.png\" />"));
    cursor.insertHtml(trUtf8("<br />All rights reserved &#169; 2014 - 2017 LEDS - University of Vale do Itajaí<br/>- Laboratory of Embedded and Distributed Systems"));
    cursor.setBlockFormat(headFormat);

    cursor.insertBlock();
    QTextBlockFormat bodyFormat = cursor.blockFormat();
    bodyFormat.setAlignment( Qt::AlignLeft );
    cursor.setBlockFormat(bodyFormat);

}

QString MainWindow::selectSystemFolder(QString msgSelect) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::selectSystemFolder" << std::endl;
#endif

    return QFileDialog::getExistingDirectory(this,msgSelect);
}

void MainWindow::clearConsole() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::clearConsole" << std::endl;
#endif

    this->ui->console->clear();
}

void MainWindow::showOpenFileError(QString error) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::openFileError" << std::endl;
#endif

    QMessageBox::information(this, trUtf8("Unable to open file"),   // Exibe mensagem de erro
        error);

}

unsigned int MainWindow::saveChanges(QString title, QString msg) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::saveChanges" << std::endl;
#endif

    QMessageBox* dialog =  new QMessageBox(title,msg,QMessageBox::Warning,QMessageBox::Save,QMessageBox::Discard,
                                        QMessageBox::Cancel,this);
    switch( dialog->exec() ) {
        case QMessageBox::Save:
            return 0;
        case QMessageBox::Discard:
            return 1;
        case QMessageBox::Cancel:
            return 2;
    }
    return 999; // Invalido
}

QString MainWindow::dialogLoadFile(QString caption, QString filter, QString currentDir) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::dialogLoadFile" << std::endl;
#endif
    return QFileDialog::getOpenFileName(this,caption, currentDir,filter);
}

QString MainWindow::dialogSaveFile(QString caption, QString filter, QString currentDir) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::dialogSaveFile" << std::endl;
#endif
    return QFileDialog::getSaveFileName(this,caption, currentDir,filter);//;All Files (*)"));
}

void MainWindow::closeEvent(QCloseEvent *event) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::closeEvent" << std::endl;
#endif
    emit exitApplication(event);
}
void MainWindow::changeEvent(QEvent *event) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::changeEvent" << std::endl;
#endif

    if(event->type() == QEvent::LanguageChange) {
        int stopOption = ui->comboInSimulationStopMethod->currentIndex();
        int vcdIndex = ui->comboInGenerateVCD->currentIndex();
        int flowSel = ui->comboFlowSelection->currentIndex();
        int xAnalysis = ui->comboBoxXAxisGraphic->currentIndex();
        int yAnalysis = ui->comboBoxYAxisGraphic->currentIndex();
        bool windowMod = this->isWindowModified();
        ui->retranslateUi(this);
        ui->comboInGenerateVCD->setCurrentIndex( vcdIndex );
        ui->comboBoxXAxisGraphic->setCurrentIndex(xAnalysis);
        ui->comboBoxYAxisGraphic->setCurrentIndex(yAnalysis);
        ui->comboFlowSelection->setCurrentIndex(flowSel);
        ui->comboInSimulationStopMethod->setCurrentIndex(stopOption);
        this->setWindowModified(windowMod);
        ui->doubleSpinBoxLineWidth->setValue( ui->doubleSpinBoxLineWidth->value() );
        ui->doubleSpinBoxPointSize->setValue( ui->doubleSpinBoxPointSize->value() );
        ui->doubleSpinInChannelBWRange->setValue( ui->doubleSpinInChannelBWRange->value() );
        ui->doubleSpinInChannelBWRange_2->setValue( ui->doubleSpinInChannelBWRange_2->value() );
        ui->doubleSpinInChannelTclkRange->setValue( ui->doubleSpinInChannelTclkRange->value() );
        ui->doubleSpinInChannelTclkRange_2->setValue( ui->doubleSpinInChannelTclkRange_2->value() );
        ui->toolBar->setWindowTitle( QApplication::translate("MainWindow", "Toolbar") );
    }

    QWidget::changeEvent(event);

}

void MainWindow::updateView(QList<SystemConfiguration> sysConfs, QList<Experiment> experiments,
                            SystemOperation sop) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::updateView" << std::endl;
#endif

    // Clear the current configuration
    ui->listConf->clear();

    for( int i = 0; i < sysConfs.size(); i++ ) {
        SystemConfiguration conf = sysConfs.at(i);
        SystemParameters sysParam = conf.getSystemParameters();
        // Create the widget item
        QListWidgetItem* item = new QListWidgetItem(sysParam.getFormattedString(),ui->listConf);
        QVariant parameters;
        parameters.setValue(sysParam);
        // Set Parameters
        item->setData(Qt::UserRole,parameters);

        QList<TrafficParameters> traffics = conf.getTrafficConfiguration();
        QList<QVariant> trafficsVar;
        for( int x = 0; x < traffics.size(); x++ ) {
            TrafficParameters tp = traffics.at(x);
            QVariant tpVar;
            tpVar.setValue(tp);
            trafficsVar.append(tpVar);
        }
        if( !trafficsVar.isEmpty() ) {
            item->setData(Qt::UserRole+1,trafficsVar);
        }
    }

    // Experiments
    for(int i = 0u; i < experiments.size(); i++) {
        const Experiment* experiment = &experiments.at(i);
        if( experiment != NULL ) {
            switch (i+1) {
                case 1:
                    ui->comboInTopologyExp1->setCurrentIndex( experiment->getTopology() );
                    ui->comboInRoutingAlgorithmExp1->setCurrentIndex( experiment->getRoutingAlgorithm() );
                    ui->comboInSwitchingExp1->setCurrentIndex( experiment->getFlowControl() );
                    ui->comboInArbiterTypeExp1->setCurrentIndex( experiment->getArbiterType() );
                    ui->comboInVCExp1->setCurrentIndex(experiment->getVCOption());
                    ui->spinInInputBuffersExp1->setValue( experiment->getInputBufferSize() );
                    ui->spinInOutputBuffersExp1->setValue( experiment->getOutputBufferSize() );
                    break;
                case 2:
                    ui->comboInTopologyExp2->setCurrentIndex( experiment->getTopology() );
                    ui->comboInRoutingAlgorithmExp2->setCurrentIndex( experiment->getRoutingAlgorithm() );
                    ui->comboInSwitchingExp2->setCurrentIndex( experiment->getFlowControl() );
                    ui->comboInArbiterTypeExp2->setCurrentIndex( experiment->getArbiterType() );
                    ui->comboInVCExp2->setCurrentIndex(experiment->getVCOption());
                    ui->spinInInputBuffersExp2->setValue( experiment->getInputBufferSize() );
                    ui->spinInOutputBuffersExp2->setValue( experiment->getOutputBufferSize() );

                    ui->checkInExperiment2->setChecked(experiment->isActive());

                    break;
                case 3:
                    ui->comboInTopologyExp3->setCurrentIndex( experiment->getTopology() );
                    ui->comboInRoutingAlgorithmExp3->setCurrentIndex( experiment->getRoutingAlgorithm() );
                    ui->comboInSwitchingExp3->setCurrentIndex( experiment->getFlowControl() );
                    ui->comboInArbiterTypeExp3->setCurrentIndex( experiment->getArbiterType() );
                    ui->comboInVCExp3->setCurrentIndex(experiment->getVCOption());
                    ui->spinInInputBuffersExp3->setValue( experiment->getInputBufferSize() );
                    ui->spinInOutputBuffersExp3->setValue( experiment->getOutputBufferSize() );

                    ui->checkInExperiment3->setChecked(experiment->isActive());

                    break;
                case 4:
                    ui->comboInTopologyExp4->setCurrentIndex( experiment->getTopology() );
                    ui->comboInRoutingAlgorithmExp4->setCurrentIndex( experiment->getRoutingAlgorithm() );
                    ui->comboInSwitchingExp4->setCurrentIndex( experiment->getFlowControl() );
                    ui->comboInArbiterTypeExp4->setCurrentIndex( experiment->getArbiterType() );
                    ui->comboInVCExp4->setCurrentIndex(experiment->getVCOption());
                    ui->spinInInputBuffersExp4->setValue( experiment->getInputBufferSize() );
                    ui->spinInOutputBuffersExp4->setValue( experiment->getOutputBufferSize() );

                    ui->checkInExperiment4->setChecked(experiment->isActive());

                    break;
                case 5:
                    ui->comboInTopologyExp5->setCurrentIndex( experiment->getTopology() );
                    ui->comboInRoutingAlgorithmExp5->setCurrentIndex( experiment->getRoutingAlgorithm() );
                    ui->comboInSwitchingExp5->setCurrentIndex( experiment->getFlowControl() );
                    ui->comboInArbiterTypeExp5->setCurrentIndex( experiment->getArbiterType() );
                    ui->comboInVCExp5->setCurrentIndex(experiment->getVCOption());
                    ui->spinInInputBuffersExp5->setValue( experiment->getInputBufferSize() );
                    ui->spinInOutputBuffersExp5->setValue( experiment->getOutputBufferSize() );

                    ui->checkInExperiment5->setChecked(experiment->isActive());

                    break;
            }
        }
    }


    // System operation
    ui->spinBoxStopTimeCycles->setValue( sop.stopTime_cycles );
    ui->spinBoxStopTimeNs->setValue( sop.stopTime_ns );
    ui->spinBoxStopNumberOfPacketsDelivered->setValue( sop.stopNumPackets );
    ui->comboInSimulationStopMethod->setCurrentIndex( sop.stopOption );

    ui->comboInGenerateVCD->setCurrentIndex( sop.vcdOption );
    ui->doubleSpinInChannelFclkRange->setValue(sop.fClkFirst);
    ui->doubleSpinInChannelFclkRange_2->setValue(sop.fClkLast);
    ui->comboInStep->setCurrentIndex(sop.fClkStepType);
    ui->doubleSpinInStep->setValue(sop.fClkStep);

}

void MainWindow::enableRun() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::enableRun" << std::endl;
#endif

    this->ui->progressBar->setVisible(false);
    this->ui->buttonRunSimulation->setEnabled(true);
    this->ui->buttonCancel->setEnabled( false );
    this->setAnalysisOptionsEnabled(true);
}

void MainWindow::setLimitsProgressBar(int minimum, int maximum) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::setLimitsProgressBar" << std::endl;
#endif
    this->ui->progressBar->setRange(minimum,maximum);
    this->ui->progressBar->setValue(minimum);

}

void MainWindow::updateProgressBar(int value) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::updateProgressBar" << std::endl;
#endif

    this->ui->progressBar->setValue(value);

}

AnalysisOptions* MainWindow::getAnalysisOptions(TypeAnalysis opcaoAnalise) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::getAnalysisOptions" << std::endl;
#endif

    // Legend parameters
    bool topology    = ui->checkBoxTopology->isChecked();
    bool routingAlg  = ui->checkBoxRoutingAlgorithm->isChecked();
    bool flowControl = ui->checkBoxFlowControl->isChecked();
    bool arbiterType = ui->checkBoxArbiterType->isChecked();
    bool vcOp        = ui->checkBoxVcOption->isChecked();
    bool inBuffers   = ui->checkBoxInputBuffersDepth->isChecked();
    bool outBuffers  = ui->checkBoxOutputBuffersDepth->isChecked();

    float lineWidth = ui->doubleSpinBoxLineWidth->value();
    float pointSize = ui->doubleSpinBoxPointSize->value();

    unsigned short source = quint16(ui->spinAnalysisSource->value());
    unsigned short destination = quint16(ui->spinAnalysisDestination->value());

    QString xLabel;
    QString yLabel;
    QString title;
    AnalysisOptions::FlowOptions flowOps =
            static_cast<AnalysisOptions::FlowOptions>(ui->comboFlowSelection->currentIndex());

    int xAxis = ui->comboBoxXAxisGraphic->currentIndex();
    int yAxis = ui->comboBoxYAxisGraphic->currentIndex();

    bool latencyDistribution = false;
    if( opcaoAnalise == LatencyDistribution ) {
        xLabel = trUtf8("Latency (cycles)");
        yLabel = trUtf8("Frequency");
        title = trUtf8("Latency Distribution Histogram");
        latencyDistribution = true;
    } else if( opcaoAnalise == GraphicSelected ) {
        xLabel = ui->comboBoxXAxisGraphic->currentText();
        yLabel = ui->comboBoxYAxisGraphic->currentText();
        if( xLabel.contains('(') ) {

        }
        int ix = xLabel.indexOf('(');
        int iy = yLabel.indexOf('(');

        title = QString( "%1 x %2" )
                .arg((iy == -1 ? yLabel : yLabel.left(iy-1)))
                .arg((ix == -1 ? xLabel : xLabel.left(ix-1)));
    } else if(opcaoAnalise == Report) {
        title = trUtf8("Report");
    }
    AnalysisOptions* gop = new AnalysisOptions(topology,routingAlg,flowControl,
            arbiterType,vcOp,inBuffers,outBuffers,lineWidth,pointSize,source,destination,
            xLabel,yLabel,xAxis,yAxis,title,flowOps,
            graphLineColors,latencyDistribution);

    return gop;
}

SystemConfiguration MainWindow::getConfiguration(int index) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::getConfiguration" << std::endl;
#endif
    if( index < ui->listConf->count() ) {
        QListWidgetItem* item = ui->listConf->item(index);
        if( item != NULL ) {
            QVariant sysParameterVar = item->data(Qt::UserRole);
            SystemParameters sysParameter = sysParameterVar.value<SystemParameters>();
            QList<QVariant> trafficParameterListVar = item->data(Qt::UserRole+1).toList();
            QList<TrafficParameters> trafficList;
            for( int i = 0; i < trafficParameterListVar.size(); i++) {
                QVariant trafficVar = trafficParameterListVar.at(i);
                TrafficParameters trafficConf = trafficVar.value<TrafficParameters>();
                if( trafficConf.isValidForSystem( sysParameter.getNumberElements() ) ) {
                    trafficList.append(trafficConf);
                }
            }
            return SystemConfiguration(sysParameter,trafficList);
        }
    }

    return SystemConfiguration(); // Return an invalid conf - verify with isValid()

}

SystemConfiguration MainWindow::getSelectedConfiguration() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::getSelectedConfiguration" << std::endl;
#endif
    return this->getConfiguration( ui->listConf->currentRow() );
//    QList<QListWidgetItem *> confSelected = ui->listConf->selectedItems();

//    if(confSelected.size()) {
//        if(confSelected.size() > 1) {
//        }
//    }

}


QList<SystemConfiguration> MainWindow::getAllConfiguration() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::getAllConfiguration" << std::endl;
#endif
    QList<SystemConfiguration> configurations;
    for( int i = 0; i < ui->listConf->count(); i++ ) {
        SystemConfiguration sysConf = this->getConfiguration(i);
        if( sysConf.isValid() ) { // && sysConf.hasTraffic() ) {
            configurations.append(sysConf);
        }
    }
    return configurations;

}

QList<float> MainWindow::getOperationFrequencies() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::getOperationFrequencies" << std::endl;
#endif
    float firstFClk = ui->doubleSpinInChannelFclkRange->value();
    float lastFClk = ui->doubleSpinInChannelFclkRange_2->value();
    int stepType = ui->comboInStep->currentIndex();
    float step = ui->doubleSpinInStep->value();

    float fClk = firstFClk;
    float previousFClk = fClk;

    unsigned int numExecutions = 0;

    if(step == 0) {
        numExecutions = 1u;
    } else {
        switch(stepType) {
            case 0: { // INC
                numExecutions = (unsigned int) (std::abs(lastFClk-firstFClk)) / std::abs(step) + 1;
                if((firstFClk+step*numExecutions) != lastFClk ){
                    if((firstFClk+step*(numExecutions-1)) !=  lastFClk) {
                        numExecutions++;
                    }
                }
                break;
            }
            case 1: { // EXP
                int i = 0;
                numExecutions = 1;
                unsigned int tmpInt;
                if(firstFClk > lastFClk) {
                    while(fClk >= lastFClk) {
                        previousFClk = fClk;
                        tmpInt = (unsigned int) (firstFClk/(exp(i*step))*10.f);
                        fClk = ((float) tmpInt)/10.f;
                        if((fClk >= lastFClk) && (fClk != previousFClk) ){
                            numExecutions++;
                        }
                        i++;
                    }
                } else if(firstFClk < lastFClk) {
                    while(fClk <= lastFClk) {
                        previousFClk = fClk;
                        tmpInt = (unsigned int) (firstFClk/(exp(i*step))*10.f);
                        fClk = ((float) tmpInt)/10.f;
                        if((fClk <= lastFClk) && (fClk != previousFClk)){
                            numExecutions++;
                        }
                        i++;
                    }
                }
                break;
            }
        }

    }

    QList<float> freqs;

    fClk = firstFClk;
    previousFClk = fClk;
    /*
     * Calculating
     */
    int aux = 0;
    for(unsigned int i = 0; i < numExecutions; i++) {
        freqs.append(fClk);
        if( stepType == 0 ) { // Increment (or decrement)
            fClk += step;
            if( (firstFClk > lastFClk )
                    && (fClk < lastFClk) ) {
                fClk = lastFClk;
            }
            if( (firstFClk < lastFClk)
                    && (fClk > lastFClk) ) {
                fClk = lastFClk;
            }
        } else { // Exponential
            do {
                previousFClk = fClk;
                int tmp = (unsigned int) (firstFClk / ( exp(aux*step) )*10.f );
                fClk = ((float) tmp) / 10.0f;
                aux++;
            } while (fClk == previousFClk);
        }
    }
    return freqs;
}

QList<Experiment> MainWindow::getAllExperiments() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::getActiveExperiments" << std::endl;
#endif

    QList<Experiment> experiments;

    for( int i = 0; i < MAX_EXPERIMENTS; i++ ) {
        Experiment e;
        bool active = false;
        QComboBox* topology = NULL;
        QComboBox* routing = NULL;
        QComboBox* flowControl = NULL;
        QComboBox* arbiter = NULL;
        QComboBox* vcOption = NULL;
        QSpinBox* inBuffers = NULL;
        QSpinBox* outBuffers = NULL;
        switch (i+1) {
            case 1: {
                active = true;
                topology = ui->comboInTopologyExp1;
                routing = ui->comboInRoutingAlgorithmExp1;
                flowControl = ui->comboInSwitchingExp1;
                arbiter = ui->comboInArbiterTypeExp1;
                vcOption = ui->comboInVCExp1;
                inBuffers = ui->spinInInputBuffersExp1;
                outBuffers = ui->spinInOutputBuffersExp1;
                break;
            }
            case 2: {
                active = ui->checkInExperiment2->isChecked();
                topology = ui->comboInTopologyExp2;
                routing = ui->comboInRoutingAlgorithmExp2;
                flowControl = ui->comboInSwitchingExp2;
                arbiter = ui->comboInArbiterTypeExp2;
                vcOption = ui->comboInVCExp2;
                inBuffers = ui->spinInInputBuffersExp2;
                outBuffers = ui->spinInOutputBuffersExp2;
                break;
            }
            case 3: {
                active = ui->checkInExperiment3->isChecked();
                topology = ui->comboInTopologyExp3;
                routing = ui->comboInRoutingAlgorithmExp3;
                flowControl = ui->comboInSwitchingExp3;
                arbiter = ui->comboInArbiterTypeExp3;
                vcOption = ui->comboInVCExp3;
                inBuffers = ui->spinInInputBuffersExp3;
                outBuffers = ui->spinInOutputBuffersExp3;
                break;
            }
            case 4: {
                active = ui->checkInExperiment4->isChecked();
                topology = ui->comboInTopologyExp4;
                routing = ui->comboInRoutingAlgorithmExp4;
                flowControl = ui->comboInSwitchingExp4;
                arbiter = ui->comboInArbiterTypeExp4;
                vcOption = ui->comboInVCExp4;
                inBuffers = ui->spinInInputBuffersExp4;
                outBuffers = ui->spinInOutputBuffersExp4;
                break;
            }
            case 5: {
                active = ui->checkInExperiment5->isChecked();
                topology = ui->comboInTopologyExp5;
                routing = ui->comboInRoutingAlgorithmExp5;
                flowControl = ui->comboInSwitchingExp5;
                arbiter = ui->comboInArbiterTypeExp5;
                vcOption = ui->comboInVCExp5;
                inBuffers = ui->spinInInputBuffersExp5;
                outBuffers = ui->spinInOutputBuffersExp5;
                break;
            }
            default:
                break;
        }
        e.setActive(active);
        e.setArbiterType(arbiter->currentIndex());
        e.setFlowControl(flowControl->currentIndex());
        e.setInputBufferSize(inBuffers->value());
        e.setOutputBufferSize(outBuffers->value());
        e.setRoutingAlgorithm(routing->currentIndex());
        e.setTopology(topology->currentIndex());
        e.setVCOption(vcOption->currentIndex());
        experiments.append(e);
    }
    return experiments;
}

SystemOperation MainWindow::getSystemOperation() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::getSystemOperation" << std::endl;
#endif
    SystemOperation sysOp;

    sysOp.fClkFirst = ui->doubleSpinInChannelFclkRange->value();
    sysOp.fClkLast = ui->doubleSpinInChannelFclkRange_2->value();
    sysOp.fClkStep = ui->doubleSpinInStep->value();
    sysOp.fClkStepType = ui->comboInStep->currentIndex();
    sysOp.stopOption = ui->comboInSimulationStopMethod->currentIndex();
    sysOp.stopTime_cycles = ui->spinBoxStopTimeCycles->value();
    sysOp.stopNumPackets = ui->spinBoxStopNumberOfPacketsDelivered->value();
    sysOp.stopTime_ns = ui->spinBoxStopTimeNs->value();
    sysOp.vcdOption = ui->comboInGenerateVCD->currentIndex();

    return sysOp;
}

/////////////////// Slots ///////////////////

void MainWindow::toolButtonCurveClicked() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::toolButtonCurveClicked" << std::endl;
#endif

    QToolButton* toolButton = (QToolButton *) sender();
    int index = toolButton->property("curve").toInt();

    QColorDialog* colorDial = new QColorDialog( *this->graphLineColors[index], this );

    if(  colorDial->exec() == QColorDialog::Accepted ) {
        QColor currentColor = colorDial->currentColor();
        QPalette palette = toolButton->palette();
        palette.setColor( QPalette::Window, currentColor );
        this->graphLineColors[index]->setRgb( currentColor.rgb() );
        switch( index ) {
            case 0:
                ui->frameCurve1->setPalette( palette );
                break;
            case 1:
                ui->frameCurve2->setPalette( palette );
                break;
            case 2:
                ui->frameCurve3->setPalette( palette );
                break;
            case 3:
                ui->frameCurve4->setPalette( palette );
                break;
            case 4:
                ui->frameCurve5->setPalette( palette );
                break;
        }    }

}

void MainWindow::buttonCheckUncheckAllClicked() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::buttonCheckUncheckAllClicked" << std::endl;
#endif
    const int nCHECKS = 7;
    QCheckBox* checks[nCHECKS];
    checks[0] = ui->checkBoxTopology;
    checks[1] = ui->checkBoxRoutingAlgorithm;
    checks[2] = ui->checkBoxFlowControl;
    checks[3] = ui->checkBoxArbiterType;
    checks[4] = ui->checkBoxVcOption;
    checks[5] = ui->checkBoxInputBuffersDepth;
    checks[6] = ui->checkBoxOutputBuffersDepth;

    bool checkeds = false;
    for( int i = 0; i < nCHECKS; i++ ) {
        if( checks[i]->isChecked() ) {
            checkeds = true;
            break;
        }
    }

    if( checkeds ) {
        for( int i = 0; i < nCHECKS; i++ ) {
            checks[i]->setChecked( false );
        }
    } else {
        for( int i = 0; i < nCHECKS; i++ ) {
            checks[i]->setChecked( true );
        }
    }

}

void MainWindow::flowSelectionUpdated(int current) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::flowSelectionUpdated" << std::endl;
#endif

    if( current == ui->comboFlowSelection->count()-1 ) {
        ui->labelAnalysisSource->setEnabled( true );
        ui->labelAnalysisDestination->setEnabled(true);
        ui->spinAnalysisSource->setEnabled(true);
        ui->spinAnalysisDestination->setEnabled(true);
        ui->labelFlowParameters->setEnabled(true);
    } else {
        ui->labelAnalysisSource->setEnabled( false );
        ui->labelAnalysisDestination->setEnabled(false);
        ui->spinAnalysisSource->setEnabled(false);
        ui->spinAnalysisDestination->setEnabled(false);
        ui->labelFlowParameters->setEnabled(false);
    }

}

void MainWindow::translate() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::translate" << std::endl;
#endif

    QString filename = sender()->objectName();

    emit changeLanguage(filename);
}

void MainWindow::translate(QStringList filenames) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::translate(QStringList filenames)" << std::endl;
#endif

    if( filenames.isEmpty() ) {
        return;
    }

    for( int i = 0; i < translators.size(); i++ ) {
        QTranslator* translator = translators.at(i);
        qApp->removeTranslator( translator );
        delete translator;
    }
    translators.clear();

    QString dirTranslations = QString(qApp->applicationDirPath()+"/translations");

    for( int i = 0; i < filenames.size(); i++ ) {
        QString filename = filenames.at(i);
        QTranslator* translator = new QTranslator(this);
        translator->load( filename,dirTranslations );
        qApp->installTranslator( translator );
        translators.append( translator );
    }

    if( filenames.contains("qt_pt") ) {
        this->setLocale( QLocale(QLocale::Portuguese,QLocale::Brazil) );
    } else {
        this->setLocale( QLocale(QLocale::English,QLocale::UnitedStates) );
    }

}

void MainWindow::printConsole(QString msg, QColor color, Qt::Alignment alignment) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::printConsole" << std::endl;
#endif

    QTextCursor cursor(ui->console->textCursor());
    cursor.movePosition( QTextCursor::End );

    QTextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.setAlignment(alignment);
    cursor.setBlockFormat(blockFormat);
    cursor.insertHtml( QString("<font color=%1>%2</font>").arg(color.name()).arg(msg) );
    cursor.insertBlock();

    cursor.movePosition( QTextCursor::End );
    ui->console->setTextCursor(cursor);

}

void MainWindow::experimentChangeState() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::stateChangedExperiment" << std::endl;
#endif

    QCheckBox* check = (QCheckBox* ) sender();

    int numExp = check->property("exp").toInt();
    bool checked = check->isChecked();

    QGridLayout* layout = (QGridLayout* ) ui->groupBoxExperimentsConfiguration->layout();

    for(int i = 1; i < 10; i++) {
        QLayoutItem* item = layout->itemAtPosition(i,numExp);
        if(item == NULL) {
            this->printConsole(trUtf8("Problem in layout of experiment modified!\n"
                                      "Contact the development team to fix."));
        } else {
            item->widget()->setEnabled(checked);
        }
    }

    switch( numExp ) {
        case 2:
            ui->labelColorCurve2->setEnabled(checked);
            ui->frameCurve2->setEnabled(checked);
            ui->toolButtonColorCurve2->setEnabled(checked);
            break;
        case 3:
            ui->labelColorCurve3->setEnabled(checked);
            ui->frameCurve3->setEnabled(checked);
            ui->toolButtonColorCurve3->setEnabled(checked);
            break;
        case 4:
            ui->labelColorCurve4->setEnabled(checked);
            ui->frameCurve4->setEnabled(checked);
            ui->toolButtonColorCurve4->setEnabled(checked);
            break;
        case 5:
            ui->labelColorCurve5->setEnabled(checked);
            ui->frameCurve5->setEnabled(checked);
            ui->toolButtonColorCurve5->setEnabled(checked);
            break;
    }
    this->setAppModified();
}

void MainWindow::topologyExpChange(int newValue) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::topologyExpChange" << std::endl;
#endif

    QComboBox* combo = (QComboBox* ) sender();
    QString objName = combo->objectName();
    int experimentIndex = objName.at( objName.size()-1 ).digitValue();

    // After change topology, update the routing algorithms to new topology
    SystemDefines* def = SystemDefines::getInstance();

    // Selecting the current experiment combo
    QComboBox* routingCombo = NULL;
    switch(experimentIndex) {
        case 1:
            routingCombo = ui->comboInRoutingAlgorithmExp1;
            break;
        case 2:
            routingCombo = ui->comboInRoutingAlgorithmExp2;
            break;
        case 3:
            routingCombo = ui->comboInRoutingAlgorithmExp3;
            break;
        case 4:
            routingCombo = ui->comboInRoutingAlgorithmExp4;
            break;
        case 5:
            routingCombo = ui->comboInRoutingAlgorithmExp5;
            break;
    }

    routingCombo->clear();

    SystemDefines::Routing routing = def->findRoutingAlgorithm( def->findTopology(newValue).getTopology() );
    for( int x = 0; x < routing.algorithmsCount(); x++ ) {
        QString alg = routing.getRoutingAlgorithm(x);
        routingCombo->insertItem(qint32(x),alg);
    }
    this->setAppModified();

}

void MainWindow::loadDefaultValuesExperiment() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::loadDefaultValuesExperiment" << std::endl;
#endif
    QPushButton* botao = (QPushButton* ) sender();

    int numExp = botao->property("exp").toInt();

    switch(numExp) {
        case 1:
            ui->comboInTopologyExp1->setCurrentIndex( qint32(DefaultValuesSystem::DEFAULT_TOPOLOGY) );
            ui->comboInRoutingAlgorithmExp1->setCurrentIndex    (qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE     ));
            ui->comboInSwitchingExp1->setCurrentIndex           (qint32(DefaultValuesSystem::DEFAULT_FC_TYPE          ));
            ui->comboInArbiterTypeExp1->setCurrentIndex         (qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE     ));
            ui->comboInVCExp1->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_VC_OPTION      ));
            ui->spinInInputBuffersExp1->setValue                (qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH    ));
            ui->spinInOutputBuffersExp1->setValue               (qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH   ));
            break;
        case 2:
            ui->comboInTopologyExp2->setCurrentIndex( qint32(DefaultValuesSystem::DEFAULT_TOPOLOGY) );
            ui->comboInVCExp2->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_VC_OPTION      ));
            ui->comboInRoutingAlgorithmExp2->setCurrentIndex    (qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE     ));
            ui->comboInSwitchingExp2->setCurrentIndex           (qint32(DefaultValuesSystem::DEFAULT_FC_TYPE          ));
            ui->comboInArbiterTypeExp2->setCurrentIndex         (qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE     ));
            ui->spinInInputBuffersExp2->setValue                (qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH    ));
            ui->spinInOutputBuffersExp2->setValue               (qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH   ));
            break;
        case 3:
            ui->comboInTopologyExp3->setCurrentIndex( qint32(DefaultValuesSystem::DEFAULT_TOPOLOGY) );
            ui->comboInVCExp3->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_VC_OPTION      ));
            ui->comboInRoutingAlgorithmExp3->setCurrentIndex    (qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE     ));
            ui->comboInSwitchingExp3->setCurrentIndex           (qint32(DefaultValuesSystem::DEFAULT_FC_TYPE          ));
            ui->comboInArbiterTypeExp3->setCurrentIndex         (qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE     ));
            ui->spinInInputBuffersExp3->setValue                (qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH    ));
            ui->spinInOutputBuffersExp3->setValue               (qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH   ));
            break;
        case 4:
            ui->comboInTopologyExp4->setCurrentIndex( qint32(DefaultValuesSystem::DEFAULT_TOPOLOGY) );
            ui->comboInVCExp4->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_VC_OPTION      ));
            ui->comboInRoutingAlgorithmExp4->setCurrentIndex    (qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE     ));
            ui->comboInSwitchingExp4->setCurrentIndex           (qint32(DefaultValuesSystem::DEFAULT_FC_TYPE          ));
            ui->comboInArbiterTypeExp4->setCurrentIndex         (qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE     ));
            ui->spinInInputBuffersExp4->setValue                (qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH    ));
            ui->spinInOutputBuffersExp4->setValue               (qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH   ));
            break;
        case 5:
            ui->comboInTopologyExp5->setCurrentIndex( qint32(DefaultValuesSystem::DEFAULT_TOPOLOGY) );
            ui->comboInVCExp5->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_VC_OPTION      ));
            ui->comboInRoutingAlgorithmExp5->setCurrentIndex    (qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE     ));
            ui->comboInSwitchingExp5->setCurrentIndex           (qint32(DefaultValuesSystem::DEFAULT_FC_TYPE          ));
            ui->comboInArbiterTypeExp5->setCurrentIndex         (qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE     ));
            ui->spinInInputBuffersExp5->setValue                (qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH    ));
            ui->spinInOutputBuffersExp5->setValue               (qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH   ));
            break;
    }
    this->setAppModified();
}

void MainWindow::copyPreviousExperimentActive() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::copyPreviousExperimentActive" << std::endl;
#endif

    QPushButton* botao = (QPushButton* ) sender();

    int numExp = botao->property("exp").toInt();

    QGridLayout* layout = (QGridLayout* ) ui->groupBoxExperimentsConfiguration->layout();

    int previousExperiment;
    for( previousExperiment = numExp - 1 ; !layout->itemAtPosition(1,previousExperiment)->widget()->isEnabled() ; previousExperiment-- );

    QComboBox* topologySrc = (QComboBox *) layout->itemAtPosition(1,previousExperiment)->widget();
    QComboBox* topologyDst = (QComboBox *) layout->itemAtPosition(1,numExp)->widget();
    topologyDst->setCurrentIndex( topologySrc->currentIndex() );

    QComboBox* routingAlgorithmSrc = (QComboBox *) layout->itemAtPosition(2,previousExperiment)->widget();
    QComboBox* routingAlgorithmDst = (QComboBox *) layout->itemAtPosition(2,numExp)->widget();
    routingAlgorithmDst->setCurrentIndex( routingAlgorithmSrc->currentIndex() );

    QComboBox* flowControlSrc = (QComboBox *) layout->itemAtPosition(3,previousExperiment)->widget();
    QComboBox* flowControlDst = (QComboBox *) layout->itemAtPosition(3,numExp)->widget();
    flowControlDst->setCurrentIndex( flowControlSrc->currentIndex() );

    QComboBox* arbiterTypeSrc = (QComboBox *) layout->itemAtPosition(4,previousExperiment)->widget();
    QComboBox* arbiterTypeDst = (QComboBox *) layout->itemAtPosition(4,numExp)->widget();
    arbiterTypeDst->setCurrentIndex( arbiterTypeSrc->currentIndex() );

    QComboBox* vcOptionSrc = (QComboBox *) layout->itemAtPosition(5,previousExperiment)->widget();
    QComboBox* vcOptionDst = (QComboBox *) layout->itemAtPosition(5,numExp)->widget();
    vcOptionDst->setCurrentIndex( vcOptionSrc->currentIndex() );

    QSpinBox* inputBufferSrc = (QSpinBox *) layout->itemAtPosition(6,previousExperiment)->widget();
    QSpinBox* inputBufferDst = (QSpinBox *) layout->itemAtPosition(6,numExp)->widget();
    inputBufferDst->setValue( inputBufferSrc->value() );

    QSpinBox* outputBufferSrc = (QSpinBox *) layout->itemAtPosition(7,previousExperiment)->widget();
    QSpinBox* outputBufferDst = (QSpinBox *) layout->itemAtPosition(7,numExp)->widget();
    outputBufferDst->setValue( outputBufferSrc->value() );

    this->setAppModified();
}

void MainWindow::stopOptionUpdate(int pos) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::stopOptionUpdated" << std::endl;
#endif

    switch( pos ) {
        case 1: // Stop by time (ns)
            this->ui->spinBoxStopTimeCycles->setVisible(false);
            this->ui->spinBoxStopTimeNs->setVisible(true);
            this->ui->spinBoxStopNumberOfPacketsDelivered->setVisible(false);
            break;
        case 2: // Stop by cycles
            this->ui->spinBoxStopTimeCycles->setVisible(true);
            this->ui->spinBoxStopTimeNs->setVisible(false);
            this->ui->spinBoxStopNumberOfPacketsDelivered->setVisible(false);
            break;
        case 3: // Stop by number of packets delivered
            this->ui->spinBoxStopTimeCycles->setVisible(false);
            this->ui->spinBoxStopTimeNs->setVisible(false);
            this->ui->spinBoxStopNumberOfPacketsDelivered->setVisible(true);
            break;
        default: // Stop when all packets are delivered
            this->ui->spinBoxStopTimeCycles->setVisible(false);
            this->ui->spinBoxStopTimeNs->setVisible(false);
            this->ui->spinBoxStopNumberOfPacketsDelivered->setVisible(false);
    }
}

void MainWindow::fClkFirstUpdate(double value) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::fClkFirstUpdated" << std::endl;
#endif
    ui->doubleSpinInChannelTclkRange->setValue( (1.0 / value ) * 1000.0);
    ui->doubleSpinInChannelBWRange->setValue( value * ui->inDataWidth->value() );
}

void MainWindow::fClkLastUpdate(double value) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::fClkLastUpdated" << std::endl;
#endif
    ui->doubleSpinInChannelTclkRange_2->setValue( (1.0 / value) * 1000.0 );
    ui->doubleSpinInChannelBWRange_2->setValue( value * ui->inDataWidth->value() );
}

void MainWindow::about() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::about" << std::endl;
#endif

    AboutWindow* about = new AboutWindow(this);

    about->exec();

    delete about;
}

void MainWindow::run() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::run" << std::endl;
#endif

    this->ui->progressBar->setVisible(true);
    this->ui->buttonRunSimulation->setEnabled(false);
    this->ui->buttonCancel->setEnabled(true);
    this->setAnalysisOptionsEnabled(false);

    emit this->runSimulation();
}

void MainWindow::runAnalysis() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::runAnalysis" << std::endl;
#endif

    float lower = ui->spinBoxInitPacketsAnalyze->value();
    lower /= 100.0;
    float upper = ui->spinBoxEndPacketsAnalyze->value();
    upper /= 100.0;

    emit this->generateAnalysis(lower,upper,0);
}

void MainWindow::plotGraphic() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::plotGraphic" << std::endl;
#endif

    AnalysisOptions* aop = NULL;
    if( sender() == ui->buttonPlotLatencyDistribution ) {
        aop = this->getAnalysisOptions( LatencyDistribution );
    } else {
        aop = this->getAnalysisOptions( GraphicSelected );
    }

    emit this->viewGraphic(aop);

}

void MainWindow::report() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::report" << std::endl;
#endif

    emit viewReport( this->getAnalysisOptions( Report ) );

}

void MainWindow::generateCSVClicked() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::generateCSVClicked" << std::endl;
#endif

    emit this->generateCSVSimulationReport( this->getAnalysisOptions( GraphicSelected ) );

}

////////////// NEW SIMULATOR //////////////

void MainWindow::topologyChange(int newTop) {

    switch (newTop) {
        case 0: // Non-Orthogonal
        ui->widget2dOrtho->setVisible(false);
        ui->widget3dOrtho->setVisible(false);
            ui->widgetNonOrtho->setVisible(true);
            break;
        case 1: // 2D-Orthogonal
            ui->widgetNonOrtho->setVisible(false);
            ui->widget3dOrtho->setVisible(false);
            ui->widget2dOrtho->setVisible(true);
            break;
        case 2: // 3D-Orthogonal
            ui->widgetNonOrtho->setVisible(false);
            ui->widget2dOrtho->setVisible(false);
            ui->widget3dOrtho->setVisible(true);
            break;
        default:break;
    }

}

void MainWindow::addSystemConfiguration() {

    SystemParameters sp;
    sp.setDataWidth( quint16(ui->inDataWidth->value()) );
    switch( ui->comboTopology->currentIndex() ) {
        case 0: // Non-Orthogonal
            sp.setTopologyType(SystemParameters::NonOrthogonal);
            sp.setNumberElements( quint16(ui->inNElements->value()) );
            break;
        case 1: // 2D-Orthogonal
            sp.setTopologyType(SystemParameters::Orthogonal2D);
            sp.setXSize( quint16(ui->in2dXSize->value()) );
            sp.setYSize( quint16(ui->in2dYSize->value()) );
            break;
        case 2: // 3D-Orthogonal
            sp.setTopologyType(SystemParameters::Orthogonal3D);
            sp.setXSize( quint16(ui->in3dXSize->value()) );
            sp.setYSize( quint16(ui->in3dYSize->value()) );
            sp.setZSize( quint16(ui->in3dZSize->value()) );
            break;
        default:break;
    }

    if( this->alreadyExists(&sp) ) {
        QMessageBox::information(this,tr("Configuration already exists"),
                                 tr("Duplicated configuration... not added"));
    } else {
        QListWidgetItem* item = new QListWidgetItem(sp.getFormattedString(),ui->listConf);
        QVariant parameters;
        parameters.setValue(sp);
        item->setData(Qt::UserRole,parameters);
        item->setSelected(true);
        this->setAppModified();
    }

}

bool MainWindow::alreadyExists(SystemParameters *sp) {

    int listSize = ui->listConf->count();
    for(int i = 0; i < listSize; i++) {
        QListWidgetItem* it = ui->listConf->item(i);
        QVariant parameter = it->data(Qt::UserRole);
        SystemParameters listSp = parameter.value<SystemParameters>();
        if( sp->equal(listSp) ) {
            return true;
        }
    }
    return false;

}

void MainWindow::removeSelectedItems() {

    QList<QListWidgetItem *> items = this->ui->listConf->selectedItems();

    if( items.size() != 0 ) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Confirm remove"),
                                      tr("Are you sure to remove the selected items?"),
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            for(int i = 0; i < items.size(); i++) {
                QListWidgetItem* item = items.at(i);
                delete item;
            }
            this->setWindowModified(true);
        }
    }
}

void MainWindow::editTrafficPatterns() {

    QList<QListWidgetItem *> items = this->ui->listConf->selectedItems();

    if(items.size() == 0) {
        QMessageBox::information(this,tr("Message"),tr("None configuration selected."),QMessageBox::Ok);
    } else {
        QString differentNodeCount = tr("Different number of elements on the selected configurations!");
        bool someDifferent = false;
        for( int i = 0; i < items.size() - 1; i++ ) {
            QListWidgetItem* item1 = items.at(i);
            QVariant data1 = item1->data(Qt::UserRole);
            SystemParameters sp1 = data1.value<SystemParameters>();
            for( int x =  i+1; x < items.size(); x++) {
                QListWidgetItem* item2 = items.at(x);
                QVariant data2 = item2->data(Qt::UserRole);
                SystemParameters sp2 = data2.value<SystemParameters>();
                if(sp1.getNumberElements() != sp2.getNumberElements()) {
                    differentNodeCount += tr("\nConfig.: %1 has %2 elements and Config.: %3 has %4 elements")
                            .arg(i+1)
                            .arg(sp1.getNumberElements())
                            .arg(x+1)
                            .arg(sp2.getNumberElements());
                    someDifferent = true;
                }
            }
        }
        if( someDifferent ) {
            QMessageBox::information(this,tr("ATTENTION on the traffic setup!"),differentNodeCount,
                                     QMessageBox::Ok);
        }
        TrafficConfigurationDialog* tcw = new TrafficConfigurationDialog(this,items);
        connect(tcw,SIGNAL(trafficConfigured(QList<QVariant>)),this,SLOT(setupTraffic(QList<QVariant>)));
        tcw->show();
    }
}

void MainWindow::setupTraffic(QList<QVariant> configuration) {

    QList<QListWidgetItem *> systemSelected = ui->listConf->selectedItems();

    for( int i = 0; i < systemSelected.size(); i++ ) {
        QListWidgetItem* sysConf = systemSelected.at(i);
        sysConf->setData(Qt::UserRole+1,configuration);
    }
    this->setAppModified();
}

void MainWindow::lowerLimitAnalysisChange(int newValue) {
    ui->spinBoxEndPacketsAnalyze->setMinimum(newValue+1);
}

void MainWindow::upperLimitAnalysisChange(int newValue) {
    ui->spinBoxInitPacketsAnalyze->setMaximum(newValue-1);
}
