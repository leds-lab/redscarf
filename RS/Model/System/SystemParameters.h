/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* SystemParameters.h
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

#ifndef __SYSTEM_PARAMETERS_H__
#define __SYSTEM_PARAMETERS_H__

#include <QApplication>

class SystemParameters {
    Q_DECLARE_TR_FUNCTIONS(SystemParameters)
public:
    enum TopologyType { NonOrthogonal, Orthogonal2D,Orthogonal3D };

    SystemParameters();
    ~SystemParameters();

    bool equal(SystemParameters&) const;

    // Set
    inline void setTopologyType(TopologyType type) { this->type = type; }
    inline void setNumberElements(unsigned short number) { this->numElements = number; }
    inline void setXSize(unsigned short size) { this->xSize = size; }
    inline void setYSize(unsigned short size) { this->ySize = size; }
    inline void setZSize(unsigned short size) { this->zSize = size; }
    inline void setDataWidth(unsigned short width) { this->dataWidth = width; }
    // Get
    static TopologyType getTopologyType(QString name);
    inline TopologyType getTopologyType() const { return this->type; }
    inline unsigned short getNumberElements() const {
        switch(type) {
            case NonOrthogonal:
                return numElements;
            case Orthogonal2D:
                return (xSize * ySize);
            case Orthogonal3D:
                return (xSize * ySize * zSize);
            default: break;
        }
        return this->numElements;
    }
    inline unsigned short getXSize() const { return this->xSize; }
    inline unsigned short getYSize() const { return this->ySize; }
    inline unsigned short getZSize() const { return this->zSize; }
    inline unsigned short getDataWidth() const { return this->dataWidth; }

    QString getFormattedString() const;

private:
    TopologyType type;
    unsigned short numElements;
    unsigned short xSize;
    unsigned short ySize;
    unsigned short zSize;

    unsigned short dataWidth;
};

Q_DECLARE_METATYPE( SystemParameters )

#endif // __SYSTEM_PARAMETERS_H__
