/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* MainWindow.cpp
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

#include "ui_MainWindow.h"
#include "include/View/MainWindow.h"

#include "include/View/AboutWindow.h"
#include "include/View/AnalysisOptions.h"

#include "include/Model/System/Defines.h"
#include "include/Model/System/SystemDefines.h"
#include "include/Model/System/ExperimentManager.h"

#include "include/Control/EnvironmentConfiguration.h"

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

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class View/MainWindow" << std::endl;
#endif
    ui->setupUi(this); // Configura o uso do formulário da tela

//    this->centralizarTela(); // Desativado - o gerenciador de janelas do SO posiciona no local mais adequado
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
    // Desabilitando botoes "Edit" da configuração dos padrões de tráfico
    ui->buttonEditTrafficPattern0->setDisabled(true);
    ui->buttonEditTrafficPattern1->setDisabled(true);
    ui->buttonEditTrafficPattern2->setDisabled(true);
    ui->buttonEditTrafficPattern3->setDisabled(true);

    // Desabilitando widgets dos experimentos (pois por default são habilitados na inicialização)
    QGridLayout* layout = (QGridLayout* ) ui->groupBoxExperimentsConfiguration->layout();
    for(int i = 1; i < 9; i++) {
        for(int x = 2; x < 6; x++) {
            layout->itemAtPosition(i,x)->widget()->setEnabled(false);
        }
    }

    // Desabilitando o spin de stop time da simulação, por padrão a opção de parada é até entregar todos os pacotes
    ui->spinBoxStopTimeNs->setVisible(false);
    ui->spinBoxStopTimeCycles->setVisible(false);

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
    ui->toolBar->addAction( ui->actionClearAll );
    ui->toolBar->addSeparator();
    ui->toolBar->addAction( ui->actionPreview_Traffic_Configuration );
    ui->toolBar->addAction( ui->actionXML_View_Traffic_Configuration );
    ui->toolBar->addSeparator();
    ui->toolBar->addAction( ui->actionLoadSimulation );
    ui->toolBar->addAction( ui->actionSaveSimulation );
    ui->toolBar->addAction( ui->actionGenerateCSV );
}

