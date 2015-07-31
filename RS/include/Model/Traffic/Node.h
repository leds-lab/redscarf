/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Node.h
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

#ifndef NODE_H
#define NODE_H

#include "include/Model/Traffic/TrafficParameters.h"
#include "include/Model/System/Defines.h"

class Node {
private:
    unsigned int       xPosition                   ;
    unsigned int       yPosition                   ;
    TrafficParameters* trafficPattern[MAX_PATTERNS];
    bool               patternActive[MAX_PATTERNS] ;

    unsigned int       numberFlows;


    bool validIndex(unsigned int) const;

public:
    std::string fluxos;
    Node                                ( unsigned int posX, unsigned int posY      );
    ~Node();
    TrafficParameters* getTrafficPattern( unsigned int index                        ) const;
    bool               isPatternActive  ( unsigned int index                        ) const;
    void               setTrafficPattern( TrafficParameters* tp, unsigned int index );
    void               setPatternActive ( bool active, unsigned int index           );
    unsigned int       getPosicaoX      ( ) const;
    unsigned int       getPosicaoY      ( ) const;

    inline unsigned int getNumberFlows() { return this->numberFlows; }
    inline void         setNumberFlows(unsigned int nbFlows) { this->numberFlows = nbFlows; }
    inline void         incrementNumberFlows() { this->numberFlows++; }
    inline void         decrementNumberFlows() { this->numberFlows--; }
    inline void         resetNumberFlows() { this->numberFlows = 0; }


    Node* clone();

    std::string toString( ) const;
};

#endif // NODE_H
