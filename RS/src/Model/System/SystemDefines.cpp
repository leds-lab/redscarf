/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* SystemDefines.cpp
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

#include "include/Model/System/SystemDefines.h"

#include <QSettings>
#include <QApplication>
#include <QVector>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

SystemDefines *SystemDefines::def = 0;

SystemDefines::SystemDefines() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/SystemConfiguration/SystemDefines" << std::endl;
#endif
    this->loadTopologies();

    this->loadRoutingAlgorithms();

    this->loadFlowControls();

    this->loadArbiterTypes();

    this->loadVcOptions();
}

SystemDefines::~SystemDefines() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Model/SystemConfiguration/SystemDefines" << std::endl;
#endif

    SystemDefines::def = 0;
}

SystemDefines* SystemDefines::getInstance() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::getInstance" << std::endl;
#endif

    if(!def) {
        def = new SystemDefines();
    }
    return def;
}

template <class T>
T SystemDefines::find(const QMap<unsigned int, T> &m, unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::find" << std::endl;
#endif
    if(this->keyExist(m,key)) {
        return m.find(key).value();
    }
    return T();
}

template <class T>
unsigned int SystemDefines::size(const QMap<unsigned int, T> &m) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::size" << std::endl;
#endif
    return (unsigned int) m.size();
}

template <class T>
bool SystemDefines::keyExist(const QMap<unsigned int, T> &m, unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::keyExist" << std::endl;
#endif
    return m.contains(key);
}

void SystemDefines::loadTopologies() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::loadTopologies" << std::endl;
#endif
    QSettings settings(qApp->applicationDirPath()+"/etc/system.ini",QSettings::IniFormat);
    int size = settings.beginReadArray("Topologies");
    for( int i = 0; i < size; i++ ) {
        settings.setArrayIndex(i);
        Topology top;
        QString topology = settings.value("Topology").toString();
        QString alias = settings.value("Alias").toString();
        QString nocPlugin = settings.value("NoCPlugin").toString();
        QString routerPlugin = settings.value("RouterPlugin").toString();
        top.setAlias(alias);
        top.setNoCPlugin(nocPlugin);
        top.setRouterPlugin(routerPlugin);
        top.setTopology(topology);
        this->topologies.insert( i, top );
//        this->topologies.insert( std::pair<unsigned int,Topology>(i,top) );
    }

}

void SystemDefines::loadRoutingAlgorithms() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::loadRoutingAlgorithms" << std::endl;
#endif

    QSettings settings(qApp->applicationDirPath()+"/etc/system.ini",QSettings::IniFormat);
    int size = settings.beginReadArray("Routing_Algorithms");
    for( int i = 0; i < size; i++ ) {
        settings.setArrayIndex(i);
        Routing routing;
        QString topology = settings.value("Topology").toString();
        QStringList algorithms =  settings.value("Algorithms").toStringList();
        QStringList plugins = settings.value("Plugins").toStringList();
        routing.setTopology(topology);
        routing.setAlgorithms(algorithms);
        routing.setPlugins(plugins);
        this->routingAlgorithms.insert( i,routing );
//        this->routingAlgorithms.insert( std::pair<unsigned int,std::string>(i,value) );
    }
}

void SystemDefines::loadFlowControls() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::loadFlowControls" << std::endl;
#endif
    QSettings settings(qApp->applicationDirPath()+"/etc/system.ini",QSettings::IniFormat);
    int size = settings.beginReadArray("Flow_Controls");
    for( int i = 0; i < size; i++ ) {
        settings.setArrayIndex(i);
        FlowControl fc;
        QString value = settings.value("FlowControl").toString();
        QString plugin = settings.value("Plugin").toString();
        fc.setFlowControl(value);
        fc.setPlugin(plugin);
        this->flowControls.insert( i,fc );
    }
}

void SystemDefines::loadArbiterTypes() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::loadArbiterTypes" << std::endl;
#endif

    QSettings settings(qApp->applicationDirPath()+"/etc/system.ini",QSettings::IniFormat);
    int size = settings.beginReadArray("Arbiter_PGs");
    for( int i = 0; i < size; i++ ) {
        settings.setArrayIndex(i);
        PriorityGenerator pg;
        QString value = settings.value("PriorityGenerator").toString();
        QString plugin = settings.value("Plugin").toString();
        pg.setPG(value);
        pg.setPlugin(plugin);
        this->priorityGenerators.insert( i,pg );
    }
}