void MainWindow::loadDefaultValues() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::loadDefaultValues" << std::endl;
#endif

    ui->spinInXSize->setValue(qint32(DefaultValuesSystem::DEFAULT_X_SIZE));
    ui->spinInYSize->setValue(qint32(DefaultValuesSystem::DEFAULT_Y_SIZE));
    ui->spinInChannelWidth->setValue(qint32(DefaultValuesSystem::DEFAULT_DATA_WIDTH));

    ui->spinBoxStopTimeNs->setValue( qint32(DefaultValuesSystem::DEFAULT_STOP_TIME_NS) );
    ui->spinBoxStopTimeCycles->setValue( qint32(DefaultValuesSystem::DEFAULT_STOP_TIME_CYCLES) );
    ui->comboInSimulationStopMethod->setCurrentIndex( qint32(DefaultValuesSystem::DEFAULT_STOP_OPTION) );

    ui->comboInVCDGenerationOptions->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_VCD_OPTION));
    ui->doubleSpinInChannelFclkRange->setValue(qint32(DefaultValuesSystem::DEFAULT_FCLK_FIRST));
    ui->doubleSpinInChannelFclkRange_2->setValue(qint32(DefaultValuesSystem::DEFAULT_FCLK_LAST));
    ui->comboInStep->setCurrentIndex(qint32(DefaultValuesSystem::DEFAULT_FCLK_STEP_TYPE));
    ui->doubleSpinInStep->setValue(qint32(DefaultValuesSystem::DEFAULT_FCLK_STEP_VALUE));
    ui->doubleSpinInChannelTclkRange->setValue(qint32(DefaultValuesSystem::DEFAULT_TCLK));
    ui->doubleSpinInChannelTclkRange_2->setValue(qint32(DefaultValuesSystem::DEFAULT_TCLK));
    ui->doubleSpinInChannelBWRange->setValue(qint32(DefaultValuesSystem::DEFAULT_CHANNEL_BW));
    ui->doubleSpinInChannelBWRange_2->setValue(qint32(DefaultValuesSystem::DEFAULT_CHANNEL_BW));

    ui->comboInRouterArchitectureExp1->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_ROUTER_TYPE      ));
    ui->comboInRoutingAlgorithmExp1->setCurrentIndex    (qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE     ));
    ui->comboInSwitchingExp1->setCurrentIndex           (qint32(DefaultValuesSystem::DEFAULT_FC_TYPE          ));
    ui->comboInArbiterTypeExp1->setCurrentIndex         (qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE     ));
    ui->spinInInputBuffersExp1->setValue                (qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH    ));
    ui->spinInOutputBuffersExp1->setValue               (qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH   ));

    ui->comboInRouterArchitectureExp2->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_ROUTER_TYPE      ));
    ui->comboInRoutingAlgorithmExp2->setCurrentIndex    (qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE     ));
    ui->comboInSwitchingExp2->setCurrentIndex           (qint32(DefaultValuesSystem::DEFAULT_FC_TYPE          ));
    ui->comboInArbiterTypeExp2->setCurrentIndex         (qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE     ));
    ui->spinInInputBuffersExp2->setValue                (qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH    ));
    ui->spinInOutputBuffersExp2->setValue               (qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH   ));

    ui->comboInRouterArchitectureExp3->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_ROUTER_TYPE      ));
    ui->comboInRoutingAlgorithmExp3->setCurrentIndex    (qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE     ));
    ui->comboInSwitchingExp3->setCurrentIndex           (qint32(DefaultValuesSystem::DEFAULT_FC_TYPE          ));
    ui->comboInArbiterTypeExp3->setCurrentIndex         (qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE     ));
    ui->spinInInputBuffersExp3->setValue                (qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH    ));
    ui->spinInOutputBuffersExp3->setValue               (qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH   ));

    ui->comboInRouterArchitectureExp4->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_ROUTER_TYPE      ));
    ui->comboInRoutingAlgorithmExp4->setCurrentIndex    (qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE     ));
    ui->comboInSwitchingExp4->setCurrentIndex           (qint32(DefaultValuesSystem::DEFAULT_FC_TYPE          ));
    ui->comboInArbiterTypeExp4->setCurrentIndex         (qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE     ));
    ui->spinInInputBuffersExp4->setValue                (qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH    ));
    ui->spinInOutputBuffersExp4->setValue               (qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH   ));

    ui->comboInRouterArchitectureExp5->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_ROUTER_TYPE      ));
    ui->comboInRoutingAlgorithmExp5->setCurrentIndex    (qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE     ));
    ui->comboInSwitchingExp5->setCurrentIndex           (qint32(DefaultValuesSystem::DEFAULT_FC_TYPE          ));
    ui->comboInArbiterTypeExp5->setCurrentIndex         (qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE     ));
    ui->spinInInputBuffersExp5->setValue                (qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH    ));
    ui->spinInOutputBuffersExp5->setValue               (qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH   ));

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

    ui->checkInTrafficPattern0->setProperty("num",0);
    ui->checkInTrafficPattern1->setProperty("num",1);
    ui->checkInTrafficPattern2->setProperty("num",2);
    ui->checkInTrafficPattern3->setProperty("num",3);

    ui->buttonEditTrafficPattern0->setProperty("num",0);
    ui->buttonEditTrafficPattern1->setProperty("num",1);
    ui->buttonEditTrafficPattern2->setProperty("num",2);
    ui->buttonEditTrafficPattern3->setProperty("num",3);

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

    ui->buttonPreviewDefault->setProperty("preview",1);
    ui->buttonXmlViewer->setProperty("preview",2);

    ui->actionPreview_Traffic_Configuration->setProperty("preview",1);
    ui->actionXML_View_Traffic_Configuration->setProperty("preview",2);

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
    connect(ui->spinInXSize,SIGNAL(valueChanged(int)),this,SLOT(sizeUpdate()));
    connect(ui->spinInYSize,SIGNAL(valueChanged(int)),this,SLOT(sizeUpdate()));
    connect(ui->spinInChannelWidth,SIGNAL(valueChanged(int)),this,SLOT(channelWidthUpdate(int)));

    connect(ui->checkInTrafficPattern0,SIGNAL(stateChanged(int)),this,SLOT(trafficPatternStateChanged(int)));
    connect(ui->checkInTrafficPattern1,SIGNAL(stateChanged(int)),this,SLOT(trafficPatternStateChanged(int)));
    connect(ui->checkInTrafficPattern2,SIGNAL(stateChanged(int)),this,SLOT(trafficPatternStateChanged(int)));
    connect(ui->checkInTrafficPattern3,SIGNAL(stateChanged(int)),this,SLOT(trafficPatternStateChanged(int)));

    connect(ui->buttonEditTrafficPattern0,SIGNAL(clicked()),this,SLOT(editClicked()));
    connect(ui->buttonEditTrafficPattern1,SIGNAL(clicked()),this,SLOT(editClicked()));
    connect(ui->buttonEditTrafficPattern2,SIGNAL(clicked()),this,SLOT(editClicked()));
    connect(ui->buttonEditTrafficPattern3,SIGNAL(clicked()),this,SLOT(editClicked()));

    connect(ui->spinInSrcNodeX,SIGNAL(valueChanged(int)),this,SLOT(srcNodeUpdated()));
    connect(ui->spinInSrcNodeY,SIGNAL(valueChanged(int)),this,SLOT(srcNodeUpdated()));

    connect(this,SIGNAL(nodeSelected(uint,uint)),this,SLOT(nodeSelectedUpdated(uint,uint)));

    connect(ui->buttonPreviewDefault,SIGNAL(clicked()),this,SLOT(previewTrafficConfigurationClicked()));
    connect(ui->buttonXmlViewer,SIGNAL(clicked()),this,SLOT(previewTrafficConfigurationClicked()));

    connect(ui->actionPreview_Traffic_Configuration,SIGNAL(triggered()),this,SLOT(previewTrafficConfigurationClicked()));
    connect(ui->actionXML_View_Traffic_Configuration,SIGNAL(triggered()),this,SLOT(previewTrafficConfigurationClicked()));

    // System Simulation
    connect(ui->checkInExperiment2,SIGNAL(stateChanged(int)),this,SLOT(stateChangedExperiment()));
    connect(ui->checkInExperiment3,SIGNAL(stateChanged(int)),this,SLOT(stateChangedExperiment()));
    connect(ui->checkInExperiment4,SIGNAL(stateChanged(int)),this,SLOT(stateChangedExperiment()));
    connect(ui->checkInExperiment5,SIGNAL(stateChanged(int)),this,SLOT(stateChangedExperiment()));

    connect(ui->buttonDefaultExp1,SIGNAL(clicked()),this,SLOT(loadDefaultValuesExperiment()));
    connect(ui->buttonDefaultExp2,SIGNAL(clicked()),this,SLOT(loadDefaultValuesExperiment()));
    connect(ui->buttonDefaultExp3,SIGNAL(clicked()),this,SLOT(loadDefaultValuesExperiment()));
    connect(ui->buttonDefaultExp4,SIGNAL(clicked()),this,SLOT(loadDefaultValuesExperiment()));
    connect(ui->buttonDefaultExp5,SIGNAL(clicked()),this,SLOT(loadDefaultValuesExperiment()));

    connect(ui->buttonCopyExp2,SIGNAL(clicked()),this,SLOT(copyPreviousExperimentActive()));
    connect(ui->buttonCopyExp3,SIGNAL(clicked()),this,SLOT(copyPreviousExperimentActive()));
    connect(ui->buttonCopyExp4,SIGNAL(clicked()),this,SLOT(copyPreviousExperimentActive()));
    connect(ui->buttonCopyExp5,SIGNAL(clicked()),this,SLOT(copyPreviousExperimentActive()));

    connect(ui->comboInRouterArchitectureExp1,SIGNAL(currentIndexChanged(int)),this,SLOT(routerArchitectureChanged(int)));
    connect(ui->comboInRouterArchitectureExp2,SIGNAL(currentIndexChanged(int)),this,SLOT(routerArchitectureChanged(int)));
    connect(ui->comboInRouterArchitectureExp3,SIGNAL(currentIndexChanged(int)),this,SLOT(routerArchitectureChanged(int)));
    connect(ui->comboInRouterArchitectureExp4,SIGNAL(currentIndexChanged(int)),this,SLOT(routerArchitectureChanged(int)));
    connect(ui->comboInRouterArchitectureExp5,SIGNAL(currentIndexChanged(int)),this,SLOT(routerArchitectureChanged(int)));

    connect(ui->comboInRoutingAlgorithmExp1,SIGNAL(currentIndexChanged(int)),this,SLOT(routingAlgorithmChanged(int)));
    connect(ui->comboInRoutingAlgorithmExp2,SIGNAL(currentIndexChanged(int)),this,SLOT(routingAlgorithmChanged(int)));
    connect(ui->comboInRoutingAlgorithmExp3,SIGNAL(currentIndexChanged(int)),this,SLOT(routingAlgorithmChanged(int)));
    connect(ui->comboInRoutingAlgorithmExp4,SIGNAL(currentIndexChanged(int)),this,SLOT(routingAlgorithmChanged(int)));
    connect(ui->comboInRoutingAlgorithmExp5,SIGNAL(currentIndexChanged(int)),this,SLOT(routingAlgorithmChanged(int)));

    connect(ui->comboInSwitchingExp1,SIGNAL(currentIndexChanged(int)),this,SLOT(flowControlChanged(int)));
    connect(ui->comboInSwitchingExp2,SIGNAL(currentIndexChanged(int)),this,SLOT(flowControlChanged(int)));
    connect(ui->comboInSwitchingExp3,SIGNAL(currentIndexChanged(int)),this,SLOT(flowControlChanged(int)));
    connect(ui->comboInSwitchingExp4,SIGNAL(currentIndexChanged(int)),this,SLOT(flowControlChanged(int)));
    connect(ui->comboInSwitchingExp5,SIGNAL(currentIndexChanged(int)),this,SLOT(flowControlChanged(int)));

    connect(ui->comboInArbiterTypeExp1,SIGNAL(currentIndexChanged(int)),this,SLOT(arbiterTypeChanged(int)));
    connect(ui->comboInArbiterTypeExp2,SIGNAL(currentIndexChanged(int)),this,SLOT(arbiterTypeChanged(int)));
    connect(ui->comboInArbiterTypeExp3,SIGNAL(currentIndexChanged(int)),this,SLOT(arbiterTypeChanged(int)));
    connect(ui->comboInArbiterTypeExp4,SIGNAL(currentIndexChanged(int)),this,SLOT(arbiterTypeChanged(int)));
    connect(ui->comboInArbiterTypeExp5,SIGNAL(currentIndexChanged(int)),this,SLOT(arbiterTypeChanged(int)));

    connect(ui->spinInInputBuffersExp1,SIGNAL(valueChanged(int)),this,SLOT(inputBuffersChanged(int)));
    connect(ui->spinInInputBuffersExp2,SIGNAL(valueChanged(int)),this,SLOT(inputBuffersChanged(int)));
    connect(ui->spinInInputBuffersExp3,SIGNAL(valueChanged(int)),this,SLOT(inputBuffersChanged(int)));
    connect(ui->spinInInputBuffersExp4,SIGNAL(valueChanged(int)),this,SLOT(inputBuffersChanged(int)));
    connect(ui->spinInInputBuffersExp5,SIGNAL(valueChanged(int)),this,SLOT(inputBuffersChanged(int)));

    connect(ui->spinInOutputBuffersExp1,SIGNAL(valueChanged(int)),this,SLOT(outputBuffersChanged(int)));
    connect(ui->spinInOutputBuffersExp2,SIGNAL(valueChanged(int)),this,SLOT(outputBuffersChanged(int)));
    connect(ui->spinInOutputBuffersExp3,SIGNAL(valueChanged(int)),this,SLOT(outputBuffersChanged(int)));
    connect(ui->spinInOutputBuffersExp4,SIGNAL(valueChanged(int)),this,SLOT(outputBuffersChanged(int)));
    connect(ui->spinInOutputBuffersExp5,SIGNAL(valueChanged(int)),this,SLOT(outputBuffersChanged(int)));

    connect(ui->comboInSimulationStopMethod,SIGNAL(currentIndexChanged(int)),this,SLOT(stopOptionUpdated(int)));
    connect(ui->spinBoxStopTimeNs,SIGNAL(valueChanged(int)),this,SIGNAL(stopTimeNsChanged(int)));
    connect(ui->spinBoxStopTimeCycles,SIGNAL(valueChanged(int)),this,SIGNAL(stopTimeCyclesChanged(int)));
    connect(ui->comboInVCDGenerationOptions,SIGNAL(currentIndexChanged(int)),this,SLOT(vcdOptionUpdated(int)));

    connect(ui->doubleSpinInChannelFclkRange,SIGNAL(valueChanged(double)),this,SLOT(fClkFirstUpdated(double)));
    connect(ui->doubleSpinInChannelFclkRange_2,SIGNAL(valueChanged(double)),this,SLOT(fClkLastUpdated(double)));
    connect(ui->doubleSpinInStep,SIGNAL(valueChanged(double)),this,SLOT(fClkStepUpdated(double)));
    connect(ui->comboInStep,SIGNAL(currentIndexChanged(int)),this,SLOT(fClkStepTypeUpdated(int)));

    connect(ui->buttonCancel,SIGNAL(clicked()),this,SIGNAL(cancel()));
    connect(ui->buttonRunSimulation,SIGNAL(clicked()),this,SLOT(run()));

    // Performance Analysis
    connect(ui->spinBoxInitPacketsAnalyze,SIGNAL(valueChanged(int)),this,SLOT(lowerAnalysisValueChanged(int)));
    connect(ui->spinBoxEndPacketsAnalyze,SIGNAL(valueChanged(int)),this,SLOT(upperAnalysisValueChanged(int)));
    connect(ui->buttonRunAnalysis,SIGNAL(clicked()),this,SLOT(runAnalysis()));
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
    connect(ui->actionClearAll,SIGNAL(triggered()),this,SIGNAL(clearSystem()));
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
 * Centralizar a janela principal em relação a tela do sistema
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

    SystemDefines* def = SystemDefines::getInstance();
    unsigned int qtdRouterArchitectures = def->sizeRouterArchitectures();
    for(unsigned int i = 0; i < qtdRouterArchitectures; i++) {
        std::string routerArc = def->findRouterArchitectures(i);
        QString rArc = QString::fromStdString(routerArc);
        ui->comboInRouterArchitectureExp1->insertItem(qint32(i),rArc);
        ui->comboInRouterArchitectureExp2->insertItem(qint32(i),rArc);
        ui->comboInRouterArchitectureExp3->insertItem(qint32(i),rArc);
        ui->comboInRouterArchitectureExp4->insertItem(qint32(i),rArc);
        ui->comboInRouterArchitectureExp5->insertItem(qint32(i),rArc);
    }

    unsigned int qtdRoutingAlgorithms = def->sizeRoutingAlgorithms();
    for(unsigned int i = 0; i < qtdRoutingAlgorithms; i++) {
        std::string routingAlg = def->findRoutingAlgorithms(i);
        QString rAlg = QString::fromStdString(routingAlg);
        ui->comboInRoutingAlgorithmExp1->insertItem(qint32(i),rAlg);
        ui->comboInRoutingAlgorithmExp2->insertItem(qint32(i),rAlg);
        ui->comboInRoutingAlgorithmExp3->insertItem(qint32(i),rAlg);
        ui->comboInRoutingAlgorithmExp4->insertItem(qint32(i),rAlg);
        ui->comboInRoutingAlgorithmExp5->insertItem(qint32(i),rAlg);
    }

    unsigned int qtdFlowControls = def->sizeFlowControls();
    for(unsigned int i = 0; i < qtdFlowControls; i++) {
        std::string flowCtrl = def->findFlowControls(i);
        QString fCtrl = QString::fromStdString(flowCtrl);
        ui->comboInSwitchingExp1->insertItem(qint32(i),fCtrl);
        ui->comboInSwitchingExp2->insertItem(qint32(i),fCtrl);
        ui->comboInSwitchingExp3->insertItem(qint32(i),fCtrl);
        ui->comboInSwitchingExp4->insertItem(qint32(i),fCtrl);
        ui->comboInSwitchingExp5->insertItem(qint32(i),fCtrl);

    }

    unsigned int qtdArbiterTypes = def->sizeArbiterTypes();
    for(unsigned int i = 0; i < qtdArbiterTypes; i++) {
        std::string arbiterType = def->findArbiterTypes(i);
        QString arbType = QString::fromStdString(arbiterType);
        ui->comboInArbiterTypeExp1->insertItem(qint32(i),arbType);
        ui->comboInArbiterTypeExp2->insertItem(qint32(i),arbType);
        ui->comboInArbiterTypeExp3->insertItem(qint32(i),arbType);
        ui->comboInArbiterTypeExp4->insertItem(qint32(i),arbType);
        ui->comboInArbiterTypeExp5->insertItem(qint32(i),arbType);
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
    cursor.insertHtml(trUtf8("<br />All rights reserved &#169; 2014 LEDS - University of Vale do Itajaí<br/>- Laboratory of Embedded and Distributed Systems"));
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

void MainWindow::openFileError(QString error) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::openFileError" << std::endl;
#endif

    QMessageBox::information(this, trUtf8("Unable to open file"),   // Exibe mensagem de erro
        error);

}

