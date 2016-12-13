/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* XmlConfigParser.cpp
* Copyright (C) 2014-2017 LEDS - Univali <zeferino@univali.br>
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

#include "Control/XmlConfigParser.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "Model/System/Defines.h"
#include "Model/System/SystemConfiguration.h"
#include "Model/System/SystemDefines.h"
#include "Model/System/SystemParameters.h"
#include "Model/System/SystemOperation.h"
#include "Model/System/Experiment.h"
#include "Control/Control.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

XmlConfigParser::XmlConfigParser() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Control/XmlConfigParser" << std::endl;
#endif
}

XmlConfigParser::~XmlConfigParser() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Control/XmlConfigParser" << std::endl;
#endif
}

TrafficParameters XmlConfigParser::parseTrafficPattern(QXmlStreamReader *xml) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/XmlConfigParser::parseTrafficPattern" << std::endl;
#endif

    TrafficParameters tp;

    QXmlStreamAttributes attributes;

    // Addressing
    xml->readNextStartElement();
    attributes = xml->attributes();
    xml->skipCurrentElement();
    tp.setSource( attributes.value("Source").toString().toUShort() );
    tp.setDestination( attributes.value("Destination").toString().toUShort() );

    // Spatial Distribution
    xml->readNextStartElement();
    attributes = xml->attributes();
    xml->skipCurrentElement();
    tp.setSpatialDistribution( tp.indexOfSpatialDistribution( attributes.value("SpatialDistribution").toString() ) );

    // Traffic Class
    xml->readNextStartElement();
    attributes = xml->attributes();
    xml->skipCurrentElement();
    tp.setTrafficClass( tp.indexOfTrafficClass( attributes.value("class").toString() ) );

    // Type injection
    xml->readNextStartElement();
    attributes = xml->attributes();
    xml->skipCurrentElement();
    unsigned int typeInjection = tp.indexOfInjectionType(attributes.value("type").toString());
    tp.setInjectionType( typeInjection );

    // Switching Technique
    xml->readNextStartElement();
    attributes = xml->attributes();
    xml->skipCurrentElement();
    tp.setSwitchingTechnique( tp.indexOfSwitchingTechnique(attributes.value("technique").toString()) );

    // Package To Send
    xml->readNextStartElement();
    attributes = xml->attributes();
    xml->skipCurrentElement();
    tp.setPackageToSend( attributes.value("value").toString().toULong() );

    // Deadline
    xml->readNextStartElement();
    attributes = xml->attributes();
    xml->skipCurrentElement();
    tp.setDeadline( attributes.value("value").toString().toULong() );

    // Required Bandwidth
    xml->readNextStartElement();
    attributes = xml->attributes();
    xml->skipCurrentElement();
    tp.setRequiredBandwidth( attributes.value("value").toString().toFloat() );

    if(typeInjection == 0 || typeInjection == 1 || typeInjection == 4 || typeInjection == 5) {
        // Message Size
        xml->readNextStartElement();
        attributes = xml->attributes();
        xml->skipCurrentElement();
        tp.setMessageSize( attributes.value("value").toString().toUInt() );
    } else if(typeInjection == 2) {
        // Idle time
        xml->readNextStartElement();
        attributes = xml->attributes();
        xml->skipCurrentElement();
        tp.setIdleTime( attributes.value("value").toString().toUInt() );
    }
    if(typeInjection == 3 || typeInjection == 5) {
        // Interval time
        xml->readNextStartElement();
        attributes = xml->attributes();
        xml->skipCurrentElement();
        tp.setIntervalTime( attributes.value("value").toString().toUInt() );
    }

    if(typeInjection != 0) {
        // Function Probability
        xml->readNextStartElement();
        attributes = xml->attributes();
        xml->skipCurrentElement();
        unsigned int funcProb = tp.indexOfProbabilityFunction( attributes.value("type").toString() );
        tp.setProbabilityFunction( funcProb );
        if(funcProb == 0) {
            // Required Bandwidth
            xml->readNextStartElement();
            attributes = xml->attributes();
            xml->skipCurrentElement();
            tp.setRequiredBandwidthStdDeviation( attributes.value("value").toString().toFloat() );
        } else if(funcProb == 2) {
            // Pareto Alfa On
            xml->readNextStartElement();
            attributes = xml->attributes();
            xml->skipCurrentElement();
            tp.setAlfaOn( attributes.value("value").toString().toFloat() );
            // Alfa Off
            xml->readNextStartElement();
            attributes = xml->attributes();
            xml->skipCurrentElement();
            tp.setAlfaOff( attributes.value("value").toString().toFloat() );
        }
    }
    return tp;
}

