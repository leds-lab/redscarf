/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Builder.h
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


#ifndef BUILDER_H
#define BUILDER_H

#include <QObject>

class ExperimentManager;
class TrafficPatternManager;
class SystemParameters;
class Experiment;
QT_BEGIN_NAMESPACE
class QProcess;
QT_END_NAMESPACE

/*!
 * \brief The Builder class invokes the C++ compiler for build the simulators
 * and generates the traffic models for all configurations.
 *
 * The Builder class receive by inputs the system configurations and generates
 * the Makefile, manages cache objects, build the system and generates the traffic
 * models.
 * If successfully builded send to the control the folders for the simulations.
 */
class Builder : public QObject {
    Q_OBJECT
private:
    QProcess* builder; /*!< Process object that invokes C++ compiler */
    QString systemcDir;
    QString compilerDir;
    QString workDir;

    bool canceled;

    ExperimentManager*  experimentManager;
    SystemParameters* systemParameters;
    TrafficPatternManager* gpt;

    unsigned int amountExperimentsExecute;

    QList<QString*>* executionFolders;

    bool generateMakefile();
    int generateTrafficModels(Experiment* experiment,QString dir);
    void copyToCache(Experiment* exp);
    void copyFromCache(Experiment* exp);
    void removeObjects(const QString path);

public:
    explicit Builder(SystemParameters* sp, ExperimentManager *ge,
                     TrafficPatternManager* gpt, unsigned int amountExperimentsExecute,
                     QString workDir, QString systemc_dir, QString compilerDir = QString(),
                     QObject* parent = 0);
    ~Builder();

signals:
    void sendMessage(QString msg);
    void unsuccessful();
    void sendDirs(QList<QString*>*);
    void finished();
public slots:
    void execute();
    void cancel();
private slots:
    void readyRead();
};

#endif // BUILDER_H