void MainWindow::clearGridNodes() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::clearGridNodes" << std::endl;
#endif

    QGridLayout* grid = (QGridLayout* ) ui->nodes->layout();

    QLayoutItem* item;
    while( (item = grid->takeAt(0)) != 0 ) {
        delete item->widget();
        delete item;
    }

}

void MainWindow::setPatternState(unsigned int patternNum, bool state) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::setPatternState" << std::endl;
#endif

    switch(patternNum) {
        case 0:
            ui->checkInTrafficPattern0->setChecked(state);
            ui->buttonEditTrafficPattern0->setEnabled(state);
            break;
        case 1:
            ui->checkInTrafficPattern1->setChecked(state);
            ui->buttonEditTrafficPattern1->setEnabled(state);
            break;
        case 2:
            ui->checkInTrafficPattern2->setChecked(state);
            ui->buttonEditTrafficPattern2->setEnabled(state);
            break;
        case 3:
            ui->checkInTrafficPattern3->setChecked(state);
            ui->buttonEditTrafficPattern3->setEnabled(state);
            break;
    }

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
        int vcdIndex = ui->comboInVCDGenerationOptions->currentIndex();
        int flowSel = ui->comboFlowSelection->currentIndex();
        int xAnalysis = ui->comboBoxXAxisGraphic->currentIndex();
        int yAnalysis = ui->comboBoxYAxisGraphic->currentIndex();
        bool windowMod = this->isWindowModified();
        ui->retranslateUi(this);
        ui->comboInVCDGenerationOptions->setCurrentIndex( vcdIndex );
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