void XmlConfigParser::writeTrafficParametersInNode(QXmlStreamWriter *xml, TrafficParameters *tp) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/XmlConfigParser::writeTrafficParametersInNode" << std::endl;
#endif

    // Start - Addressing
    xml->writeStartElement("Addresses");
    xml->writeAttribute("Source",QString::number(tp->getSource()));
    xml->writeAttribute("Destination",QString::number(tp->getDestination()));
    xml->writeEndElement();
    // End - Addressing

    // Start - Spatial Distribution
    xml->writeStartElement("SpatialDistribution");
    xml->writeAttribute("type", tp->getSpatialDistributionName() );
    xml->writeEndElement();
    // End - Spatial Distribution

    // Start - Traffic Class
    xml->writeStartElement("TrafficClass");
    xml->writeAttribute( "class", tp->getTrafficClassName() );
    xml->writeEndElement();
    // End - Traffic Class

    unsigned int typeInjec = tp->getInjectionType();
    // Start - Injection Type
    xml->writeStartElement("InjectionType");
    xml->writeAttribute("type",tp->getInjectionTypeName());
    xml->writeEndElement();
    // End - Injection Type

    // Start - Switching Technique
    xml->writeStartElement("SwitchingTechnique");
    xml->writeAttribute("technique",tp->getSwitchingTechniqueName());
    xml->writeEndElement();
    // End - Switching Technique

    // Start - Number of Packets Per Flow
    xml->writeStartElement("NumberPacketsPerFlow");
    xml->writeAttribute("value",QString::number(tp->getPackageToSend()));
    xml->writeEndElement();
    // End - Number of Packets Per Flow

    // Start - Deadline
    xml->writeStartElement("Deadline");
    xml->writeAttribute("value",QString::number(tp->getDeadline()));
    xml->writeEndElement();
    // End - Deadline

    // Start - Required Bandwidth
    xml->writeStartElement("RequiredBandwidth");
    xml->writeAttribute("value",QString::number(tp->getRequiredBandwidth(),'f',2));
    xml->writeEndElement();
    // End - Required Bandwidth

    // Filters
    if(typeInjec == 0 || typeInjec == 1 || typeInjec == 4 || typeInjec == 5) {
        // Start - Message Size
        xml->writeStartElement("MessageSize");
        xml->writeAttribute("value",QString::number(tp->getMessageSize()));
        xml->writeEndElement();
        // End - Message Size
    } else if(typeInjec == 2) {
        // Start - Idle Time
        xml->writeStartElement("IdleTime");
        xml->writeAttribute("value",QString::number(tp->getIdleTime()));
        xml->writeEndElement();
        // End - Idle Time
    }
    if(typeInjec == 3 || typeInjec == 5) {
        // Start - Message Interval
        xml->writeStartElement("MessageInterval");
        xml->writeAttribute("value",QString::number(tp->getIntervalTime()));
        xml->writeEndElement();
        // End - Message Interval
    }

    if(typeInjec != 0) {
        // Start - Function Probability
        unsigned int funcProb = tp->getProbabilityFunction();
        xml->writeStartElement("FunctionProbability");
        xml->writeAttribute("type",tp->getProbabilityFunctionName());
        xml->writeEndElement();
        // End - Function Probability
        if(funcProb == 0) {
            // Start - Standard Deviation
            xml->writeStartElement("StandardDeviation");
            xml->writeAttribute("value",QString::number(tp->getRequiredBandwidthStdDeviation(),'f',2));
            xml->writeEndElement();
            // End - Standard Deviation
        } else if(funcProb == 2) {
            // Start - Pareto Alfa On
            xml->writeStartElement("ParetoAlfaOn");
            xml->writeAttribute("value",QString::number(tp->getAlfaOn(),'f',2));
            xml->writeEndElement();
            // End - Pareto Alfa On
            // Start - Pareto Alfa Off
            xml->writeStartElement("ParetoAlfaOff");
            xml->writeAttribute("value",QString::number(tp->getAlfaOff(),'f',2));
            xml->writeEndElement();
            // End - Pareto Alfa Off
        }
    }

}


