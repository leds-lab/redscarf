/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* TrafficPatternManager.h
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

#ifndef TRAFFICPATTERNMANAGER_H
#define TRAFFICPATTERNMANAGER_H

#include "include/Model/Traffic/Node.h"

#include <map>


/*!
 * \brief The TrafficPatternManager class
 * is responsible by mantain the traffic parameters
 * applied in system
 *
 * Warning: Only supports keys to a network with 2 dimensions
 * and wich dimension with size from 0 to 9 positions
 * Pos: 9,9 -> key= '9' * 10 + '9' = 99
 */
class TrafficPatternManager {
private:
    std::map<unsigned int,Node*>           *nodes;
    std::map<unsigned int,Node*>::iterator  it   ;

    bool chaveExiste (unsigned int chave) const;

public:
    TrafficPatternManager();
    ~TrafficPatternManager();

    void   insertNode(Node* nodo);
    void   removeNode(unsigned int posX,unsigned int posY );
    void   removeNode(Node* nodo);
    Node*  getNode   (unsigned int posX, unsigned int posY) const;
    void   clear     ();

    TrafficPatternManager* clone();

    std::string toString();

};

#endif // TRAFFICPATTERNMANAGER_H