void MainWindow::updateView(unsigned int xSize, unsigned int ySize,
        unsigned int channelWidth, ExperimentManager *gpe,
        int stopOption, int stopTime_ns, int stopTime_cycles,
        int vcdOption, float fClkFirst, float fClkLast, int fClkStepType, float fClkStep) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::updateView" << std::endl;
#endif

    ui->spinInXSize->setValue( qint32(xSize) );
    ui->spinInYSize->setValue( qint32(ySize) );
    ui->spinInChannelWidth->setValue( qint32(channelWidth) );

    ui->spinInSrcNodeX->setValue(0);
    ui->spinInSrcNodeY->setValue(0);

    ui->spinBoxStopTimeCycles->setValue( stopTime_cycles );
    ui->spinBoxStopTimeNs->setValue( stopTime_ns );
    ui->comboInSimulationStopMethod->setCurrentIndex( stopOption );

    ui->comboInVCDGenerationOptions->setCurrentIndex( vcdOption );
    ui->doubleSpinInChannelFclkRange->setValue(fClkFirst);
    ui->doubleSpinInChannelFclkRange_2->setValue(fClkLast);
    ui->comboInStep->setCurrentIndex(fClkStepType);
    ui->doubleSpinInStep->setValue(fClkStep);

    // Update experiments
    for(unsigned int i = 1u; i <= 5u; i++) {
        Experiment* experiment = gpe->getExperiment(i);
        if( experiment != NULL ) {

            switch (i) {
                case 1:
                    ui->comboInRouterArchitectureExp1->setCurrentIndex(experiment->getRouterArchitecture());
                    ui->comboInRoutingAlgorithmExp1->setCurrentIndex( experiment->getRoutingAlgorithm() );
                    ui->comboInSwitchingExp1->setCurrentIndex( experiment->getFlowControl() );
                    ui->comboInArbiterTypeExp1->setCurrentIndex( experiment->getArbiterType() );
                    ui->spinInInputBuffersExp1->setValue( experiment->getInputBufferSize() );
                    ui->spinInOutputBuffersExp1->setValue( experiment->getOutputBufferSize() );
                    break;
                case 2:

                    ui->comboInRouterArchitectureExp2->setCurrentIndex(experiment->getRouterArchitecture());
                    ui->comboInRoutingAlgorithmExp2->setCurrentIndex( experiment->getRoutingAlgorithm() );
                    ui->comboInSwitchingExp2->setCurrentIndex( experiment->getFlowControl() );
                    ui->comboInArbiterTypeExp2->setCurrentIndex( experiment->getArbiterType() );
                    ui->spinInInputBuffersExp2->setValue( experiment->getInputBufferSize() );
                    ui->spinInOutputBuffersExp2->setValue( experiment->getOutputBufferSize() );

                    ui->checkInExperiment2->setChecked(experiment->isActive());

                    break;
                case 3:
                    ui->comboInRouterArchitectureExp3->setCurrentIndex(experiment->getRouterArchitecture());
                    ui->comboInRoutingAlgorithmExp3->setCurrentIndex( experiment->getRoutingAlgorithm() );
                    ui->comboInSwitchingExp3->setCurrentIndex( experiment->getFlowControl() );
                    ui->comboInArbiterTypeExp3->setCurrentIndex( experiment->getArbiterType() );
                    ui->spinInInputBuffersExp3->setValue( experiment->getInputBufferSize() );
                    ui->spinInOutputBuffersExp3->setValue( experiment->getOutputBufferSize() );

                    ui->checkInExperiment3->setChecked(experiment->isActive());

                    break;
                case 4:
                    ui->comboInRouterArchitectureExp4->setCurrentIndex(experiment->getRouterArchitecture());
                    ui->comboInRoutingAlgorithmExp4->setCurrentIndex( experiment->getRoutingAlgorithm() );
                    ui->comboInSwitchingExp4->setCurrentIndex( experiment->getFlowControl() );
                    ui->comboInArbiterTypeExp4->setCurrentIndex( experiment->getArbiterType() );
                    ui->spinInInputBuffersExp4->setValue( experiment->getInputBufferSize() );
                    ui->spinInOutputBuffersExp4->setValue( experiment->getOutputBufferSize() );

                    ui->checkInExperiment4->setChecked(experiment->isActive());

                    break;
                case 5:
                    ui->comboInRouterArchitectureExp5->setCurrentIndex(experiment->getRouterArchitecture());
                    ui->comboInRoutingAlgorithmExp5->setCurrentIndex( experiment->getRoutingAlgorithm() );
                    ui->comboInSwitchingExp5->setCurrentIndex( experiment->getFlowControl() );
                    ui->comboInArbiterTypeExp5->setCurrentIndex( experiment->getArbiterType() );
                    ui->spinInInputBuffersExp5->setValue( experiment->getInputBufferSize() );
                    ui->spinInOutputBuffersExp5->setValue( experiment->getOutputBufferSize() );

                    ui->checkInExperiment5->setChecked(experiment->isActive());

                    break;
            }
        }
    }

    emit this->nodeSelected(0,0);
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
    bool routerArch  = ui->checkBoxRouterArchitecture->isChecked();
    bool routingAlg  = ui->checkBoxRoutingAlgorithm->isChecked();
    bool flowControl = ui->checkBoxFlowControl->isChecked();
    bool arbiterType = ui->checkBoxArbiterType->isChecked();
    bool inBuffers   = ui->checkBoxInputBuffersDepth->isChecked();
    bool outBuffers  = ui->checkBoxOutputBuffersDepth->isChecked();

    float lineWidth = ui->doubleSpinBoxLineWidth->value();
    float pointSize = ui->doubleSpinBoxPointSize->value();

    int xSrc = ui->spinBoxXSourceAnalyze->value();
    int ySrc = ui->spinBoxYSourceAnalyze->value();
    int xDest = ui->spinBoxXDestinationAnalyze->value();
    int yDest = ui->spinBoxYDestinationAnalyze->value();
    int trfPtr = ui->spinBoxTrafficPatternAnalyze->value();

    QString xLabel;
    QString yLabel;
    QString title;
    AnalysisOptions::FlowOptions flowOps = static_cast<AnalysisOptions::FlowOptions>(ui->comboFlowSelection->currentIndex());

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
    AnalysisOptions* gop = new AnalysisOptions(routerArch,routingAlg,flowControl,arbiterType,inBuffers,
            outBuffers,lineWidth,pointSize,xSrc,ySrc,xDest,yDest,trfPtr,xLabel,
            yLabel,xAxis,yAxis,title,flowOps,graphLineColors,latencyDistribution);

    return gop;
}


