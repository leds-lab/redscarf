/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : irs (input read switch)
FILE   : irs.h
--------------------------------------------------------------------------------
DESCRIPTION: Provides the command used in the input channel to read a data from 
the FIFO buffer. This command is obtained from the gnting output channel 
(requested by the input channel after routing).

IMPLEMENTATION NOTE: This entity is basically a 4-to-1 multiplexer with 
selectors based in one-hot encoding.
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distribution System (LSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: August 21th, 2006.
--------------------------------------------------------------------------------
*/
#ifndef __IRS_H__
#define __IRS_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(irs)
{
    // Interface
    sc_in<bool>	 gnt0;	 // Grant from the output channel 0
    sc_in<bool>	 gnt1;	 // Grant from the output channel 1
    sc_in<bool>	 gnt2;	 // Grant from the output channel 2
    sc_in<bool>	 gnt3;	 // Grant from the output channel 3
    sc_in<bool>  rd_in0; // Read  command from the output channel 0
    sc_in<bool>  rd_in1; // Read  command from the output channel 1
    sc_in<bool>  rd_in2; // Read  command from the output channel 2
    sc_in<bool>  rd_in3; // Read  command from the output channel 3
    sc_out<bool> rd_out; // Selected read command

    // There is no internal data structures

    // Module's processes
    void p_output();

    SC_HAS_PROCESS(irs);
    //////////////////////////////////////////////////////////////////////////////
    irs(sc_module_name nm) : sc_module(nm)
    //////////////////////////////////////////////////////////////////////////////
    {
        SC_METHOD(p_output);
        sensitive << gnt0   << gnt1   << gnt2   << gnt3 \
                  << rd_in0 << rd_in1 << rd_in2 << rd_in3;
    }
};
#endif // __IRS__