void XmlConfigParser::saveXML(QFile *file) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/XmlConfigParser::saveXML" << std::endl;
#endif

    QXmlStreamWriter* xml = new QXmlStreamWriter(file);

    xml->setAutoFormatting(true);

    // Document start
    xml->writeStartDocument();
    xml->writeDTD( QString("<!DOCTYPE %1>").arg(APPLICATION_NAME) );
    // Root element - Application info
    xml->writeStartElement(APPLICATION_NAME);
    xml->writeAttribute("version","2.0");

    for( int i = 0; i < sysConfs.size(); i++ ) {
        SystemConfiguration conf = sysConfs.at(i);
        SystemParameters sysParam = conf.getSystemParameters();
        // Start System Configuration
        xml->writeStartElement("SystemConfiguration");
        switch (sysParam.getTopologyType()) {
            case SystemParameters::NonOrthogonal:
                xml->writeAttribute("TopologyType","NonOrthogonal");
                xml->writeAttribute("NumElements",QString::number(sysParam.getNumberElements()));
                break;
            case SystemParameters::Orthogonal2D:
                xml->writeAttribute("TopologyType","Orthogonal2D");
                xml->writeAttribute("XSize",QString::number(sysParam.getXSize()));
                xml->writeAttribute("YSize",QString::number(sysParam.getYSize()));
                break;
            case SystemParameters::Orthogonal3D:
                xml->writeAttribute("TopologyType","Orthogonal3D");
                xml->writeAttribute("XSize",QString::number(sysParam.getXSize()));
                xml->writeAttribute("YSize",QString::number(sysParam.getYSize()));
                xml->writeAttribute("ZSize",QString::number(sysParam.getZSize()));
                break;
        }

        xml->writeAttribute("DataWidth",QString::number(sysParam.getDataWidth()));

        QList<TrafficParameters> trafficConfs = conf.getTrafficConfiguration();
        for( int x = 0; x < trafficConfs.size(); x++) {
            TrafficParameters* tp = &trafficConfs[x];
            // Start Traffic Pattern
            xml->writeStartElement("TrafficPattern");
            this->writeTrafficParametersInNode(xml,tp);
            // End Traffic Pattern
            xml->writeEndElement();
        }

        // End System Configuration
        xml->writeEndElement();
    }


    SystemDefines* defSys = SystemDefines::getInstance();
    // Tab System Simulation options
    for( int i = 0; i < exps.size(); i++ ) {
        Experiment* experiment = &exps[i];
        SystemDefines::Topology topology = defSys->findTopology( experiment->getTopology() );
        SystemDefines::Routing routing = defSys->findRoutingAlgorithm( topology.getTopology() );
        SystemDefines::FlowControl flowControl = defSys->findFlowControl( experiment->getFlowControl() );
        SystemDefines::PriorityGenerator pg = defSys->findArbiterPG( experiment->getArbiterType() );
        // Start - Experiment
        xml->writeStartElement("Experiment");
        xml->writeAttribute( "index", QString::number(i) );
        xml->writeAttribute( "active", (experiment->isActive() ? "true":"false" ) );
        xml->writeAttribute( "topology", topology.getTopology() );
        xml->writeAttribute( "routingAlgorithm", routing.getRoutingAlgorithm(experiment->getRoutingAlgorithm()) );
        xml->writeAttribute( "flowControl", flowControl.getFlowControl() );
        xml->writeAttribute( "arbiterType", pg.getPG() );
        xml->writeAttribute( "vcOption",defSys->findVcOption( experiment->getVCOption() ) );
        xml->writeAttribute( "inputBuffers", QString::number( experiment->getInputBufferSize() ) );
        xml->writeAttribute( "outputBuffers", QString::number( experiment->getOutputBufferSize() ) );
        // End - Experiment
        xml->writeEndElement();
    }

    // Start - Stop option
    xml->writeStartElement("StopOption");
    xml->writeAttribute("option",QString::number(this->sysOp.stopOption));
    xml->writeAttribute("stopTime_ns",QString::number(sysOp.stopTime_ns));
    xml->writeAttribute("stopTime_cycles",QString::number(sysOp.stopTime_cycles));
    // End - Stop option
    xml->writeEndElement();

    // Start - VCD Generation
    xml->writeStartElement("VcdOption");
    xml->writeAttribute( "index",QString::number(sysOp.vcdOption) );
    // End - VCD generation
    xml->writeEndElement();

    // Start FClk Config
    xml->writeStartElement("FClk");
    xml->writeAttribute("start", QString::number(sysOp.fClkFirst) );
    xml->writeAttribute("end", QString::number(sysOp.fClkLast));
    xml->writeAttribute("stepTypeIndex",QString::number(sysOp.fClkStepType));
    xml->writeAttribute("step",QString::number(sysOp.fClkStep));
    xml->writeEndElement();
    // End FClk config

    xml->writeEndElement();
    // Fim da escrita do elemento raiz - Nome da Aplicação

    xml->writeComment(QString("Traffic pattern range in %1: 0 <-> 3").arg(APPLICATION_NAME));
    xml->writeComment(QString("Spatial Distribution in %1: Specified Address, Bit-reversal, Perfect Shuffle, Butterfly, Matrix Transpose, Complement, Uniform, Non-uniform 1 (1, 1/2, 1/4,...), Non-uniform 2 (1, 1/2), Local").arg(APPLICATION_NAME));
    xml->writeComment(QString("Traffic Class in %1: RT0-Signalling , RT1-Audio/Video, nRT0-Read/Write, nRT1-Block Transfer").arg(APPLICATION_NAME));
    xml->writeComment(QString("Type injection in %1: Constante Injection, Variable idle time - Fix message size, Variable message size - Fix idle time, Variable message size - Fix message inter-arrival, Variable message interarrival - Fix message size, Variable burst size - Fix message interarrival").arg(APPLICATION_NAME));
    xml->writeComment(QString("Switching Technique in %1: Wormhole Switching, Circuit Switching").arg(APPLICATION_NAME));
    xml->writeComment(QString("Number packets per flow range in %1: 0 - 1000000").arg(APPLICATION_NAME));
    xml->writeComment(QString("Deadline range in %1: 0 - 100000000").arg(APPLICATION_NAME));
    xml->writeComment(QString("Required Bandwidth range in %1: 0,00 - 530000,00").arg(APPLICATION_NAME));
    xml->writeComment(QString("Message size range in %1: 0 - 100000").arg(APPLICATION_NAME));
    xml->writeComment(QString("Idle time range in %1: 0 - 100000").arg(APPLICATION_NAME));
    xml->writeComment(QString("Message Interval range in %1: 0 - 100000").arg(APPLICATION_NAME));
    xml->writeComment(QString("Function Probability in %1: Normal, Exponential, Pareto").arg(APPLICATION_NAME));
    xml->writeComment(QString("Std. deviation range in %1: 0 - 100").arg(APPLICATION_NAME));
    xml->writeComment(QString("Pareto Alfa On range in %1: 0,0 - 100,0").arg(APPLICATION_NAME));
    xml->writeComment(QString("Pareto Alfa Off range in %1: 0,0 - 100,0").arg(APPLICATION_NAME));

    xml->writeEndDocument();
    // Fim da escrita no documento

    delete xml;
}