/////////////////// Slots ///////////////////

void MainWindow::toolButtonCurveClicked() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::tooButtonCurveClicked" << std::endl;
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
    QCheckBox* checks[6];
    checks[0] = ui->checkBoxRouterArchitecture;
    checks[1] = ui->checkBoxRoutingAlgorithm;
    checks[2] = ui->checkBoxFlowControl;
    checks[3] = ui->checkBoxArbiterType;
    checks[4] = ui->checkBoxInputBuffersDepth;
    checks[5] = ui->checkBoxOutputBuffersDepth;

    bool checkeds = false;
    for( int i = 0; i < 6; i++ ) {
        if( checks[i]->isChecked() ) {
            checkeds = true;
            break;
        }
    }

    if( checkeds ) {
        for( int i = 0; i < 6; i++ ) {
            checks[i]->setChecked( false );
        }
    } else {
        for( int i = 0; i < 6; i++ ) {
            checks[i]->setChecked( true );
        }
    }

}

void MainWindow::flowSelectionUpdated(int current) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::flowSelectionUpdated" << std::endl;
#endif

    if( current == ui->comboFlowSelection->count()-1 ) {
        ui->labelXSource->setEnabled( true );
        ui->labelXDestination->setEnabled(true);
        ui->labelYSource->setEnabled(true);
        ui->labelYDestination->setEnabled(true);
        ui->labelTrafficPatternAnalyze->setEnabled(true);
        ui->spinBoxXDestinationAnalyze->setEnabled(true);
        ui->spinBoxXSourceAnalyze->setEnabled(true);
        ui->spinBoxYDestinationAnalyze->setEnabled(true);
        ui->spinBoxYSourceAnalyze->setEnabled(true);
        ui->spinBoxTrafficPatternAnalyze->setEnabled(true);
        ui->labelFlowParameters->setEnabled(true);
    } else {
        ui->labelXSource->setEnabled( false );
        ui->labelXDestination->setEnabled(false);
        ui->labelYSource->setEnabled(false);
        ui->labelYDestination->setEnabled(false);
        ui->labelTrafficPatternAnalyze->setEnabled(false);
        ui->spinBoxXDestinationAnalyze->setEnabled(false);
        ui->spinBoxXSourceAnalyze->setEnabled(false);
        ui->spinBoxYDestinationAnalyze->setEnabled(false);
        ui->spinBoxYSourceAnalyze->setEnabled(false);
        ui->spinBoxTrafficPatternAnalyze->setEnabled(false);
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

}

