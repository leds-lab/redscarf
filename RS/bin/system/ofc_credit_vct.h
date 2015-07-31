/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : ofc_credit_vct (credit-based output flow controller for VCT switching)
FILE	 : ofc_credit_vct.h
--------------------------------------------------------------------------------
DESCRIPTION: Controller responsible to regulate the flow of flits at the 
output channels. It makes the adapting between the credit-based flow control 
and the internal flow control protocol (FIFO) for VCT switching.
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distribution System (LSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: August 21th, 2006.
--------------------------------------------------------------------------------
*/
#ifndef __OFC_CREDIT_VCT_H__
#define __OFC_CREDIT_VCT_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(ofc_credit_vct)
{
    // System signals
    sc_in_clk     clk;  // clock
    sc_in<bool>   rst;  // reset

    // Link interface
    sc_out<bool>  val;  // data validation
    sc_in<bool>   ret;  // return (credit or acknowledgement)

    // FIFO interface
    sc_out<bool>  rd;   // command to read a data from the FIFO
    sc_in<bool>   rok;  // FIFO has a data to be read (not empty)

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    // Signals
    sc_signal<sc_uint<CREDIT_COUNTER_SIZE> > counter; // The credit counter

    // Module's processes
    void p_counter();
    void p_outputs();
    void p_debug();

    SC_HAS_PROCESS(ofc_credit_vct);

    /////////////////////////////////////////////////////////////////////////////////
    ofc_credit_vct(sc_module_name nm, unsigned short int XID, unsigned short int YID,
                   unsigned short int PORT_ID) : sc_module(nm),  XID(XID), YID(YID),
        PORT_ID(PORT_ID)
      /////////////////////////////////////////////////////////////////////////////////
    {
        SC_METHOD(p_counter);
        sensitive << clk.pos() << rst;

        SC_METHOD(p_outputs);
        sensitive << rok << counter << ret ;

#ifdef DEBUG_OFC_CREDIT_VCT
        SC_METHOD(p_debug);
        sensitive << clk.pos();
#endif

    }
};
#endif // __OFC_CREDIT_VCT__
