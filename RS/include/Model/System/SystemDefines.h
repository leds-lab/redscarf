/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* SystemDefines.h
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

#ifndef SYSTEMDEFINES_H
#define SYSTEMDEFINES_H

#include <string>
#include <map>

// Singleton
class SystemDefines {
private:
    static SystemDefines* def;

    std::map<unsigned int,std::string> routerArchitectures;    // Router architecture disponible
    std::map<unsigned int,std::string> routingAlgorithms;      // Routing algorithm disponible
    std::map<unsigned int,std::string> flowControls;           // Flow control disponible
    std::map<unsigned int,std::string> arbiterTypes;           // Arbiter type disponible

    // Private constructor
    SystemDefines();

    std::string  find    (const std::map<unsigned int, std::string> &m, unsigned int key) const;
    unsigned int getKey  (const std::map<unsigned int,std::string> &m,std::string value) const;
    unsigned int size    (const std::map<unsigned int,std::string>& m) const;
    bool         keyExist(const std::map<unsigned int,std::string>& m,unsigned int key) const;

protected:
    virtual void loadRouterArchitectures();
    virtual void loadRoutingAlgorithms();
    virtual void loadFlowControls();
    virtual void loadArbiterTypes();

public:

    // Singleton
    static SystemDefines* getInstance();

    std::string findRouterArchitectures(unsigned int key) const;
    unsigned int getKeyRouterArchitectures(std::string value) const;
    unsigned int sizeRouterArchitectures() const;

    std::string findRoutingAlgorithms(unsigned int key) const;
    unsigned int getKeyRoutingAlgorithms(std::string value) const;
    unsigned int sizeRoutingAlgorithms() const;

    std::string findFlowControls(unsigned int key) const;
    unsigned int getKeyFlowControls(std::string value) const;
    unsigned int sizeFlowControls() const;

    std::string findArbiterTypes(unsigned int key) const;
    unsigned int getKeyArbiterTypes(std::string value) const;
    unsigned int sizeArbiterTypes() const;

    virtual ~SystemDefines();

    // Flag on result not found
    static const unsigned int ERROR = 999;

};

#endif // SYSTEMDEFINES_H