void MainWindow::sizeUpdate() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::sizeUpdate" << std::endl;
#endif

    unsigned int xSize = quint32(ui->spinInXSize->value());
    unsigned int ySize = quint32(ui->spinInYSize->value());

    this->clearGridNodes();

    QGridLayout* grid = (QGridLayout* ) ui->nodes->layout();
    for(unsigned int x = 0; x < xSize; x++) {
        for(unsigned int y = 0; y < ySize; y++) {
            unsigned int yMatrix = ySize - 1 -y;
            QPushButton* botao = new QPushButton(QString("%1,%2").arg(QString::number( x )).arg(QString::number( yMatrix )));
            botao->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
            botao->setMinimumWidth(32);
            botao->setCheckable(true);
            botao->setObjectName(QString("Button%1%2").arg(x).arg(yMatrix));
            QFont fonte = QFont();
            if( xSize > ySize ) {
                fonte.setPointSize( 22 - xSize );
            } else {
                fonte.setPointSize( 22 - ySize );
            }
            botao->setFont(fonte);
            botao->setStatusTip(QString("X: %1 | Y: %2").arg(QString::number(x)).arg(QString::number(yMatrix)));
            connect(botao,SIGNAL(clicked()),this,SLOT(nodeClicked()));
            grid->addWidget(botao,y,x);
        }
    }

    ui->spinInSrcNodeX->setMaximum(xSize - 1);
    ui->spinInSrcNodeY->setMaximum(ySize - 1);

    ui->spinBoxXDestinationAnalyze->setMaximum( xSize - 1 );
    ui->spinBoxXSourceAnalyze->setMaximum( xSize - 1 );
    ui->spinBoxYDestinationAnalyze->setMaximum( ySize - 1 );
    ui->spinBoxYSourceAnalyze->setMaximum( ySize - 1 );

    emit nodeSelected( ui->spinInSrcNodeX->value(),ui->spinInSrcNodeY->value() );
    // Send to control
    emit this->sizeUpdated(xSize,ySize);
}

void MainWindow::nodeSelectedUpdated(unsigned int posX, unsigned int posY) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::nodeSelectedUpdated" << std::endl;
#endif

    unsigned int xSize = quint32(ui->spinInXSize->value());
    unsigned int ySize = quint32(ui->spinInYSize->value());

    QGridLayout* grid = (QGridLayout* ) ui->nodes->layout();
    for( unsigned int x = 0; x < xSize; x++) {
        for( unsigned int y = 0; y < ySize; y++ ) {
            QLayoutItem* item = grid->itemAtPosition( y , x );
            QPushButton* nodeButton = qobject_cast<QPushButton *>(item->widget());
            if( x == posX && (ySize - 1 - y) == posY ) {
                nodeButton->setChecked(true);
            } else {
                nodeButton->setChecked(false);
            }
        }
    }


}

void MainWindow::nodeClicked() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::nodeClicked" << std::endl;
#endif

    QPushButton* botao = (QPushButton* ) sender();
    QString text = botao->text();
    QStringList list = text.split(',');
    unsigned int xSrc = quint32(list.at(0).toInt());
    unsigned int ySrc = quint32(list.at(1).toInt());


    ui->spinInSrcNodeX->setValue(xSrc);
    ui->spinInSrcNodeY->setValue(ySrc);

    emit this->nodeSelected(xSrc,ySrc);

}

void MainWindow::channelWidthUpdate(int chWidth) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::channelWidthUpdate" << std::endl;
#endif

    this->ui->doubleSpinInChannelBWRange->setValue((float) ui->doubleSpinInChannelFclkRange->value() * chWidth);
    this->ui->doubleSpinInChannelBWRange_2->setValue( ui->doubleSpinInChannelFclkRange_2->value() * chWidth );
    emit this->channelWidthUpdated(quint32(chWidth));

}

void MainWindow::srcNodeUpdated() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::srcNodeUpdated" << std::endl;
#endif

    emit this->nodeSelected( quint32(ui->spinInSrcNodeX->value()) , quint32(ui->spinInSrcNodeY->value()) );

}

void MainWindow::trafficPatternStateChanged(int s) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::trafficPatternStateChanged" << std::endl;
#endif

    QCheckBox* check = (QCheckBox* ) sender();

    unsigned int num =  check->property("num").toUInt();

    bool state = s == 0 ? false : true;

    switch(num) {
        case 0:
            this->ui->buttonEditTrafficPattern0->setEnabled(state);
            break;
        case 1:
            this->ui->buttonEditTrafficPattern1->setEnabled(state);
            break;
        case 2:
            this->ui->buttonEditTrafficPattern2->setEnabled(state);
            break;
        case 3:
            this->ui->buttonEditTrafficPattern3->setEnabled(state);
            break;
    }

    emit this->trafficPatternUpdate( quint32(ui->spinInSrcNodeX->value()), quint32(ui->spinInSrcNodeY->value()), num, state );

}

