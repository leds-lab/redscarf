/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* MathFunction.cpp
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

#include "Model/Traffic/MathFunction.h"

#include <math.h>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

MathFunction::MathFunction() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/TrafficPattern/MathFunction" << std::endl;
#endif

}

float MathFunction::fNormal(float x, float avg, float var) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/MathFunction::fNormal" << std::endl;
#endif
    return (float) exp( (-pow((x-avg),2))/(2*pow(var,2)) ) / (float)(var*sqrt(2*PI));
}

float MathFunction::fExponential(float x, float avg) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/TrafficPattern/MathFunction::fExponential" << std::endl;
#endif
    return (float) exp((-x)/avg)/avg;
}
