/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* SpatialDistribution.h
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

#ifndef __SPATIALDISTRIBUTION_H__
#define __SPATIALDISTRIBUTION_H__

#include <QList>

class SpatialDistribution {
private:
    unsigned short source;
    unsigned short numElements;
    unsigned short numBits;
    unsigned short specificDestination;
public:
    enum Distribution { Specific_Address = 0,
                        Bit_Reversal,
                        Perfect_Shuffle,
                        Butterfly,
                        Matrix_Transpose,
                        Complement,
                        Uniform,
                        Non_Uniform,
                        Local
                       };

    SpatialDistribution(unsigned short source,unsigned short numElements,unsigned short specificDestination=0);

    /**
     * @brief getDestination Get the destination for a source address in a single determined distribution
     * @param distribution Spatial distribution selected
     * @return The destination address of a single distribution
    */
    int getDestination(Distribution distribution);

    /**
     * Bit-reversal change from a[n-1],a[n-2],...,a[1],a[0] to a[0],a[1],...,a[n-2],a[n-1], invert the bits
     * @brief toBitReversal Calculates the destination node to Bit-Reversal distribution
     * @return The destination address in Bit-Reversal
     */
    int toBitReversal();

    /**
     * @brief Perfect Shuffle change from a[n-1], a[n-2],...,a[1],a[0] to a[n-2], a[n-3],...,a[0], a[n-1]
     * Rotates 1-bit to left
     * @return The destination address in Perfect Shuffle
     */
    int toPerfectShuffle();

    /**
     * @brief Butterfly change from a[n-1], a[n-2],....,a[1], a[0] to a[0], a[n-2],..., a[1], a[n-1]
     * Change the most significant bit with the least significant bit and vice versa
     * @return The destination address in Butterfly
     */
    int toButterfly();

    /**
     * @brief Matrix Transpose change from a[n-1], a[n-2],...,a[1],a[0] to a[(n/2)-1],...,a[0],a[n-1],...,a[n/2]
     * Rotates n/2 bits to left
     * @return The destination address in Matrix Transpose
     */
    int toMatrixTranspose();

    /**
     * @brief Complement change from a[n-1], a[n-2],...,a[1],a[0] to !a[n-1], !a[n-2],..., !a[1], !a[0]
     * Bitwise not operation
     * @return the destination node in Complement
     */
    int toComplement();

    /**
     * @brief toUniform Send to all others
     * @return A list with all addresses except the source node
     */
    QList<int> toUniform();

    /**
     * @brief toLocal Send to neighbours only
     * @param topologyAdjacentyMatrix Ajdacency matrix of the topology to discover the neighbours
     * @return A list with all neighbours addresses
     */
    QList<int> toLocal(int** topologyAdjacentyMatrix);
};

#endif // __SPATIALDISTRIBUTION_H__
