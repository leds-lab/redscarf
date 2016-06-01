/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* TrafficPatternDefines.cpp
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

#include "include/Model/Traffic/TrafficPatternDefines.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

TrafficPatternDefines *TrafficPatternDefines::def=0;

TrafficPatternDefines::TrafficPatternDefines() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/TrafficPattern/TrafficPatternDefines" << std::endl;
#endif

    // Spatial Distribution
    this->spatialsDistributions = std::map<unsigned int,std::string>();
    this->loadSpatialsDistributions();

    // Traffic Class
    this->trafficClasses = std::map<unsigned int,std::string>();
    this->loadTrafficClasses();

    // Types Injection
    this->typeInjection = std::map<unsigned int, std::string>();
    this->loadTypesInjection();

    // Switching Technique
    this->switchingTechniques = std::map<unsigned int,std::string>();
    this->loadSwitchingTechniques();

    // Function Probability
    this->functionsProbability = std::map<unsigned int,std::string>();
    this->loadFunctionsProbability();
}

TrafficPatternDefines::~TrafficPatternDefines() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Model/TrafficPattern/TrafficPatternDefines" << std::endl;
#endif
//    delete spatialsDistributions;
//    delete trafficClasses;
//    delete typeInjection;
//    delete switchingTechniques;
//    delete functionsProbability;
//    delete TrafficPatternDefines::def;
    TrafficPatternDefines::def = 0;
}

TrafficPatternDefines* TrafficPatternDefines::getInstance() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::getInstance" << std::endl;
#endif
    if( !def ) {
        def = new TrafficPatternDefines();
    }
    return def;

}

std::string TrafficPatternDefines::find(const std::map<unsigned int, std::string> &m, unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::find" << std::endl;
#endif
    if( this->keyExist(m,key) ) {
        return m.find(key)->second;
    } else {
        return std::string("");
    }
}

unsigned int TrafficPatternDefines::getKey(const std::map<unsigned int,std::string> &m, std::string value) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::getKey" << std::endl;
#endif

    std::map<unsigned int,std::string>::const_iterator it;

    for( it = m.begin(); it != m.end(); it++ ) {
        std::string v = (*it).second;
        if( v.compare(value) == 0 ) {
            return (*it).first;
        }
    }
    return ERROR;
}

unsigned int TrafficPatternDefines::size(const std::map<unsigned int, std::string> &m) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::size" << std::endl;
#endif
    return (unsigned int) m.size();
}

bool TrafficPatternDefines::keyExist(const std::map<unsigned int, std::string> &m, unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::keyExist" << std::endl;
#endif
    if( m.count(key) == 0 ) {
        return false;
    } else {
        return true;
    }
}

void TrafficPatternDefines::loadTrafficClasses() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::loadTrafficClasses" << std::endl;
#endif
    this->trafficClasses.insert(std::pair<unsigned int,std::string>(0,"RT0 - Signalling"));
    this->trafficClasses.insert(std::pair<unsigned int,std::string>(1,"RT1 - Audio/Video"));
    this->trafficClasses.insert(std::pair<unsigned int,std::string>(2,"nRT0 - Read/Write"));
    this->trafficClasses.insert(std::pair<unsigned int,std::string>(3,"nRT1 - Block Transfers"));
}

void TrafficPatternDefines::loadSpatialsDistributions() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::loadSpatialsDistributions" << std::endl;
#endif

    this->spatialsDistributions.insert(std::pair<unsigned int,std::string>(0,"Specified Address"));
    this->spatialsDistributions.insert(std::pair<unsigned int,std::string>(1,"Bit-reversal"));
    this->spatialsDistributions.insert(std::pair<unsigned int,std::string>(2,"Perfect Shuffle"));
    this->spatialsDistributions.insert(std::pair<unsigned int,std::string>(3,"Butterfly"));
    this->spatialsDistributions.insert(std::pair<unsigned int,std::string>(4,"Matrix Transpose"));
    this->spatialsDistributions.insert(std::pair<unsigned int,std::string>(5,"Complement"));
    this->spatialsDistributions.insert(std::pair<unsigned int,std::string>(6,"Uniform"));
    this->spatialsDistributions.insert(std::pair<unsigned int,std::string>(7,"Non-uniform 1 (1, 1/2, 1/4...)"));
    this->spatialsDistributions.insert(std::pair<unsigned int,std::string>(8,"Non-uniform 2 (1, 1/2)"));
    this->spatialsDistributions.insert(std::pair<unsigned int,std::string>(9,"Local"));
}