void MainWindow::editClicked() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::editClicked" << std::endl;
#endif

    QPushButton* botao = (QPushButton* ) sender();

    emit this->buttonEditClicked( quint32(ui->spinInSrcNodeX->value()), quint32(ui->spinInSrcNodeY->value()), botao->property("num").toUInt() );

}

void MainWindow::previewTrafficConfigurationClicked() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::previewTrafficConfigurationClicked" << std::endl;
#endif

    emit this->previewTrafficConfiguration( sender()->property("preview").toInt() );

}

void MainWindow::stateChangedExperiment() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::stateChangedExperiment" << std::endl;
#endif

    QCheckBox* check = (QCheckBox* ) sender();

    int numExp = check->property("exp").toInt();
    bool checked = check->isChecked();

    QGridLayout* layout = (QGridLayout* ) ui->groupBoxExperimentsConfiguration->layout();

    for(int i = 1; i < 9; i++) {
        layout->itemAtPosition(i,numExp)->widget()->setEnabled(checked);
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

    emit experimentStateChanged( numExp,checked );

}

void MainWindow::routerArchitectureChanged(int newValue) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::routerArchitectureChanged" << std::endl;
#endif

    QComboBox* combo = (QComboBox* ) sender();
    QString nomeObjeto = combo->objectName();
    int numeroExperimento = nomeObjeto.at( nomeObjeto.size() - 1 ).digitValue();

    emit configurationExperimentChanged( 1, numeroExperimento,newValue );

}

void MainWindow::routingAlgorithmChanged(int newValue) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::routingAlgorithmChanged" << std::endl;
#endif
    QComboBox* combo = (QComboBox* ) sender();
    QString nomeObjeto = combo->objectName();
    int numeroExperimento = nomeObjeto.at( nomeObjeto.size() - 1 ).digitValue();

    emit configurationExperimentChanged( 2, numeroExperimento,newValue );

}

void MainWindow::arbiterTypeChanged(int newValue) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::arbiterTypeChanged" << std::endl;
#endif
    QComboBox* combo = (QComboBox* ) sender();
    QString nomeObjeto = combo->objectName();
    int numeroExperimento = nomeObjeto.at( nomeObjeto.size() - 1 ).digitValue();

    emit configurationExperimentChanged( 4, numeroExperimento,newValue );

}

void MainWindow::flowControlChanged(int newValue) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::flowControlChanged" << std::endl;
#endif
    QComboBox* combo = (QComboBox* ) sender();
    QString nomeObjeto = combo->objectName();
    int numeroExperimento = nomeObjeto.at( nomeObjeto.size() - 1 ).digitValue();
    emit configurationExperimentChanged( 3, numeroExperimento,newValue );

}

void MainWindow::inputBuffersChanged(int newValue) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::inputBuffersChanged" << std::endl;
#endif
    QSpinBox* spin = (QSpinBox* ) sender();
    QString nomeObjeto = spin->objectName();
    int numeroExperimento = nomeObjeto.at( nomeObjeto.size() - 1 ).digitValue();

    emit configurationExperimentChanged( 5, numeroExperimento,newValue );

}

void MainWindow::outputBuffersChanged(int newValue) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::outputBuffersChanged" << std::endl;
#endif
    QSpinBox* spin = (QSpinBox* ) sender();
    QString nomeObjeto = spin->objectName();
    int numeroExperimento = nomeObjeto.at( nomeObjeto.size() - 1 ).digitValue();

    emit configurationExperimentChanged( 6, numeroExperimento,newValue );

}

void MainWindow::loadDefaultValuesExperiment() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::loadDefaultValuesExperiment" << std::endl;
#endif
    QPushButton* botao = (QPushButton* ) sender();

    int numExp = botao->property("exp").toInt();

    switch(numExp) {
        case 1:
            ui->comboInRouterArchitectureExp1->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_ROUTER_TYPE      ));
            ui->comboInRoutingAlgorithmExp1->setCurrentIndex    (qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE     ));
            ui->comboInSwitchingExp1->setCurrentIndex           (qint32(DefaultValuesSystem::DEFAULT_FC_TYPE          ));
            ui->comboInArbiterTypeExp1->setCurrentIndex         (qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE     ));
            ui->spinInInputBuffersExp1->setValue                (qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH    ));
            ui->spinInOutputBuffersExp1->setValue               (qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH   ));
            break;
        case 2:
            ui->comboInRouterArchitectureExp2->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_ROUTER_TYPE      ));
            ui->comboInRoutingAlgorithmExp2->setCurrentIndex    (qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE     ));
            ui->comboInSwitchingExp2->setCurrentIndex           (qint32(DefaultValuesSystem::DEFAULT_FC_TYPE          ));
            ui->comboInArbiterTypeExp2->setCurrentIndex         (qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE     ));
            ui->spinInInputBuffersExp2->setValue                (qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH    ));
            ui->spinInOutputBuffersExp2->setValue               (qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH   ));
            break;
        case 3:
            ui->comboInRouterArchitectureExp3->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_ROUTER_TYPE      ));
            ui->comboInRoutingAlgorithmExp3->setCurrentIndex    (qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE     ));
            ui->comboInSwitchingExp3->setCurrentIndex           (qint32(DefaultValuesSystem::DEFAULT_FC_TYPE          ));
            ui->comboInArbiterTypeExp3->setCurrentIndex         (qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE     ));
            ui->spinInInputBuffersExp3->setValue                (qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH    ));
            ui->spinInOutputBuffersExp3->setValue               (qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH   ));
            break;
        case 4:
            ui->comboInRouterArchitectureExp4->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_ROUTER_TYPE      ));
            ui->comboInRoutingAlgorithmExp4->setCurrentIndex    (qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE     ));
            ui->comboInSwitchingExp4->setCurrentIndex           (qint32(DefaultValuesSystem::DEFAULT_FC_TYPE          ));
            ui->comboInArbiterTypeExp4->setCurrentIndex         (qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE     ));
            ui->spinInInputBuffersExp4->setValue                (qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH    ));
            ui->spinInOutputBuffersExp4->setValue               (qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH   ));
            break;
        case 5:
            ui->comboInRouterArchitectureExp5->setCurrentIndex  (qint32(DefaultValuesSystem::DEFAULT_ROUTER_TYPE      ));
            ui->comboInRoutingAlgorithmExp5->setCurrentIndex    (qint32(DefaultValuesSystem::DEFAULT_ROUTING_TYPE     ));
            ui->comboInSwitchingExp5->setCurrentIndex           (qint32(DefaultValuesSystem::DEFAULT_FC_TYPE          ));
            ui->comboInArbiterTypeExp5->setCurrentIndex         (qint32(DefaultValuesSystem::DEFAULT_ARBITER_TYPE     ));
            ui->spinInInputBuffersExp5->setValue                (qint32(DefaultValuesSystem::DEFAULT_FIFO_IN_DEPTH    ));
            ui->spinInOutputBuffersExp5->setValue               (qint32(DefaultValuesSystem::DEFAULT_FIFO_OUT_DEPTH   ));
            break;

    }

}

