/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : ifc_credit_vct (credit-based input flow controller for VCT switching )
FILE	 : ifc_credit_vct.h
--------------------------------------------------------------------------------
DESCRIPTION: Controller responsible to regulate the flow of flits at the 
input channels. It makes the adapting between the credit-based flow control 
and the internal flow control protocol (FIFO) for VCT switching.
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distribution System (LSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: August 1st, 2008.
--------------------------------------------------------------------------------
*/
#ifndef __IFC_CREDIT_VCT_H__
#define __IFC_CREDIT_VCT_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(ifc_credit_vct)
{
    // System signals
    sc_in_clk     clk;                  // clock
    sc_in<bool>   rst;                  // reset

    // Link interface
    sc_in<bool>   val;                  // data validation
    sc_out<bool>  ret;                  // return (credit or acknowledgement)

    // FIFO interface
    sc_out<bool>  wr;                   // command to write a data into de FIFO
    sc_in<bool>   rd;                   // command to read a data from the FIFO
    sc_in<bool>   rok;                  // FIFO has a data to be read (not empty)
    sc_in<sc_uint<FLIT_WIDTH> > data;   // FIFO data output

    // There is no signal

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    // Module's processes
    void p_return();
    void p_valid();
    void p_debug();


    SC_HAS_PROCESS(ifc_credit_vct);

    /////////////////////////////////////////////////////////////////////////////////
    ifc_credit_vct(sc_module_name nm, unsigned short int XID, unsigned short int YID,
                   unsigned short int PORT_ID) : sc_module(nm),  XID(XID), YID(YID),
        PORT_ID(PORT_ID)
      /////////////////////////////////////////////////////////////////////////////////
    {
        SC_METHOD(p_return);
        sensitive << clk.pos() << rst;

        SC_METHOD(p_valid);
        sensitive << val;

#ifdef DEBUG_IFC_CREDIT_VCT
        SC_METHOD(p_debug);
        sensitive << clk.pos();
#endif
    }
};
#endif // __IFC_CREDIT_VCT__
