/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* SoCINGenerationStrategy.h
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


////////////////////////////////////////////////////////////////////////////////
// Function: To generate the SystemC model of a SoCIN or SoCIN_2vc network
//           - parameters.h  : a file with a set of defines
////////////////////////////////////////////////////////////////////////////////

#ifndef SOCINGENERATIONSTRATEGY_H
#define SOCINGENERATIONSTRATEGY_H

#include "include/Model/System/GNoC.h"

class SoCINGenerationStrategy : public GNoC {

protected:

    unsigned int useEast;   // Used to define if port E of a router is needed or not
    unsigned int useLocal;  // Used to define if port L of a router is needed or not
    unsigned int useNorth;  // Used to define if port N of a router is needed or not
    unsigned int useSoul;   // Used to define if port S of a router is needed or not
    unsigned int useWest;   // Used to define if port W of a router is needed or not

    SoCINGenerationStrategy(SystemParameters *sp,Experiment* exp) : GNoC(sp,exp){}

    void generateParameters() throw(const char* );

};

#endif // SOCINGENERATIONSTRATEGY_H
