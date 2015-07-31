/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* SystemParameters.h
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

#ifndef SYSTEMPARAMETERS_H
#define SYSTEMPARAMETERS_H

#include <string>

#include "include/Model/System/Defines.h"

class SystemParameters {

private:
    // System parameters
    unsigned int        ageWidth;                   // Width of the header's age field
    unsigned int        ageClockWidth;              // Width of aging counter
    unsigned int        dataWidth;                  // Width of the data channel
    // rib_width of gnoc.h
    unsigned int        destinationAddressWidth;    // Width of the destination address
    unsigned int        phitWidth;                  // Width of the flit

    unsigned int        safOption;                  // Option for SAF generation
    unsigned int        sidebandWidth;              // Width of the sideband field (framing)
    unsigned int        stopOption;                 // Stop simulation option
    unsigned long int   stopTime_cycles;            // Time to stop simulation in cycles
    unsigned long int   stopTime_ns;                // Time to stop simulation in ns
    float               tClk;                       // Clock cycle
    unsigned int        vcdOption;                  // VCD generation option
    unsigned int        xSize;                      // Network dimension in axis x
    unsigned int        ySize;                      // Network dimensions in axis y

    float               channelBandwidth;

    float               fClkFirst;
    float               fClkLast;
    float               fClkStep;
    unsigned int        fClkStepType;

public:

    // Construtor
    SystemParameters(bool defaultValues = true);

    ///////////////// GETs /////////////////
    inline unsigned int      getAgeClockWidth           () const { return this->ageClockWidth;           }
    inline unsigned int      getAgeWidth                () const { return this->ageWidth;                }
    inline unsigned int      getDataWidth               () const { return this->dataWidth;               }
    inline unsigned int      getPhitWidth               () const { return this->phitWidth;               }
    inline unsigned int      getDestinationAddressWidth () const { return this->destinationAddressWidth; }
    inline unsigned int      getSafOption               () const { return this->safOption;               }
    inline unsigned int      getSidebandWidth           () const { return this->sidebandWidth;           }
    inline unsigned int      getStopOption              () const { return this->stopOption;              }
    inline unsigned long int getStopTime_cycles         () const { return this->stopTime_cycles;         }
    inline unsigned long int getStopTime_ns             () const { return this->stopTime_ns;             }
    inline float             getTClk                    () const { return this->tClk;                    }
    inline unsigned int      getVcdOption               () const { return this->vcdOption;               }
    inline unsigned int      getXSize                   () const { return this->xSize;                   }
    inline unsigned int      getYSize                   () const { return this->ySize;                   }

    inline float             getfClkFirst               () const { return this->fClkFirst;               }
    inline float             getfClkLast                () const { return this->fClkLast;                }
    inline float             getfClkStep                () const { return this->fClkStep;                }
    inline unsigned int      getfClkStepType            () const { return this->fClkStepType;            }
    inline float             getChannelBandwidth        () const { return this->channelBandwidth;        }

    ///////////////// SETs /////////////////
    inline void setAgeClockWidth          (unsigned int      value) { this->ageClockWidth = value;                                          }
    inline void setAgeWidth               (unsigned int      value) { this->ageWidth = value;                                               }
    inline void setDataWidth              (unsigned int      value) { this->dataWidth = value; setPhitWidth(value + this->sidebandWidth);   }
    inline void setDestinationAddressWidth(unsigned int      value) { this->destinationAddressWidth = value;                                }
    inline void setPhitWidth              (unsigned int      value) { this->phitWidth = value;                                              }
    inline void setSafOption              (unsigned int      value) { this->safOption = value;                                              }
    inline void setSidebandWidth          (unsigned int      value) { this->sidebandWidth = value; setPhitWidth(value + this->dataWidth);   }
    inline void setStopOption             (unsigned int      value) { this->stopOption = value;                                             }
    inline void setStopTime_cycles        (unsigned long int value) { this->stopTime_cycles = value;                                        }
    inline void setStopTime_ns            (unsigned long int value) { this->stopTime_ns = value;                                            }
    inline void setTClk                   (float             value) { this->tClk = value;                                                   }
    inline void setVcdOption              (unsigned int      value) { this->vcdOption = value;                                              }
    inline void setXSize                  (unsigned int      value) { this->xSize = value;                                                  }
    inline void setYSize                  (unsigned int      value) { this->ySize = value;                                                  }

    inline void setfClkFirst              (float             value) { this->fClkFirst = value;                                              }
    inline void setfClkLast               (float             value) { this->fClkLast  = value;                                              }
    inline void setfClkStep               (float             value) { this->fClkStep  = value;                                              }
    inline void setfClkStepType           (unsigned int      value) { this->fClkStepType = value;                                           }
    inline void setChannelBandwidth       (float             value) { this->channelBandwidth = value;                                       }

    ///////////////// DEBUG /////////////////
    std::string toString() const;
    bool equals(SystemParameters*) const;
};

#endif // SYSTEMPARAMETERS_H
