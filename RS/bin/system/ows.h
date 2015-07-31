/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : ows (output write switch)
FILE   : ows.h
--------------------------------------------------------------------------------
DESCRIPTION: Provides the command used in the input channel to read a data from 
the FIFO buffer. This command is obtained from the gnt  ing output channel 
(requested by the input channel after routing).

IMPLEMENTATION NOTE: This entity is basically a 4-to-1 multiplexer with 
selectors based in one-hot encoding.
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distribution System (LSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: August 21th, 2006.
---------------------------------------------------------------------------------
*/
#ifndef __OWS_H__
#define __OWS_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(ows)
{
    // Interface
    sc_in<bool>	 gnt0;  // Grant to the input channel 0
    sc_in<bool>	 gnt1;  // Grant to the input channel 1
    sc_in<bool>	 gnt2;  // Grant to the input channel 2
    sc_in<bool>	 gnt3;  // Grant to the input channel 3
    sc_in<bool>  wr_in0;// Write command from input channel 0
    sc_in<bool>  wr_in1;// Write command from input channel 1
    sc_in<bool>  wr_in2;// Write command from input channel 2
    sc_in<bool>  wr_in3;// Write command from input channel 3
    sc_out<bool> wr_out;// Selected write command

    // There is no internal data structures

    // Module's processes
    void p_output();

    SC_HAS_PROCESS(ows);
    //////////////////////////////////////////////////////////////////////////////
    ows(sc_module_name nm) : sc_module(nm)
    //////////////////////////////////////////////////////////////////////////////
    {
        SC_METHOD(p_output);
        sensitive  << gnt0   << gnt1   << gnt2   << gnt3 \
                   << wr_in0 << wr_in1 << wr_in2 << wr_in3;
    }
};
#endif // __OWS__
