/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* GSoC.h
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
// Name: gsoc.h
////////////////////////////////////////////////////////////////////////////////
// Function: To generate the SystemC model SoCIN-based SoC used to evaluate
//           the performance of different network configuration. It creates  the
//           following files:
//           - stopsim.(h and cpp): a model of a component that synchronizes the
//                                  end of a simulation
//
//           - main.cpp: the SystemC model of a system composed of a SoCIN NoC,
//                       traffic generators and trafic probes
//
//           - listnodes.sav: a file with formats for visualization of simulation
//                            outputs (waves.vcd) in GTKWave
////////////////////////////////////////////////////////////////////////////////

#ifndef GSOC_H
#define GSOC_H

#include "include/Model/System/SystemParameters.h"
#include "include/Model/System/Experiment.h"

#include <stdlib.h>
#include <stdio.h>

class GSoC {

private:
    SystemParameters *systemParameters;
    Experiment* experimento;

    FILE* out;
    char fileName[128];

protected:
    virtual bool generateHeaderStopSim();
    virtual bool generateStopSim();
    virtual bool generateMain();
    virtual bool generateListNodes();

public:
    GSoC(SystemParameters*,Experiment*);

    ///////////////// GET /////////////////
    inline SystemParameters* getSystemParameters() const  { return this->systemParameters; }
    ///////////////// SET /////////////////
    inline void setSystemParameters(SystemParameters* sp) { this->systemParameters = sp;   }

    virtual void generateSoC() throw(const char* );

    virtual ~GSoC() {  }
};

#endif // GSOC_H
