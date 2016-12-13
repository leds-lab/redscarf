/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* XmlConfigParser.h
* Copyright (C) 2014-2016 LEDS - Univali <zeferino@univali.br>
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
* 31/05/2016 - 1.1     - Eduardo Alves da Silva      | First refactoring
*    ||      - ||      - Sérgio Vargas Júnior        |      ||
* ----------------------------------------------------------------------------
* 12/12/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*
*/

#ifndef __XMLCONFIGPARSER_H__
#define __XMLCONFIGPARSER_H__

#include <QList>
#include "Model/System/SystemOperation.h"

class QXmlStreamReader;
class QXmlStreamWriter;
class QFile;
class SystemConfiguration;
class TrafficParameters;
class Experiment;

class XmlConfigParser {
private:

    QList<SystemConfiguration> sysConfs;
    QList<Experiment> exps;
    SystemOperation sysOp;

    /**
     * @brief parseTrafficPattern Parse a XML Stream node to a TrafficPattern object
     * @param xml XML Stream Node
     * @return The TrafficPattern object parsed
     */
    TrafficParameters parseTrafficPattern(QXmlStreamReader* xml);
    /**
     * @brief writeTrafficParametersInNode Write a TrafficPattern object in a XML Stream Node
     * @param xml XML Stream to be written
     * @param tp TrafficPattern object to be written
     */
    void writeTrafficParametersInNode(QXmlStreamWriter* xml,TrafficParameters* tp);


public:
    XmlConfigParser();
    virtual ~XmlConfigParser();

    /**
     * @brief saveXML Save a system setup file in XML format
     * @param file A file already opened
     */
    void saveXML(QFile* file);
    /**
     * @brief loadXML Load a system setup according the file
     * @param file The file already opened
     */
    void loadXML(QFile* file);

    inline QList<SystemConfiguration> getSystemsConfigured() const { return this->sysConfs; }
    inline QList<Experiment> getExperiments() const { return this->exps; }
    inline SystemOperation getSystemOperation() const { return this->sysOp; }

    inline void setSystemsConfigured(QList<SystemConfiguration> sys) { this->sysConfs = sys; }
    inline void setExperiments(QList<Experiment> exps) { this->exps = exps; }
    inline void setSystemOperation(SystemOperation sysOp) { this->sysOp = sysOp; }
};

#endif // XMLCONFIGPARSER_H
