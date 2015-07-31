/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* ExperimentManager.cpp
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

#include "include/Model/System/ExperimentManager.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

ExperimentManager::ExperimentManager() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/SystemConfiguration/ExperimentManager" << std::endl;
#endif

    this->experiments = new std::map<unsigned int,Experiment* >();
}

Experiment* ExperimentManager::getExperiment(unsigned pos) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/ExperimentManager::getExperiment" << std::endl;
#endif
    if( this->chaveExiste(pos) ) {
        return this->experiments->find(pos)->second;
    } else {
        return NULL;
    }
}

void ExperimentManager::clear() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/ExperimentManager::clear" << std::endl;
#endif

    for( it = experiments->begin(); it != experiments->end(); it++ ) {
        Experiment* exp =  (*it).second;
        delete exp;
    }
    this->experiments->clear();

}

void ExperimentManager::removeExperiment(unsigned int pos) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/ExperimentManager::removeExperiment" << std::endl;
#endif
    Experiment* exp = this->getExperiment(pos);
    if(exp != NULL) {
        this->experiments->erase(pos);
        delete exp;
    }
}

void ExperimentManager::insertExperiment(unsigned int pos,Experiment *exp) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/ExperimentManager::insertExperiment" << std::endl;
#endif

    if( this->chaveExiste(pos) ) {
        this->removeExperiment(pos);
    }

    this->experiments->insert(std::pair<unsigned int, Experiment* >(pos,exp));
}

bool  ExperimentManager::chaveExiste(unsigned int chave) const {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/ExperimentManager::chaveExiste" << std::endl;
#endif

    if(this->experiments->count(chave) == 0) {
        return false;
    } else {
        return true;
    }

}

std::string ExperimentManager::toString() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/ExperimentManager::toString" << std::endl;
#endif

    std::stringstream format;
    std::string str = "";

    for( this->it = this->experiments->begin(); it != this->experiments->end(); it++ ) {
        Experiment* exp = (*it).second;
        format.str("");
        format << (*it).first;
        str += "Chave: "+format.str()+"\nValor: "+exp->toString()+"\n\n";
    }
    return str;


}

ExperimentManager::~ExperimentManager() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Model/SystemConfiguration/ExperimentManager" << std::endl;
#endif
    this->clear();
    delete this->experiments;
}
