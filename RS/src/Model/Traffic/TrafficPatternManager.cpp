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

TrafficPatternManager::TrafficPatternManager() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/TrafficPattern/TrafficPatternManager" << std::endl;
#endif
    this->nodes = new std::map<unsigned int,Node*>();
}

Node* TrafficPatternManager::getNode(unsigned int posX, unsigned int posY) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternManager::getNode" << std::endl;
#endif
    unsigned int chave = posX * 10 + posY;
    if( this->chaveExiste(chave) ) {
        return this->nodes->find(chave)->second;
    } else {
        return NULL;
    }
}

void TrafficPatternManager::clear() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternManager::clear" << std::endl;
#endif

    for( this->it = this->nodes->begin(); it != this->nodes->end(); it++ ) {
        Node* nodo = (*it).second;
        delete nodo;
    }

    this->nodes->clear();
}

void TrafficPatternManager::removeNode(Node *nodo) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternManager::removeNode" << std::endl;
#endif
    this->nodes->erase(nodo->getPosicaoX() * 10 + nodo->getPosicaoY());
    delete nodo;

}

void TrafficPatternManager::removeNode(unsigned int posX, unsigned int posY) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternManager::removeNode" << std::endl;
#endif
    Node* no = this->getNode(posX,posY);
    if(no != NULL) {
        this->removeNode(no);
    }
}

void TrafficPatternManager::insertNode(Node *nodo) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternManager::insertNode" << std::endl;
#endif

    unsigned int chave = (nodo->getPosicaoX() * 10 + nodo->getPosicaoY());

    if( this->chaveExiste(chave) ) {
        this->removeNode(chave / 10 , chave % 10);
    }

    this->nodes->insert(std::pair<unsigned int, Node*>(chave,nodo) );

}

bool TrafficPatternManager::chaveExiste(unsigned int chave) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternManager::chaveExiste" << std::endl;
#endif
    if( this->nodes->count(chave) == 0 ) {
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
        Node* nodo = (*it).second;
        format.str("");
        format << (*it).first;
        str += "Chave: "+format.str()+"\nValor: "+nodo->toString()+"\n\n";
    }
    return str;
}

TrafficPatternManager* TrafficPatternManager::clone() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/TrafficPatternManager::clone" << std::endl;
#endif

    TrafficPatternManager* clone = new TrafficPatternManager();

    for(it = this->nodes->begin(); it != this->nodes->end(); it++) {
        Node* nodo = (*it).second;
        clone->insertNode( nodo->clone() );
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
