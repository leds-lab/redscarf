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
*
*/

#include "include/Model/System/SystemDefines.h"

#include <QSettings>
#include <QApplication>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

SystemDefines *SystemDefines::def = 0;

SystemDefines::SystemDefines() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/SystemConfiguration/SystemDefines" << std::endl;
#endif

    this->routerArchitectures = std::map<unsigned int,std::string>();
    this->loadRouterArchitectures();

    this->routingAlgorithms = std::map<unsigned int,std::string>();
    this->loadRoutingAlgorithms();

    this->flowControls = std::map<unsigned int,std::string>();
    this->loadFlowControls();

    this->arbiterTypes = std::map<unsigned int,std::string>();
    this->loadArbiterTypes();

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

std::string SystemDefines::find(const std::map<unsigned int, std::string> &m, unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::find" << std::endl;
#endif
    if(this->keyExist(m,key)) {
        return m.find(key)->second;
    }
    return std::string("");
}

unsigned int SystemDefines::getKey(const std::map<unsigned int, std::string> &m, std::string value) const  {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::getKey" << std::endl;
#endif

    std::map<unsigned int,std::string>::const_iterator it;

    for(it = m.begin(); it != m.end(); it++) {
        std::string v = it->second;
        if( v.compare(value) == 0 ) {
            return it->first;
        }
    }
    return ERROR;
}

unsigned int SystemDefines::size(const std::map<unsigned int, std::string> &m) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::size" << std::endl;
#endif
    return (unsigned int) m.size();
}

bool SystemDefines::keyExist(const std::map<unsigned int, std::string> &m, unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::keyExist" << std::endl;
#endif
    if(m.count(key) == 0) {
        return false;
    } else {
        return true;
    }
}

void SystemDefines::loadArbiterTypes() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::loadArbiterTypes" << std::endl;
#endif

    QSettings settings(qApp->applicationDirPath()+"/etc/system.ini",QSettings::IniFormat);
    int size = settings.beginReadArray("Arbiter_Types");
    for( int i = 0; i < size; i++ ) {
        settings.setArrayIndex(i);
        std::string value = settings.value("Type").toString().toStdString();
        this->arbiterTypes.insert( std::pair<unsigned int,std::string>(i,value) );
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
        std::string value = settings.value("Switching").toString().toStdString();
        this->flowControls.insert( std::pair<unsigned int,std::string>(i,value) );
    }
}

void SystemDefines::loadRouterArchitectures() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::loadRouterArchitectures" << std::endl;
#endif

    QSettings settings(qApp->applicationDirPath()+"/etc/system.ini",QSettings::IniFormat);
    int size = settings.beginReadArray("Router_Architectures");
    for( int i = 0; i < size; i++ ) {
        settings.setArrayIndex(i);
        std::string value = settings.value("Architecture").toString().toStdString();
        this->routerArchitectures.insert( std::pair<unsigned int,std::string>(i,value) );
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
        std::string value = settings.value("Algorithm").toString().toStdString();
        this->routingAlgorithms.insert( std::pair<unsigned int,std::string>(i,value) );
    }
}

std::string SystemDefines::findArbiterTypes(unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::findArbiterTypes" << std::endl;
#endif
    return this->find(arbiterTypes,key);
}

unsigned int SystemDefines::getKeyArbiterTypes(std::string value) const  {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::getKeyArbiterTypes" << std::endl;
#endif
    return this->getKey(arbiterTypes,value);
}

unsigned int SystemDefines::sizeArbiterTypes() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::sizeArbiterTypes" << std::endl;
#endif
    return this->size(arbiterTypes);
}

std::string SystemDefines::findFlowControls(unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::findFlowControls" << std::endl;
#endif
    return this->find(flowControls,key);
}

unsigned int SystemDefines::getKeyFlowControls(std::string value) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::getKeyFlowControls" << std::endl;
#endif
    return this->getKey(flowControls,value);
}

unsigned int SystemDefines::sizeFlowControls() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::sizeFlowControls" << std::endl;
#endif
    return this->size(flowControls);
}

std::string SystemDefines::findRoutingAlgorithms(unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::findRoutingAlgorithms" << std::endl;
#endif

    return this->find(routingAlgorithms,key);
}

unsigned int SystemDefines::getKeyRoutingAlgorithms(std::string value) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::getKeyRoutingAlgorithms" << std::endl;
#endif
    return this->getKey(routingAlgorithms,value);
}

unsigned int SystemDefines::sizeRoutingAlgorithms() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::sizeRoutingAlgorithms" << std::endl;
#endif
    return this->size(routingAlgorithms);
}

std::string SystemDefines::findRouterArchitectures(unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::findRouterArchitectures" << std::endl;
#endif
    return this->find(routerArchitectures,key);
}

unsigned int SystemDefines::getKeyRouterArchitectures(std::string value) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::getKeyRouterArchitectures" << std::endl;
#endif

    return this->getKey(routerArchitectures,value);
}

unsigned int SystemDefines::sizeRouterArchitectures() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/SystemDefines::sizeRouterArchitectures" << std::endl;
#endif
    return this->size(routerArchitectures);
}
