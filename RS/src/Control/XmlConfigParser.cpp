/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* XmlConfigParser.cpp
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
* 20/11/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
* ----------------------------------------------------------------------------
*
*/

#include "include/Control/XmlConfigParser.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "include/Model/System/SystemParameters.h"
#include "include/Model/System/SystemDefines.h"
#include "include/Model/System/ExperimentManager.h"
#include "include/Model/Traffic/TrafficPatternManager.h"
#include "include/Model/Traffic/TrafficPatternDefines.h"
#include "include/Control/Control.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

XmlConfigParser::XmlConfigParser(SystemParameters *sp, TrafficPatternManager *tpm, ExperimentManager *em) :
        systemParameters(sp),trafficPatternManager(tpm),expManager(em) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Control/XmlConfigParser" << std::endl;
#endif
}

XmlConfigParser::~XmlConfigParser() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Control/XmlConfigParser" << std::endl;
#endif
}

TrafficParameters* XmlConfigParser::parseTrafficPattern(QXmlStreamReader *xml) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/XmlConfigParser::parseTrafficPattern" << std::endl;
#endif

    TrafficPatternDefines* dpt = TrafficPatternDefines::getInstance();

    TrafficParameters* tp = new TrafficParameters();

    QXmlStreamAttributes attributes;

    // Spatial Distribution
    xml->readNextStartElement();
    attributes = xml->attributes();
    xml->skipCurrentElement();
    tp->setSpatialDistribution( dpt->getKeySpatialDistribution( attributes.value("type").toString().toStdString() ) );
    if( dpt->findSpatialDistribution( tp->getSpatialDistribution() ).compare( "Specified Address" ) == 0 ) {
        tp->setDestinationNodeX( attributes.value("xDestination").toString().toUInt() );
        tp->setDestinationNodeY( attributes.value("yDestination").toString().toUInt() );
    }

    // Traffic Class
    xml->readNextStartElement();
    attributes = xml->attributes();
    xml->skipCurrentElement();
    tp->setTrafficClass( dpt->getKeyTrafficClass( attributes.value("class").toString().toStdString() ) );

    // Type injection
    xml->readNextStartElement();
    attributes = xml->attributes();
    xml->skipCurrentElement();
    unsigned int typeInjection = dpt->getKeyTypeInjection( attributes.value("type").toString().toStdString() );
    tp->setInjectionType( typeInjection );

    // Switching Technique
    xml->readNextStartElement();
    attributes = xml->attributes();
    xml->skipCurrentElement();
    tp->setSwitchingTechnique( dpt->getKeySwitchingTechnique( attributes.value("technique").toString().toStdString() ) );

    // Package To Send
    xml->readNextStartElement();
    attributes = xml->attributes();
    xml->skipCurrentElement();
    tp->setPackageToSend( attributes.value("value").toString().toULong() );

    // Deadline
    xml->readNextStartElement();
    attributes = xml->attributes();
    xml->skipCurrentElement();
    tp->setDeadline( attributes.value("value").toString().toULong() );

    // Required Bandwidth
    xml->readNextStartElement();
    attributes = xml->attributes();
    xml->skipCurrentElement();
    tp->setRequiredBandwidth( attributes.value("value").toString().toFloat() );

    if(typeInjection == 0 || typeInjection == 1 || typeInjection == 4 || typeInjection == 5) {
        // Message Size
        xml->readNextStartElement();
        attributes = xml->attributes();
        xml->skipCurrentElement();
        tp->setMessageSize( attributes.value("value").toString().toUInt() );
    } else if(typeInjection == 2) {
        // Idle time
        xml->readNextStartElement();
        attributes = xml->attributes();
        xml->skipCurrentElement();
        tp->setIdleTime( attributes.value("value").toString().toUInt() );
    }
    if(typeInjection == 3 || typeInjection == 5) {
        // Interval time
        xml->readNextStartElement();
        attributes = xml->attributes();
        xml->skipCurrentElement();
        tp->setIntervalTime( attributes.value("value").toString().toUInt() );
    }

    if(typeInjection != 0) {
        // Function Probability
        xml->readNextStartElement();
        attributes = xml->attributes();
        xml->skipCurrentElement();
        unsigned int funcProb = dpt->getKeyFunctionProbability( attributes.value("type").toString().toStdString() );
        tp->setProbabilityFunction( funcProb );
        if(funcProb == 0) {
            // Required Bandwidth
            xml->readNextStartElement();
            attributes = xml->attributes();
            xml->skipCurrentElement();
            tp->setRequiredBandwidthStdDeviation( attributes.value("value").toString().toFloat() );
        } else if(funcProb == 2) {
            // Pareto Alfa On
            xml->readNextStartElement();
            attributes = xml->attributes();
            xml->skipCurrentElement();
            tp->setAlfaOn( attributes.value("value").toString().toFloat() );
            // Alfa Off
            xml->readNextStartElement();
            attributes = xml->attributes();
            xml->skipCurrentElement();
            tp->setAlfaOff( attributes.value("value").toString().toFloat() );
        }
    }
    return tp;
}