void SystemDefines::loadVcOptions() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::loadVcOptions" << std::endl;
#endif

    QSettings settings(qApp->applicationDirPath()+"/etc/system.ini",QSettings::IniFormat);
    int size = settings.beginReadArray("VirtualChannel_options");
    for( int i = 0; i < size; i++ ) {
        settings.setArrayIndex(i);
        QString value = settings.value("Option").toString();
        this->vcOptions.insert( i,value );
    }
}

SystemDefines::PriorityGenerator SystemDefines::findArbiterPG(unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::findArbiterPG" << std::endl;
#endif
    return this->find(this->priorityGenerators,key);
}

unsigned int SystemDefines::getKeyArbiterPG(QString value) const  {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::getKeyArbiterPG" << std::endl;
#endif
    QMap<unsigned int, PriorityGenerator>::const_iterator it;
    for( it = priorityGenerators.begin(); it != priorityGenerators.end(); it++) {
        PriorityGenerator pg = it.value();
        if( pg.getPG().compare(value) == 0 ) {
            return it.key();
        }
    }
    return ERROR;
}

int SystemDefines::sizeArbiterPGs() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::sizeArbiterPGs" << std::endl;
#endif
    return this->size(priorityGenerators);
}

SystemDefines::FlowControl SystemDefines::findFlowControl(unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::findFlowControl" << std::endl;
#endif
    return this->find(flowControls,key);
}

unsigned int SystemDefines::getKeyFlowControl(QString value) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::getKeyFlowControl" << std::endl;
#endif
    QMap<unsigned int, FlowControl>::const_iterator it;
    for( it = flowControls.begin(); it != flowControls.end(); it++ ) {
        FlowControl fc = it.value();
        if( fc.getFlowControl().compare(value) == 0 ) {
            return it.key();
        }

    }
    return ERROR;
}

int SystemDefines::sizeFlowControls() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::sizeFlowControls" << std::endl;
#endif
    return this->size(flowControls);
}

SystemDefines::Routing SystemDefines::findRoutingAlgorithm(QString topology) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::findRoutingAlgorithm" << std::endl;
#endif

    for(QMap<unsigned int,Routing>::const_iterator it = routingAlgorithms.begin();
        it != routingAlgorithms.end(); it++ ) {
        Routing r = it.value();
        if( r.getTopology().compare(topology) == 0 ) {
            return r;
        }
    }
    return Routing();
}

QPair<int,int> SystemDefines::getKeyRoutingAlgorithm(QString value) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::getKeyRoutingAlgorithm" << std::endl;
#endif
    QMap<unsigned int, Routing>::const_iterator it;
    for( it = routingAlgorithms.begin(); it != routingAlgorithms.end(); it++ ) {
        Routing routing = it.value();
        for( int i = 0; i < routing.algorithmsCount(); i++ ) {
            if( routing.containsAlgorithm(value) ) {
                return QPair<int,int>(it.key(),i);
            }
        }
    }
    return QPair<int,int>(-1,ERROR);
}

int SystemDefines::sizeRoutingAlgorithms() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::sizeRoutingAlgorithms" << std::endl;
#endif
    return this->size(routingAlgorithms);
}

SystemDefines::Topology SystemDefines::findTopology(unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::findTopolgy" << std::endl;
#endif

    return topologies.value(key);

}

unsigned int SystemDefines::getKeyTopology(QString value) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::getKeyTopology" << std::endl;
#endif
    QMap<unsigned int, Topology>::const_iterator it;
    for( it = topologies.begin(); it != topologies.end(); it++ ) {
        Topology topo = it.value();
        if( topo.getTopology().compare(value) == 0 ) {
            return it.key();
        }
    }
    return ERROR;
}

int SystemDefines::sizeTopologies() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::sizeTopologies" << std::endl;
#endif
    return this->size(topologies);
}


int SystemDefines::sizeRoutingAlgorithms(QString topology) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::sizeRoutingAlgorithms" << std::endl;
#endif
    Routing rout = findRoutingAlgorithm(topology);
    return rout.algorithmsCount();
}



QString SystemDefines::findVcOption(unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::findVcOption" << std::endl;
#endif
    return this->find(vcOptions,key);
}

unsigned int SystemDefines::getKeyVcOption(QString value) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::getKeyVcOption" << std::endl;
#endif
    unsigned int err = ERROR;
    return this->vcOptions.key(value,err);
}

int SystemDefines::sizeVcOptions() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::sizeVcOptions" << std::endl;
#endif
    return this->size(vcOptions);
}
