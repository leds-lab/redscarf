/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Node.cpp
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

#include "include/Model/Traffic/Node.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

Node::Node(unsigned int posX, unsigned int posY, unsigned int posZ) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/TrafficPattern/Node" << std::endl;
#endif
    this->xPosition = posX;
    this->yPosition = posY;
    this->zPosition = posZ;
    for( int i = 0; i < MAX_PATTERNS; i++ ) {
        this->patternActive[i] = false;
        this->trafficPattern[i] = NULL;
    }
    this->numberFlows = 0;
    this->fluxos = std::string();
}

bool Node::validIndex(unsigned int indice) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/Node::validIndex" << std::endl;
#endif
    if( indice < MAX_PATTERNS ) {
        return true;
    }
    return false;
}

bool Node::isPatternActive(unsigned int index) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/Node::isPatternActive" << std::endl;
#endif
    if( this->validIndex(index) ) {
        return this->patternActive[index];
    }
    return false;
}

TrafficParameters* Node::getTrafficPattern(unsigned int index) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/Node::getTrafficPattern" << std::endl;
#endif
    if( this->validIndex(index) ) {
        return this->trafficPattern[index];
    }
    return NULL;
}

void Node::setTrafficPattern(TrafficParameters *tp, unsigned int index) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/Node::setTrafficPattern" << std::endl;
#endif
    if( this->validIndex(index) ) {
        this->patternActive[index] = true;
        if( this->trafficPattern[index] != NULL ) {
            delete this->trafficPattern[index];
        }
        this->trafficPattern[index] = tp;
    }
}

void Node::setPatternActive(bool active, unsigned int index) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/Node::setPatternActive" << std::endl;
#endif
    if( this->validIndex(index) ) {
        this->patternActive[index] = active;
    }
}

unsigned int Node::getPosicaoX() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/Node::getPosicaoX" << std::endl;
#endif
    return this->xPosition;
}

unsigned int Node::getPosicaoY() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/Node::getPosicaoY" << std::endl;
#endif
    return this->yPosition;
}

unsigned int Node::getPosicaoZ() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/Node::getPosicaoZ" << std::endl;
#endif
    return this->zPosition;
}

std::string Node::toString() const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/Node::toString" << std::endl;
#endif
    std::stringstream intToStr;
    std::string str = "";

    intToStr << this->xPosition;
    str += "Node PosX: "+intToStr.str()+"\n";

    intToStr.str("");
    intToStr << this->yPosition;
    str += "Node PosY: "+intToStr.str()+"\n";

    intToStr.str("");
    intToStr << this->zPosition;
    str += "Node PosZ: "+intToStr.str()+"\n";

    for( unsigned int i = 0; i < MAX_PATTERNS; i++ ) {
        if( this->patternActive[i] ) {
            intToStr.str("");
            intToStr << i;
            str += "Pattern["+intToStr.str()+"] active -> \n"+this->getTrafficPattern(i)->toString()+"\n";
        }
    }

    return str;
}

Node* Node::clone() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/Node::clone" << std::endl;
#endif

    Node* no = new Node(this->xPosition,this->yPosition,this->zPosition);

    for(unsigned int i = 0; i < MAX_PATTERNS; i++) {
        no->patternActive[i] = this->patternActive[i];
        TrafficParameters* tp = this->trafficPattern[i];
        if(tp != NULL) {
            no->trafficPattern[i] = new TrafficParameters(*tp);
        }
    }

    return no;
}

Node::~Node() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Model/TrafficPattern/Node" << std::endl;
#endif
    for(int i = 0; i < MAX_PATTERNS; i++) {
        delete this->trafficPattern[i];
    }
}
