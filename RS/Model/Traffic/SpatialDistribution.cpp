/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* SpatialDistribution.cpp
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

#include "SpatialDistribution.h"
#include <cmath>
#include <QString>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

SpatialDistribution::SpatialDistribution(unsigned short source,
                                         unsigned short numElements,
                                         unsigned short specificDestination)
    : source(source),numElements(numElements),specificDestination(specificDestination)
{
    numBits = ceil( log2(numElements) );
}


int SpatialDistribution::getDestination(Distribution distribution) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/Traffic/SpatialDistribution::getDestination" << std::endl;
#endif

    int destination = -1;
    switch (distribution) {
        case Specific_Address:
            destination = specificDestination;
            break;
        case Bit_Reversal:
            destination = toBitReversal();
            break;
        case Perfect_Shuffle:
            destination = toPerfectShuffle();
            break;
        case Butterfly:
            destination = toButterfly();
            break;
        case Matrix_Transpose:
            destination = toMatrixTranspose();
            break;
        case Complement:
            destination = toComplement();
            break;
        default:break;
    }

    if( destination == source ) { // Not send to itself
        destination = -1;
    }
    if( destination >= numElements ) { // Not send to an inexistent address
        destination = -1;
    }

    return destination;
}

int SpatialDistribution::toBitReversal() {
    int destination = -1;
    QString destStr = QString::number(source,2);
    destStr = destStr.rightJustified( numBits,'0' );
    for( int i = 0; i < numBits/2; i++ ) {
        QChar bitLeft  = destStr.at(i);
        QChar bitRight = destStr.at( numBits-i-1 );
        destStr[i] = bitRight;
        destStr[numBits-i-1] = bitLeft;
    }
    destination = destStr.toInt(NULL,2);
    return destination;
}

int SpatialDistribution::toPerfectShuffle() {

    int destination = -1;
    QString destStr = QString::number(source,2);
    destStr = destStr.rightJustified(numBits,'0');
    QChar msb = destStr[0];
    for( int i = 0; i < numBits-1; i++ ) {
        destStr[i] = destStr[i+1];
    }
    destStr[numBits-1] = msb;
    destination = destStr.toInt(NULL,2);
    return destination;
}

int SpatialDistribution::toButterfly() {

    int destination = -1;
    QString destStr = QString::number(source,2);
    destStr = destStr.rightJustified(numBits,'0');
    QChar msb = destStr[0];
    destStr[0] = destStr[numBits-1];
    destStr[numBits-1] = msb;
    destination = destStr.toInt(NULL,2);
    return destination;
}

int SpatialDistribution::toMatrixTranspose() {

    int destination = -1;
    QString destStr = QString::number(source,2);
    destStr = destStr.rightJustified(numBits,'0');
    QString copyDest = destStr;
    for(int i = 0; i < numBits/2; i++) {
        destStr[i] = destStr[numBits/2+i];
    }
    for(int i = numBits/2; i < numBits; i++) {
        destStr[i] = copyDest[i-numBits/2];
    }
    destination = destStr.toInt(NULL,2);
    return destination;
}

int SpatialDistribution::toComplement() {

    int destination = -1;
    destination = ~source & ((int) pow(2,numBits)-1);
    return destination;
}

QList<int> SpatialDistribution::toUniform() {

    QList<int> destinations;
    for(int i = 0; i < numElements; i++) {
        if( i != source ) {
            destinations.append(i);
        }
    }
    return destinations;
}

QList<int> SpatialDistribution::toLocal(int **topologyAdjacentyMatrix) {

    // TODO Implementar distribuição local de acordo com a topologia
    return QList<int>();
}
