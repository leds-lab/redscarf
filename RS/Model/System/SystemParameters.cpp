/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* SystemParameters.cpp
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

#include "SystemParameters.h"

SystemParameters::SystemParameters(){
    type = NonOrthogonal;
    numElements = 0;
    xSize = 0;
    ySize = 0;
    zSize = 0;
    dataWidth = 0;
}

SystemParameters::~SystemParameters() {}

QString SystemParameters::getFormattedString() const {
    QString formatted;
    switch( type ) {
        case 0: // Non-Orthogonal
            formatted = tr("Non-Orthogonal - %1 elements")
                    .arg(numElements);
            break;
        case 1: // 2D-Orthogonal
            formatted = tr("2D-Orthogonal - %1 (%2x%3) elements")
                    .arg( (xSize*ySize) )
                    .arg(xSize)
                    .arg(ySize);
            break;
        case 2: // 3D-Orthogonal
            formatted = tr("3D-Orthogonal - %1 (%2x%3x%4) elements")
                    .arg( (xSize*ySize*zSize) )
                    .arg(xSize)
                    .arg(ySize)
                    .arg(zSize);
            break;
        default:break;
    }
    formatted += tr(" - Data Width: %1-bit").arg(dataWidth);
    return formatted;
}

bool SystemParameters::equal(SystemParameters &sp) const {
    if( type == sp.type ) {
        if( dataWidth == sp.dataWidth ) {
            switch( type ) {
                case 0: // Non-Orthogonal
                    if( numElements == sp.numElements ) {
                        return true;
                    }
                    break;
                case 1: // 2D-Orthogonal
                    if( xSize == sp.xSize && ySize == sp.ySize) {
                        return true;
                    }
                    break;
                case 2: // 3D-Orthogonal
                    if( xSize == sp.xSize && ySize == sp.ySize && zSize == sp.zSize) {
                        return true;
                    }
                    break;
                default:break;
            }

        }

    }
    return false;
}

SystemParameters::TopologyType SystemParameters::getTopologyType(QString name) {

    if( name.compare("NonOrthogonal") == 0 ) {
        return SystemParameters::NonOrthogonal;
    } else if(name.compare("Orthogonal2D") == 0) {
        return SystemParameters::Orthogonal2D;
    } else if(name.compare("Orthogonal3D") == 0) {
        return SystemParameters::Orthogonal3D;
    } else {
        return SystemParameters::NonOrthogonal; // Default value
    }
}