void XmlConfigParser::writeTrafficParametersInNode(QXmlStreamWriter *xml, TrafficParameters *tp) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/XmlConfigParser::writeTrafficParametersInNode" << std::endl;
#endif

    TrafficPatternDefines* dpt = TrafficPatternDefines::getInstance();

    // Início da escrita do elemento da Distribuição espacial - spatialDistribution
    xml->writeStartElement("spatialDistribution");
    std::string dist = dpt->findSpatialDistribution(tp->getSpatialDistribution());
    xml->writeAttribute("type",QString::fromStdString(dist));
    if( dist.compare("Specified Address") == 0 ) {
        xml->writeAttribute("xDestination",QString::number(tp->getDestinationNodeX()));
        xml->writeAttribute("yDestination",QString::number(tp->getDestinationNodeY()));
    }
    xml->writeEndElement();
    // Fim da escrita do elemento da Distribuição espacial - spatialDistribution

    // Início da escrita do elemento da classe de tráfego - trafficClass
    xml->writeStartElement("trafficClass");
    xml->writeAttribute( "class", QString::fromStdString(dpt->findTrafficClass(tp->getTrafficClass())));
    xml->writeEndElement();
    // Fim da escrita do elemento da classe de tráfego - trafficClass

    unsigned int typeInjec = tp->getInjectionType();
    // Início da escrita do elemento tipo de injeção - typeInjection
    xml->writeStartElement("typeInjection");
    xml->writeAttribute("type",QString::fromStdString(dpt->findTypeInjection(typeInjec)));
    xml->writeEndElement();
    // Fim da escrita do elemento tipo de injeção - typeInjection

    // Início da escrita do elemento técnica de chaveamento - switchingTechnique
    xml->writeStartElement("switchingTechnique");
    xml->writeAttribute("technique",QString::fromStdString(dpt->findSwitchingTechnique(tp->getSwitchingTechnique())));
    xml->writeEndElement();
    // Fim da escrita do elemento técnica de chaveamento - switchingTechnique

    // Início da escrita do elemento número de pacotes por fluxo - numberPacketsPerFlow
    xml->writeStartElement("numberPacketsPerFlow");
    xml->writeAttribute("value",QString::number(tp->getPackageToSend()));
    xml->writeEndElement();
    // Fim da escrita do elemento número de pacotes por fluxo - numberPacketsPerFlow

    // Início da escrita do elemento deadline
    xml->writeStartElement("deadline");
    xml->writeAttribute("value",QString::number(tp->getDeadline()));
    xml->writeEndElement();
    // Fim da escrita do elemento deadline

    // Início - requiredBandwidth
    xml->writeStartElement("requiredBandwidth");
    xml->writeAttribute("value",QString::number(tp->getRequiredBandwidth(),'f',2));
    xml->writeEndElement();
    // Fim - requiredBandwidth

    // Filtros
    if(typeInjec == 0 || typeInjec == 1 || typeInjec == 4 || typeInjec == 5) {
        // Início - messageSize
        xml->writeStartElement("messageSize");
        xml->writeAttribute("value",QString::number(tp->getMessageSize()));
        xml->writeEndElement();
        // Fim - messageSize
    } else if(typeInjec == 2) {
        // Início - idleTime
        xml->writeStartElement("idleTime");
        xml->writeAttribute("value",QString::number(tp->getIdleTime()));
        xml->writeEndElement();
        // Fim - idleTime
    }
    if(typeInjec == 3 || typeInjec == 5) {
        // Início - messageInterval
        xml->writeStartElement("messageInterval");
        xml->writeAttribute("value",QString::number(tp->getIntervalTime()));
        xml->writeEndElement();
        // Fim - messageInterval
    }

    if(typeInjec != 0) {
        // Início - functionProbability
        unsigned int funcProb = tp->getProbabilityFunction();
        xml->writeStartElement("functionProbability");
        xml->writeAttribute("type",QString::fromStdString(dpt->findFunctionProbability(funcProb)));
        xml->writeEndElement();
        // Fim - functionProbability
        if(funcProb == 0) {
            // Início - standardDeviation
            xml->writeStartElement("standardDeviation");
            xml->writeAttribute("value",QString::number(tp->getRequiredBandwidthStdDeviation(),'f',2));
            xml->writeEndElement();
            // Fim - standardDeviation
        } else if(funcProb == 2) {
            // Início - paretoAlfaOn
            xml->writeStartElement("paretoAlfaOn");
            xml->writeAttribute("value",QString::number(tp->getAlfaOn(),'f',2));
            xml->writeEndElement();
            // Fim - paretoAlfaOn
            // Início - paretoAlfaOff
            xml->writeStartElement("paretoAlfaOff");
            xml->writeAttribute("value",QString::number(tp->getAlfaOff(),'f',2));
            xml->writeEndElement();
            // Fim - paretoAlfaOff
        }
    }
}


