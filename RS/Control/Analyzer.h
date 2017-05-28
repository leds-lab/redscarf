/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Analyzer.h
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
* 20/11/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*
*/

#ifndef __ANALYZER_H__
#define __ANALYZER_H__

#include <QObject>
/*!
 * \brief The Analyzer class organize data log files for read.
 *
 * The Analyzer class read all log files and generate the reports.
 * The read it's performed really with TrafficAnalysis class by instance of PerformanceAnalysis
 * that is responsible by generate performance results.
 *
 */
class Analyzer : public QObject {
    Q_OBJECT

private:
    /*!
     * \brief analysisFolders List with contains all directories for analysis
     */
    QList<QString> *analysisFolders;
    unsigned short numElements;
    unsigned int dataWidth;
    float lower;
    float upper;
    int analyzerType;

public:
    /*!
     * \brief Analyzer class constructor that set attributes
     * \param analysisFolders Folders for the analysis
     * \param xSize Size in x-dimension
     * \param ySize Size in y-dimension
     * \param dataWidth Width of data channel
     * \param lower Start percentual of the all packets transfered that it should be dismissive in analysis
     * \param upper End percentual of the all packets transfered that it should be considered
     * \param parent QObject parent of this Analyzer
     */
    explicit Analyzer(QList<QString> *analysisFolders, unsigned short numElements,
                      unsigned int dataWidth, float lower,float upper, int analyzerType, QObject *parent = 0);
    ~Analyzer();

signals:
    /*!
     * \brief Signal for emit messages to the control
     * \param msg The message to be sent to control
     */
    void sendMessage(QString msg);
    /*!
     * \brief Signal for emit result of the analysis is successfully finished or not
     * \param success True if the analysis was finished successfully otherwise it is false
     */
    void finished(bool success);

public slots:
    /*!
     * \brief Run analysis in log files for the all folders content \p analysisFolders
     */
    void analyze();
};

#endif  // ANALYZER_H
