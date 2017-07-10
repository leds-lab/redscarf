/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* SystemOperation.cpp
* Copyright (C) 2014-2017 LEDS - Univali <zeferino@univali.br>
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
* ----------------------------------------------------------------------------
* 20/11/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*
*/

#include "SystemOperation.h"

#include "Defines.h"

SystemOperation::SystemOperation()
{
    this->stopOption     = DefaultValuesSystem::DEFAULT_STOP_OPTION     ; // Stop simulation option
    this->stopTime_cycles= DefaultValuesSystem::DEFAULT_STOP_TIME_CYCLES; // Time to stop simulation in cycles
    this->stopTime_ns    = DefaultValuesSystem::DEFAULT_STOP_TIME_NS    ; // Time to stop simulation in ns
    this->tClk           = DefaultValuesSystem::DEFAULT_TCLK            ; // Clock cycle
    this->vcdOption      = DefaultValuesSystem::DEFAULT_VCD_OPTION      ; // VCD generation option

    this->fClkFirst    = DefaultValuesSystem::DEFAULT_FCLK_FIRST;
    this->fClkLast     = DefaultValuesSystem::DEFAULT_FCLK_LAST;
    this->fClkStep     = DefaultValuesSystem::DEFAULT_FCLK_STEP_VALUE;
    this->fClkStepType = DefaultValuesSystem::DEFAULT_FCLK_STEP_TYPE;
}