void TrafficPatternDefines::loadTypesInjection() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::loadTypesInjection" << std::endl;
#endif
    this->typeInjection.insert(std::pair<unsigned int,std::string>(0,"Constant"));
    this->typeInjection.insert(std::pair<unsigned int,std::string>(1,"Variable idle time - Fix message size"));
    this->typeInjection.insert(std::pair<unsigned int,std::string>(2,"Variable message size - Fix idle time"));
    this->typeInjection.insert(std::pair<unsigned int,std::string>(3,"Variable message size - Fix message inter-arrival"));
    this->typeInjection.insert(std::pair<unsigned int,std::string>(4,"Variable message inter-arrival - Fix message size"));
    this->typeInjection.insert(std::pair<unsigned int,std::string>(5,"Variable burst size - Fix message inter-arrival"));
}

void TrafficPatternDefines::loadSwitchingTechniques() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::loadSwitchingTechniques" << std::endl;
#endif
    this->switchingTechniques.insert(std::pair<unsigned int,std::string>(0,"Wormhole Switching"));
    this->switchingTechniques.insert(std::pair<unsigned int,std::string>(1,"Circuit Switching"));
}

void TrafficPatternDefines::loadFunctionsProbability() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::loadFunctionsProbability" << std::endl;
#endif
    this->functionsProbability.insert(std::pair<unsigned int,std::string>(0,"Normal"));
    this->functionsProbability.insert(std::pair<unsigned int,std::string>(1,"Exponential"));
    this->functionsProbability.insert(std::pair<unsigned int,std::string>(2,"Pareto"));
}

std::string TrafficPatternDefines::findSpatialDistribution(unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::findSpatialDistribution" << std::endl;
#endif
    return this->find(this->spatialsDistributions,key);
}

unsigned int TrafficPatternDefines::sizeSpatialDistribution() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::sizeSpatialDistribution" << std::endl;
#endif
    return this->size(this->spatialsDistributions);
}

unsigned int TrafficPatternDefines::getKeySpatialDistribution(std::string value) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::getKeySpatialDistribution" << std::endl;
#endif
    return this->getKey(this->spatialsDistributions,value);
}

std::string TrafficPatternDefines::findTrafficClass(unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::findTrafficClass" << std::endl;
#endif
    return this->find(this->trafficClasses,key);
}

unsigned int TrafficPatternDefines::getKeyTrafficClass(std::string value) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::getKeyTrafficClass" << std::endl;
#endif
    return this->getKey(this->trafficClasses,value);
}

unsigned int TrafficPatternDefines::sizeTrafficClass() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::sizeTrafficClass" << std::endl;
#endif
    return this->size(this->trafficClasses);
}

std::string TrafficPatternDefines::findTypeInjection(unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::findTypeInjection" << std::endl;
#endif
    return this->find(this->typeInjection,key);
}

unsigned int TrafficPatternDefines::getKeyTypeInjection(std::string value) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::getKeyTypeInjection" << std::endl;
#endif
    return this->getKey(this->typeInjection,value);
}

unsigned int TrafficPatternDefines::sizeTypeInjection() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::sizeTypeInjection" << std::endl;
#endif
    return this->size(this->typeInjection);
}

std::string TrafficPatternDefines::findSwitchingTechnique(unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::findSwitchingTechnique" << std::endl;
#endif
    return this->find(this->switchingTechniques,key);
}

unsigned int TrafficPatternDefines::getKeySwitchingTechnique(std::string value) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::getKeySwitchingTechnique" << std::endl;
#endif
    return this->getKey(this->switchingTechniques,value);
}

unsigned int TrafficPatternDefines::sizeSwitchingTechnique() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::sizeSwitchingTechnique" << std::endl;
#endif
    return this->size(this->switchingTechniques);
}

std::string TrafficPatternDefines::findFunctionProbability(unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::findFunctionProbability" << std::endl;
#endif
    return this->find(this->functionsProbability,key);
}

unsigned int TrafficPatternDefines::getKeyFunctionProbability(std::string value) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::getKeyFunctionProbability" << std::endl;
#endif
    return this->getKey(this->functionsProbability,value);
}

unsigned int TrafficPatternDefines::sizeFunctionsProbability() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternDefines::sizeFunctionsProbability" << std::endl;
#endif
    return this->size(this->functionsProbability);
}
