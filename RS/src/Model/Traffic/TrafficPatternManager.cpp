/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* TrafficPatternManager.cpp
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

#include "include/Model/Traffic/TrafficPatternManager.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

TrafficPatternManager::TrafficPatternManager(SystemParameters *sp)
    : sp(sp){
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/TrafficPattern/TrafficPatternManager" << std::endl;
#endif
    this->nodes = new std::map<unsigned int,Node*>();
}

Node* TrafficPatternManager::getNode(unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternManager::getNode" << std::endl;
#endif
    if( this->keyExist(key) ) {
        return this->nodes->find(key)->second;
    } else {
        return NULL;
    }
}

void TrafficPatternManager::clear() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternManager::clear" << std::endl;
#endif

    for( this->it = this->nodes->begin(); it != this->nodes->end(); it++ ) {
        Node* node = (*it).second;
        delete node;
    }

    this->nodes->clear();
}

void TrafficPatternManager::removeNode(unsigned int key) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternManager::removeNode" << std::endl;
#endif
    this->nodes->erase(key);
}

void TrafficPatternManager::insertNode(unsigned int key,Node *nodo) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternManager::insertNode" << std::endl;
#endif

    if( this->keyExist(key) ) {
        this->removeNode(key);
    }

    this->nodes->insert(std::pair<unsigned int, Node*>(key,nodo) );

}

bool TrafficPatternManager::keyExist(unsigned int key) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternManager::keyExist" << std::endl;
#endif
    if( this->nodes->count(key) == 0 ) {
        return false;
    } else {
        return true;
    }
}

std::string TrafficPatternManager::toString() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternManager::toString" << std::endl;
#endif

    std::stringstream format;
    std::string str = "";

    for( this->it = this->nodes->begin(); it != this->nodes->end(); it++ ) {
        Node* node = (*it).second;
        format.str("");
        format << (*it).first;
        str += "Key: "+format.str()+"\nValue: "+node->toString()+"\n\n";
    }
    return str;
}

TrafficPatternManager* TrafficPatternManager::clone() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternManager::clone" << std::endl;
#endif

    TrafficPatternManager* clone = new TrafficPatternManager(sp);

    for(it = this->nodes->begin(); it != this->nodes->end(); it++) {
        unsigned int key = (*it).first;
        Node* node = (*it).second;
        clone->insertNode( key,node->clone() );
    }

    return clone;
}

TrafficPatternManager::~TrafficPatternManager() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Model/TrafficPattern/TrafficPatternManager" << std::endl;
#endif
    this->clear();
    delete this->nodes;
}
