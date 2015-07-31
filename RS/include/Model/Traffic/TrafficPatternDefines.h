/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* TrafficPatternDefines.h
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

#ifndef TRAFFICPATTERNDEFINES_H
#define TRAFFICPATTERNDEFINES_H

#include <string>
#include <map>

// Singleton

class TrafficPatternDefines {
private:

    static TrafficPatternDefines* def;                         // Pointer singleton
    std::map<unsigned int,std::string> spatialsDistributions;  // Spatial distribution disponible
    std::map<unsigned int,std::string> trafficClasses;         // Traffic classes disponible
    std::map<unsigned int,std::string> typeInjection;          // Types injection disponible
    std::map<unsigned int,std::string> switchingTechniques;    // Switching Techniques disponible
    std::map<unsigned int,std::string> functionsProbability;   // Functions Probability disponible

    // Private constructor
    TrafficPatternDefines();

    std::string  find    (const std::map<unsigned int,std::string> &m,unsigned int key) const;
    unsigned int getKey  (const std::map<unsigned int,std::string> &m,std::string value) const;
    unsigned int size    (const std::map<unsigned int,std::string> &m) const;
    bool         keyExist(const std::map<unsigned int,std::string> &m,unsigned int key) const;

protected:
    // Spatial Distribution
    virtual void loadSpatialsDistributions();
    // Traffic Class
    virtual void loadTrafficClasses();
    // Type Injection
    virtual void loadTypesInjection();
    // Switching Technique
    virtual void loadSwitchingTechniques();
    // Function Probability
    virtual void loadFunctionsProbability();


public:

    // Singleton
    static TrafficPatternDefines* getInstance();

    // Spatial Distribution
    std::string findSpatialDistribution  (unsigned int key ) const;
    unsigned int getKeySpatialDistribution(std::string value) const;
    unsigned int sizeSpatialDistribution  () const;

    // Traffic Class
    std::string findTrafficClass  (unsigned int key ) const;
    unsigned int getKeyTrafficClass(std::string value) const;
    unsigned int sizeTrafficClass  () const;

    // Type Injection
    std::string findTypeInjection  (unsigned int key ) const;
    unsigned int getKeyTypeInjection(std::string value) const;
    unsigned int sizeTypeInjection  () const;

    // Switching Technique
    std::string findSwitchingTechnique(unsigned int key) const;
    unsigned int getKeySwitchingTechnique(std::string value) const;
    unsigned int sizeSwitchingTechnique  () const;

    // Function Probability
    std::string findFunctionProbability  (unsigned int key) const;
    unsigned int getKeyFunctionProbability(std::string value) const;
    unsigned int sizeFunctionsProbability () const;

    // Destructor
    virtual ~TrafficPatternDefines();

    // Flag on result not found!
    static const unsigned int ERROR = 999;

};

#endif // TRAFFICPATTERNDEFINES_H
