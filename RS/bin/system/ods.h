/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : ods (output data switch)
FILE   : ods.h
--------------------------------------------------------------------------------
DESCRIPTION: Implements the switch used in the output channels to select a data
received from the granted input channel.

IMPLEMENTATION NOTE: This entity is basically a 4-to-1 multiplexer with 
selectors based in one-hot encoding.
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distribution System (LSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: August 21th, 2006.
---------------------------------------------------------------------------------
*/
#ifndef __ODS_H__
#define __ODS_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(ods)
{
    // Interface
    sc_in<bool>	                  gnt0; // Grant to the input channel 0
    sc_in<bool>	                  gnt1; // Grant to the input channel 1
    sc_in<bool>	                  gnt2; // Grant to the input channel 2
    sc_in<bool>	                  gnt3; // Grant to the input channel 3
    sc_in<sc_uint<FLIT_WIDTH> >   din0;	// Data from input channel 0
    sc_in<sc_uint<FLIT_WIDTH> >   din1;	// Data from input channel 1
    sc_in<sc_uint<FLIT_WIDTH> >   din2;	// Data from input channel 2
    sc_in<sc_uint<FLIT_WIDTH> >   din3;	// Data from input channel 3
    sc_out<sc_uint<FLIT_WIDTH> >  dout;	// Selected data channel

    // There is no internal data structures

    // Module's processes
    void p_output();

    SC_HAS_PROCESS(ods);
    //////////////////////////////////////////////////////////////////////////////
    ods(sc_module_name nm) : sc_module(nm)
      //////////////////////////////////////////////////////////////////////////////
    {
        SC_METHOD(p_output);
        sensitive << gnt0 << gnt1 << gnt2 << gnt3 \
                  << din0 << din1 << din2 << din3;
    }
};
#endif // __ODS__
