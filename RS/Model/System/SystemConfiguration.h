/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* SystemConfiguration.h
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

#ifndef __SYSTEMCONFIGURATION_H__
#define __SYSTEMCONFIGURATION_H__

#include "Model/System/SystemParameters.h"
#include "Model/Traffic/TrafficParameters.h"

#include <QList>

class SystemConfiguration {
private:
    SystemParameters sysConf;
    QList<TrafficParameters> trafficConf;

public:
    SystemConfiguration();
    SystemConfiguration(SystemParameters sp, QList<TrafficParameters> traffic);

    inline SystemParameters getSystemParameters() const { return this->sysConf; }
    inline QList<TrafficParameters> getTrafficConfiguration() const { return this->trafficConf; }

    void addTrafficConfiguration(TrafficParameters tp);

    inline void setSystemParameters(SystemParameters sp) { this->sysConf = sp; }
    inline void setTrafficConfiguration(QList<TrafficParameters> trafficConf) { this->trafficConf = trafficConf; }

    bool isValid() const;
    bool hasTraffic() const;
};

#endif // __SYSTEMCONFIGURATION_H__
