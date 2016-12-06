/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* MainWindow.h
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

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>

class QTranslator;
class AnalysisOptions;
class ExperimentManager;
class EnvironmentConfiguration;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QList<QTranslator *> translators;
    QColor* graphLineColors[5];

    void centerWindow();
    void loadDefaultValues();
    void clearGridNodes();
    void establishConnections();
    void setProperties();
    void insertComboItens();
    void configureToolBar();
    void configureWidgets();

    enum TypeAnalysis { GraphicSelected,LatencyDistribution, Report };
    AnalysisOptions* getAnalysisOptions(TypeAnalysis opcaoAnalise);

protected:
    void changeEvent(QEvent*);
    void closeEvent(QCloseEvent *);

public:
    explicit MainWindow(QWidget *parent = 0);
    void configureLanguages(EnvironmentConfiguration* conf);

    void setPatternState(unsigned int patternNum,bool state);
    unsigned int saveChanges(QString title,QString msg);
    QString selectSystemFolder(QString msgSelect);
    void openFileError(QString error);
    void updateView(unsigned int xSize, unsigned int ySize,unsigned int zSize,
            unsigned int channelWidth, ExperimentManager* gpe,
            int stopOption, int stopTime_ns, int stopTime_cycles,
            int vcdOption, float fClkFirst, float fClkLast,
            int fClkStepType, float fClkStep);
    void enableRun();

    void setLimitsProgressBar(int minimum,int maximum);
    void updateProgressBar(int value);

    void clearConsole();
    void setAnalysisOptionsEnabled(bool enabled);

    void setActionSaveSimulationEnabled(bool enabled);
    void setActionGenerateCSVEnabled(bool enabled);
    void setOptionsSimulationEnabled(bool enabled);

    QString dialogSaveFile(QString caption,QString filter,QString currentDir=QString());
    QString dialogLoadFile(QString caption,QString filter,QString currentDir=QString());

    ~MainWindow();
signals:
    void newSystem();
    void loadSystem();
    void loadDefaultSystem();
    void saveSystem();
    void saveAsSystem();
    void saveAsDefaultSystem();
    void clearSystem();
    void exitApplication(QCloseEvent* );
    void editOptions();
    void changeLanguage(QString);
    void loadSimulationResults();
    void saveSimulationResults();
    void generateCSVSimulationReport(AnalysisOptions* aop);

    // System configuration
    void nodeSelected(unsigned int posX,unsigned int posY,unsigned int posZ);
    void sizeUpdated(unsigned int xSize,unsigned int ySize,unsigned int zSize);
    void channelWidthUpdated(unsigned int width);
    void trafficPatternUpdate(unsigned int posX,
                              unsigned int posY,
                              unsigned int posZ,
                              unsigned int trafficPattern,
                              bool state);
    void buttonEditClicked(unsigned int posX,
                           unsigned int posY,
                           unsigned int posZ,
                           unsigned int trafficNum);
    void previewTrafficConfiguration(int typePreview);
    void generateTcf();

    // System simulation
    void experimentStateChanged(int numExperimento,bool state);
    void configurationExperimentChanged(int config,int numExperimento,int novoValor);
    void stopOptionChanged(int indice);
    void stopTimeNsChanged(int newTime);
    void stopTimeCyclesChanged(int newTime);
    void vcdOptionChanged(int indice);
    void fClkFirstChanged(double);
    void fClkLastChanged(double);
    void fClkStepChanged(double);
    void fClkStepTypeChanged(int);
    void runSimulation();
    void cancel();

    // Performance Analysis
    void generateAnalysis(float lower,float upper);
    void viewWaveform();
    void viewGraphic(AnalysisOptions* aop);
    void viewReport(AnalysisOptions* aop);

public slots:
    void printConsole(QString msg,QColor color = Qt::black, Qt::Alignment alignment = Qt::AlignLeft);
    void translate(QStringList filename);
    void initConsole();

    // Tab Performance Analysis
    void runAnalysis();

private slots:

    // Actions
    // File menu

    // Tools
    void translate();
    // Help menu
    void about();

    // Tab System Configuration
    void sizeUpdate();
    void nodeClicked();
    void channelWidthUpdate(int);
    void srcNodeUpdated();
    void trafficPatternStateChanged(int state);
    void editClicked();
    void previewTrafficConfigurationClicked();
    void nodeSelectedUpdated(unsigned int posX,unsigned int posY,unsigned int posZ);

    // Tab System Simulation
    void experimentChangeState();
    void loadDefaultValuesExperiment();
    void topologyChange(int);
    void routingAlgorithmChange(int);
    void flowControlChange(int);
    void arbiterTypeChange(int);
    void vcOptionChange(int);
    void inputBuffersChange(int);
    void outputBuffersChange(int);
    void copyPreviousExperimentActive();
    void stopOptionUpdate(int);
    void vcdOptionUpdate(int);
    void fClkFirstUpdate(double);
    void fClkLastUpdate(double);
    void fClkStepUpdate(double);
    void fClkStepTypeUpdate(int);
    void run();

    // Tab Performance Analysis
    void flowSelectionUpdated(int);
    void buttonCheckUncheckAllClicked();
    void toolButtonCurveClicked();
    void lowerAnalysisValueChanged(int);
    void upperAnalysisValueChanged(int);
    void plotGraphic();
    void report();

    void generateCSVClicked();
};

#endif // MAINWINDOW_H