void XmlConfigParser::saveXML(QFile *file) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/XmlConfigParser::saveXML" << std::endl;
#endif
    unsigned int xSize = this->systemParameters->getXSize();
    unsigned int ySize = this->systemParameters->getYSize();

    QXmlStreamWriter* xml = new QXmlStreamWriter(file);

    xml->setAutoFormatting(true);

    // Início da escrita no documento
    xml->writeStartDocument();
    xml->writeDTD( QString("<!DOCTYPE %1>").arg(APPLICATION_NAME) );
    // Início da escrita do elemento raiz - Nome da Aplicação
    xml->writeStartElement(APPLICATION_NAME);
    xml->writeAttribute("version","1.0");
    // Início escrita da tag do sistema - systemSize
    xml->writeStartElement("systemSize");
    xml->writeAttribute("xSize",QString::number(xSize));
    xml->writeAttribute("ySize",QString::number(ySize));
    xml->writeAttribute("channelWidth",QString::number(systemParameters->getDataWidth()));
    xml->writeComment(QString("Sistem size range in %1: x= 2 <-> 10; y= 2 <-> 10 channelWidth= 16 <-> 56")
                      .arg(APPLICATION_NAME));
    xml->writeEndElement();
    // Fim escrita da tag do sistema - systemSize

    for( unsigned int x = 0; x < xSize; x++ ) {
        for( unsigned int y = 0; y < ySize; y++ ) {
            Node* nodo = this->trafficPatternManager->getNode(x,y);
            if( nodo != NULL ) {
                bool semPadroes = true;
                // Verifica se há padrão de tráfego ativo para este nodo
                for( unsigned int i = 0; i < MAX_PATTERNS; i++ ) {
                    if( nodo->isPatternActive(i) ) {
                        semPadroes = false;
                    }
                }
                // Se não houver padrao ativo passa para próximo nodo
                if(semPadroes) {
                    continue;
                }

                // Início da escrita do elemento fonte - sourceNode
                xml->writeStartElement("sourceNode");
                xml->writeAttribute("x",QString::number(x));
                xml->writeAttribute("y",QString::number(y));
                for( unsigned int i = 0; i < MAX_PATTERNS; i++ ) {
                    if( nodo->isPatternActive(i) ) {
                        TrafficParameters* tp = nodo->getTrafficPattern(i);
                        // Início da escrita do elemento padrão de tráfego - trafficPattern
                        xml->writeStartElement("trafficPattern");
                        xml->writeAttribute("index",QString::number(i));
                        this->writeTrafficParametersInNode(xml,tp);
                        xml->writeEndElement();
                        // Fim da escrita do elemento padrão de tráfego - trafficPattern
                    }
                }
                xml->writeEndElement();
                // Fim da escrite do elemento fonte - sourceNode
            }
        }
    }

    SystemDefines* defSys = SystemDefines::getInstance();
    // Salvar a configuração dos experimentos na Aba de Simulação do Sistema
    for( unsigned int i = 1u; i <= 5u; i++ ) {
        Experiment* exp = this->expManager->getExperiment(i);
        if( exp != NULL ) {
            SystemDefines::Topology topology = defSys->findTopology( exp->getTopology() );
            SystemDefines::Routing routing = defSys->findRoutingAlgorithm( topology.getTopology() );
            SystemDefines::FlowControl flowControl = defSys->findFlowControl( exp->getFlowControl() );
            SystemDefines::PriorityGenerator pg = defSys->findArbiterPG( exp->getArbiterType() );
            xml->writeStartElement("experiment");
            xml->writeAttribute( "index", QString::number(i) );
            xml->writeAttribute( "active", (exp->isActive() ? "true":"false" ) );
            xml->writeAttribute( "topology", topology.getTopology() );
            xml->writeAttribute( "routingAlgorithm", routing.getRoutingAlgorithm(exp->getRoutingAlgorithm()) );
            xml->writeAttribute( "flowControl", flowControl.getFlowControl() );
            xml->writeAttribute( "arbiterType", pg.getPG() );
            xml->writeAttribute( "vcOption",defSys->findVcOption( exp->getVCOption() ) );
            xml->writeAttribute( "inputBuffers", QString::number( exp->getInputBufferSize() ) );
            xml->writeAttribute( "outputBuffers", QString::number( exp->getOutputBufferSize() ) );
            xml->writeEndElement();
        }
    }

    // Stop option
    xml->writeStartElement("stopOption");
    xml->writeAttribute("option",QString::number(systemParameters->getStopOption()));
    xml->writeAttribute("stopTime_ns",QString::number(systemParameters->getStopTime_ns()));
    xml->writeAttribute("stopTime_cycles",QString::number(systemParameters->getStopTime_cycles()));
    xml->writeEndElement();

    // VCD Option
    xml->writeStartElement("vcdOption");
    xml->writeAttribute( "index",QString::number(systemParameters->getVcdOption()) );
    xml->writeEndElement();

    // FClk Config
    xml->writeStartElement("fClk");
    xml->writeAttribute("start", QString::number(systemParameters->getfClkFirst()) );
    xml->writeAttribute("end", QString::number(systemParameters->getfClkLast()));
    xml->writeAttribute("stepTypeIndex",QString::number(systemParameters->getfClkStepType()));
    xml->writeAttribute("step",QString::number(systemParameters->getfClkStep()));
    xml->writeEndElement();
    // Fim FClk config

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

    this->trafficPatternManager->clear();

    SystemDefines* def = SystemDefines::getInstance();

    fClkStepType = DefaultValuesSystem::DEFAULT_FCLK_STEP_TYPE;
    stopOption = DefaultValuesSystem::DEFAULT_STOP_OPTION;
    stopTime_ns = DefaultValuesSystem::DEFAULT_STOP_TIME_NS;
    stopTime_cycles = DefaultValuesSystem::DEFAULT_STOP_TIME_CYCLES;
    vcdOption = DefaultValuesSystem::DEFAULT_VCD_OPTION;
    fClk1 = DefaultValuesSystem::DEFAULT_FCLK_FIRST;
    fClk2 = DefaultValuesSystem::DEFAULT_FCLK_LAST;
    fClkStep = DefaultValuesSystem::DEFAULT_FCLK_STEP_VALUE;
    while (!xml->atEnd() && !xml->hasError()) {
        // Lendo elementos
        QXmlStreamReader::TokenType token = xml->readNext();

        // Se for o início do documento segue para próximo token
        if( token == QXmlStreamReader::StartDocument ) {
            continue;
        }

        // Verifica o início de um elemento
        if(token == QXmlStreamReader::StartElement) {
            // Se for o elemento raiz segue para o próximo token
            QStringRef name = xml->name();
            if( name == APPLICATION_NAME ) {
                continue;
            }

            QXmlStreamAttributes attributes = xml->attributes();
            // systemSize
            if( name == "systemSize" ) {
                this->systemParameters->setXSize( attributes.value("xSize").toString().toUInt() );
                this->systemParameters->setYSize( attributes.value("ySize").toString().toUInt() );
                this->systemParameters->setDataWidth( attributes.value("channelWidth").toString().toUInt() );
                continue;
            }

            // sourceNode
            if( name == "sourceNode" ) {
                unsigned int xSrc = attributes.value("x").toString().toUInt();
                unsigned int ySrc = attributes.value("y").toString().toUInt();
                Node* nodo = new Node(xSrc,ySrc);
                // Ler Padrões de Tráfego
                do {
                    token = xml->readNext();
                    attributes = xml->attributes();
                    if( token == QXmlStreamReader::StartElement && xml->name() == "trafficPattern" ) {
                        unsigned int index = attributes.value("index").toString().toUInt();
                        TrafficParameters* tp = this->parseTrafficPattern(xml);
                        tp->setSourceNodeX( xSrc );
                        tp->setSourceNodeY( ySrc );
                        nodo->setTrafficPattern(tp,index);
                    }
                } while( !(token == QXmlStreamReader::EndElement && xml->name() == "sourceNode" ) );

                this->trafficPatternManager->insertNode(nodo);
            }

            // Experiment
            if( name == "experiment" ) {
                Experiment* experiment = new Experiment;
                experiment->setActive( (attributes.value("active").toString() == "true" ? true : false) );
                unsigned int option = def->getKeyArbiterPG(attributes.value("arbiterType").toString());
                experiment->setArbiterType( option == SystemDefines::ERROR ? 0 : option );
                option = def->getKeyFlowControl(attributes.value("flowControl").toString());
                experiment->setFlowControl( option == SystemDefines::ERROR ? 0 : option );
                experiment->setInputBufferSize( attributes.value("inputBuffers").toString().toUInt() );
                experiment->setOutputBufferSize( attributes.value("outputBuffers").toString().toUInt() );
                option = def->getKeyVcOption( attributes.value("vcOption").toString() );
                experiment->setVCOptions( option == SystemDefines::ERROR ? 0 : option );
                option = def->getKeyRoutingAlgorithm(attributes.value("routingAlgorithm").toString()).second;
                experiment->setRoutingAlgorithm( option == SystemDefines::ERROR ? 0 : option );
                option = def->getKeyTopology( attributes.value("topology").toString() );
                experiment->setTopology( option == SystemDefines::ERROR ? 0 : option );

                this->expManager->insertExperiment(attributes.value("index").toString().toUInt(),experiment);
            }

            // Stop option
            if( name == "stopOption" ) {
                stopOption = attributes.value("option").toString().toInt();
                stopTime_ns = attributes.value("stopTime_ns").toString().toInt();
                stopTime_cycles = attributes.value("stopTime_cycles").toString().toInt();
            }

            // VCD Option
            if( name == "vcdOption" ) {
                vcdOption = attributes.value("index").toString().toInt();
            }

            // FClk
            if( name == "fClk" ) {
                fClk1 = attributes.value("start").toString().toDouble();
                fClk2 = attributes.value("end").toString().toDouble();
                fClkStepType = attributes.value("stepTypeIndex").toString().toInt();
                fClkStep = attributes.value("step").toString().toDouble();
            }

        }

    }

    delete xml;

}