void MainWindow::copyPreviousExperimentActive() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::copyPreviousExperimentActive" << std::endl;
#endif

    QPushButton* botao = (QPushButton* ) sender();

    int numExp = botao->property("exp").toInt();

    QGridLayout* layout = (QGridLayout* ) ui->groupBoxExperimentsConfiguration->layout();

    int previousExperiment;
    for( previousExperiment = numExp - 1 ; !layout->itemAtPosition(1,previousExperiment)->widget()->isEnabled() ; previousExperiment-- ) {}

    QComboBox* routerArchSrc = (QComboBox *) layout->itemAtPosition(1,previousExperiment)->widget();
    QComboBox* routerArchDst = (QComboBox *) layout->itemAtPosition(1,numExp)->widget();
    routerArchDst->setCurrentIndex( routerArchSrc->currentIndex() );

    QComboBox* routingAlgorithmSrc = (QComboBox *) layout->itemAtPosition(2,previousExperiment)->widget();
    QComboBox* routingAlgorithmDst = (QComboBox *) layout->itemAtPosition(2,numExp)->widget();
    routingAlgorithmDst->setCurrentIndex( routingAlgorithmSrc->currentIndex() );

    QComboBox* flowControlSrc = (QComboBox *) layout->itemAtPosition(3,previousExperiment)->widget();
    QComboBox* flowControlDst = (QComboBox *) layout->itemAtPosition(3,numExp)->widget();
    flowControlDst->setCurrentIndex( flowControlSrc->currentIndex() );

    QComboBox* arbiterTypeSrc = (QComboBox *) layout->itemAtPosition(4,previousExperiment)->widget();
    QComboBox* arbiterTypeDst = (QComboBox *) layout->itemAtPosition(4,numExp)->widget();
    arbiterTypeDst->setCurrentIndex( arbiterTypeSrc->currentIndex() );

    QSpinBox* inputBufferSrc = (QSpinBox *) layout->itemAtPosition(5,previousExperiment)->widget();
    QSpinBox* inputBufferDst = (QSpinBox *) layout->itemAtPosition(5,numExp)->widget();
    inputBufferDst->setValue( inputBufferSrc->value() );

    QSpinBox* outputBufferSrc = (QSpinBox *) layout->itemAtPosition(6,previousExperiment)->widget();
    QSpinBox* outputBufferDst = (QSpinBox *) layout->itemAtPosition(6,numExp)->widget();
    outputBufferDst->setValue( outputBufferSrc->value() );


}

void MainWindow::stopOptionUpdated(int pos) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::stopOptionUpdated" << std::endl;
#endif

    if(pos == 1) {
        this->ui->spinBoxStopTimeCycles->setVisible(false);
        this->ui->spinBoxStopTimeNs->setVisible(true);
    } else if( pos == 2 ) {
        this->ui->spinBoxStopTimeCycles->setVisible(true);
        this->ui->spinBoxStopTimeNs->setVisible(false);
    } else {
        this->ui->spinBoxStopTimeCycles->setVisible(false);
        this->ui->spinBoxStopTimeNs->setVisible(false);
    }
    emit stopOptionChanged(pos);
}

void MainWindow::vcdOptionUpdated(int value) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::vcdOptionUpdated" << std::endl;
#endif
    emit vcdOptionChanged(value);
}

void MainWindow::fClkFirstUpdated(double value) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::fClkFirstUpdated" << std::endl;
#endif

    ui->doubleSpinInChannelTclkRange->setValue( (1.0 / value ) * 1000.0);
    ui->doubleSpinInChannelBWRange->setValue( value * ui->spinInChannelWidth->value() );

    emit fClkFirstChanged(value);

}

void MainWindow::fClkLastUpdated(double value) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::fClkLastUpdated" << std::endl;
#endif

    ui->doubleSpinInChannelTclkRange_2->setValue( (1.0 / value) * 1000.0 );
    ui->doubleSpinInChannelBWRange_2->setValue( value * ui->spinInChannelWidth->value() );

    emit fClkLastChanged(value);
}

void MainWindow::fClkStepUpdated(double value) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::fClkStepUpdated" << std::endl;
#endif
    emit fClkStepChanged(value);
}

void MainWindow::fClkStepTypeUpdated(int value) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::fClkStepTypeUpdated" << std::endl;
#endif
    emit fClkStepTypeChanged(value);
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

void MainWindow::lowerAnalysisValueChanged(int valor) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::lowerAnalysisValueChanged" << std::endl;
#endif

    ui->spinBoxEndPacketsAnalyze->setMinimum(valor+1);

}

void MainWindow::upperAnalysisValueChanged(int valor) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::upperAnalysisValueChanged" << std::endl;
#endif

    ui->spinBoxInitPacketsAnalyze->setMaximum(valor-1);

}

void MainWindow::runAnalysis() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/MainWindow::runAnalysis" << std::endl;
#endif

    float lower = ui->spinBoxInitPacketsAnalyze->value();
    lower /= 100.0;
    float upper = ui->spinBoxEndPacketsAnalyze->value();
    upper /= 100.0;

    emit this->generateAnalysis(lower,upper);

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
