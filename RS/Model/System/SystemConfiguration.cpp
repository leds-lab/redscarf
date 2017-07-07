/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* SystemConfiguration.cpp
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

#include "SystemConfiguration.h"

SystemConfiguration::SystemConfiguration() {}

SystemConfiguration::SystemConfiguration(SystemParameters sp, QList<TrafficParameters> traffic)
    : sysConf(sp), trafficConf(traffic)
{}

bool SystemConfiguration::isValid() const {
    if( this->sysConf.getNumberElements() > 0 ) {
        return true;
    }
    return false;
}

bool SystemConfiguration::hasTraffic() const {
    return !trafficConf.isEmpty();
}

void SystemConfiguration::addTrafficConfiguration(TrafficParameters tp) {
    this->trafficConf.append(tp);
}