void XmlConfigParser::loadXML(QFile *file) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/XmlConfigParser::loadXML" << std::endl;
#endif

    QXmlStreamReader* xml = new QXmlStreamReader(file);

    SystemDefines* def = SystemDefines::getInstance();

//    fClkStepType = DefaultValuesSystem::DEFAULT_FCLK_STEP_TYPE;
//    stopOption = DefaultValuesSystem::DEFAULT_STOP_OPTION;
//    stopTime_ns = DefaultValuesSystem::DEFAULT_STOP_TIME_NS;
//    stopTime_cycles = DefaultValuesSystem::DEFAULT_STOP_TIME_CYCLES;
//    vcdOption = DefaultValuesSystem::DEFAULT_VCD_OPTION;
//    fClk1 = DefaultValuesSystem::DEFAULT_FCLK_FIRST;
//    fClk2 = DefaultValuesSystem::DEFAULT_FCLK_LAST;
//    fClkStep = DefaultValuesSystem::DEFAULT_FCLK_STEP_VALUE;
    while (!xml->atEnd() && !xml->hasError()) {
        // Reading elements
        QXmlStreamReader::TokenType token = xml->readNext();

        // Jump to next token when is the Start of document
        if( token == QXmlStreamReader::StartDocument ) {
            continue;
        }

        // Verify if is the start of an element
        if(token == QXmlStreamReader::StartElement) {
            // If is the root element then jump to next
            QStringRef name = xml->name();
            if( name == APPLICATION_NAME ) {
                continue;
            }

            QXmlStreamAttributes attributes = xml->attributes();
            // System configuration
            if( name == "SystemConfiguration" ) {
                SystemConfiguration sysConf;
                SystemParameters sysParam;
                SystemParameters::TopologyType topType = SystemParameters::getTopologyType( attributes.value("TopologyType").toString() );
                sysParam.setTopologyType(topType);
                switch(topType) {
                    case SystemParameters::NonOrthogonal:
                        sysParam.setNumberElements( attributes.value("NumElements").toString().toUShort() );
                        break;
                    case SystemParameters::Orthogonal3D:
                        sysParam.setZSize( attributes.value("ZSize").toString().toUShort() );
                    case SystemParameters::Orthogonal2D:
                        sysParam.setXSize( attributes.value("XSize").toString().toUShort() );
                        sysParam.setYSize( attributes.value("YSize").toString().toUShort() );
                        break;
                }
                sysParam.setDataWidth( attributes.value("DataWidth").toString().toUShort() );
                do {
                    token = xml->readNext();
                    attributes = xml->attributes();
                    if(token == QXmlStreamReader::StartElement && xml->name() == "TrafficPattern") {
                        TrafficParameters tp = this->parseTrafficPattern(xml);
                        sysConf.addTrafficConfiguration(tp);
                    }

                } while( !(token == QXmlStreamReader::EndElement && xml->name() == "SystemConfiguration") );

                sysConf.setSystemParameters(sysParam);
                this->sysConfs.append(sysConf);
            }

            // Experiment
            if( name == "Experiment" ) {
                Experiment experiment;
                experiment.setActive( (attributes.value("active").toString() == "true" ? true : false) );
                unsigned int option = def->getKeyArbiterPG(attributes.value("arbiterType").toString());
                experiment.setArbiterType( option == SystemDefines::ERROR ? 0 : option );
                option = def->getKeyFlowControl(attributes.value("flowControl").toString());
                experiment.setFlowControl( option == SystemDefines::ERROR ? 0 : option );
                experiment.setInputBufferSize( attributes.value("inputBuffers").toString().toUInt() );
                experiment.setOutputBufferSize( attributes.value("outputBuffers").toString().toUInt() );
                option = def->getKeyVcOption( attributes.value("vcOption").toString() );
                experiment.setVCOption( option == SystemDefines::ERROR ? 0 : option );
                option = def->getKeyRoutingAlgorithm(attributes.value("routingAlgorithm").toString()).second;
                experiment.setRoutingAlgorithm( option == SystemDefines::ERROR ? 0 : option );
                option = def->getKeyTopology( attributes.value("topology").toString() );
                experiment.setTopology( option == SystemDefines::ERROR ? 0 : option );

                this->exps.insert(attributes.value("index").toString().toInt(),experiment);
            }

            // Stop option
            if( name == "StopOption" ) {
                sysOp.stopOption = attributes.value("option").toString().toInt();
                sysOp.stopTime_ns = attributes.value("stopTime_ns").toString().toInt();
                sysOp.stopTime_cycles = attributes.value("stopTime_cycles").toString().toInt();
            }

            // VCD Option
            if( name == "VcdOption" ) {
                sysOp.vcdOption = attributes.value("index").toString().toInt();
            }

            // FClk
            if( name == "FClk" ) {
                sysOp.fClkFirst = attributes.value("start").toString().toDouble();
                sysOp.fClkLast = attributes.value("end").toString().toDouble();
                sysOp.fClkStepType = attributes.value("stepTypeIndex").toString().toInt();
                sysOp.fClkStep = attributes.value("step").toString().toDouble();
            }
        }
    }

    delete xml;

}
