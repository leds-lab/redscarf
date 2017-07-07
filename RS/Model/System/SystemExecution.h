/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* SystemExecution.h
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

#ifndef __SYSTEMEXECUTION_H__
#define __SYSTEMEXECUTION_H__

#include "Model/System/SystemConfiguration.h"
#include "Model/System/Experiment.h"

class SystemExecution {
private:
    SystemConfiguration conf;   // System Configuration of execution
    Experiment experiment;      // Experiment of execution
    float fClk;                 // Operation frequency of execution
    QString folder;             // Folder of execution
    bool vcdOption;             // If vcd generation is enabled

public:
    SystemExecution();
    SystemExecution(SystemConfiguration conf,
                    Experiment experiment,
                    float operationFrequency,
                    QString folder,
                    bool vcdOption);

    inline void setSystemConfiguration(SystemConfiguration sysConf) { this->conf = sysConf; }
    inline void setExperiment(Experiment experiment) { this->experiment = experiment; }
    inline void setOperationFrequency(float op) { this->fClk = op; }
    inline void setWorkFolder(QString folder) { this->folder = folder; }
    inline void setVcdOption(bool option) { this->vcdOption = option; }

    inline SystemConfiguration getSystemConfiguration() const { return this->conf; }
    inline Experiment getExperiment() const { return this->experiment; }
    inline float getOperationFrequency() const { return this->fClk; }
    inline QString getWorkFolder() const { return this->folder; }
    inline bool isVcdOptionEnabled() const { return vcdOption; }
};

#endif // __SYSTEMEXECUTION_H__
