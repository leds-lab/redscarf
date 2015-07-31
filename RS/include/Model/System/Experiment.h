/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Experiment.h
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

#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <sstream>

class Experiment {
private:


    bool active;

    unsigned int routerArchitecture;    // Router Architecture
    unsigned int routingAlgorithm;      // Routing Algorithm
    unsigned int flowControl;           // Flow Control ( Switching )
    unsigned int arbiterType;           // Arbiter type
    unsigned int inputBufferSize;       // Depth of the input buffers
    unsigned int outputBufferSize;      // Depth of the output buffers

public:
    Experiment(bool defaultValues = true);

    // Seters
    inline void setActive            (bool state              ) { this->active             = state      ; }
    inline void setRouterArchitecture(unsigned int router     ) { this->routerArchitecture = router     ; }
    inline void setRoutingAlgorithm  (unsigned int algorithm  ) { this->routingAlgorithm   = algorithm  ; }
    inline void setFlowControl       (unsigned int flowControl) { this->flowControl        = flowControl; }
    inline void setArbiterType       (unsigned int arbiter    ) { this->arbiterType        = arbiter    ; }
    inline void setInputBufferSize   (unsigned int inputSize  ) { this->inputBufferSize    = inputSize  ; }
    inline void setOutputBufferSize  (unsigned int outputSize ) { this->outputBufferSize   = outputSize ; }

    // Geters
    inline bool         isActive             () const { return this->active            ; }
    inline unsigned int getRouterArchitecture() const { return this->routerArchitecture; }
    inline unsigned int getRoutingAlgorithm  () const { return this->routingAlgorithm  ; }
    inline unsigned int getFlowControl       () const { return this->flowControl       ; }
    inline unsigned int getArbiterType       () const { return this->arbiterType       ; }
    inline unsigned int getInputBufferSize   () const { return this->inputBufferSize   ; }
    inline unsigned int getOutputBufferSize  () const { return this->outputBufferSize  ; }

    // Display all configurations in string
    std::string toString() const;
    bool equals(Experiment* exp) const;

};

#endif // EXPERIMENT_H
