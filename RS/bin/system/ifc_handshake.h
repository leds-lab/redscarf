/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : ifc_handshake (handshale-based input flow controller)
FILE	 : ifc_handshake.h
--------------------------------------------------------------------------------
DESCRIPTION: Controller responsible to regulate the flow of flits at the 
input channels. It makes the adapting between the handshake flow control 
and the internal flow control protocol (FIFO).
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distribution System (LSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: July 24th, 2006.
--------------------------------------------------------------------------------
*/
#ifndef __IFC_HANDSHAKE_H__
#define __IFC_HANDSHAKE_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(ifc_handshake)
{
    // System signals
    sc_in_clk     clk;              // clock
    sc_in<bool>   rst;              // reset

    // Link interface
    sc_in<bool>   val;              // data validation
    sc_out<bool>  ret;              // return (credit or acknowledgement)

    // FIFO interface
    sc_out<bool>  wr;               // command to write a data into the FIFO
    sc_in<bool>   wok;              // FIFO has room to be written (not full)

    // Signals
    sc_signal<fsm_state> state_reg; //current state of the handshake FSM
    sc_signal<fsm_state> next_state;//next state of the handshake FSM

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    // Module's processes
    void p_state_reg();
    void p_next_state();
    void p_outputs();
    void p_debug();

    SC_HAS_PROCESS(ifc_handshake);

    //////////////////////////////////////////////////////////////////////////////
    ifc_handshake(sc_module_name nm, unsigned short int XID,
                  unsigned short int YID, unsigned short int PORT_ID) : sc_module(nm),
        XID(XID), YID(YID), PORT_ID(PORT_ID)
      //////////////////////////////////////////////////////////////////////////////
    {
        SC_METHOD(p_state_reg);
        sensitive << clk.pos() << rst;

        SC_METHOD(p_next_state);
        sensitive << state_reg << val << wok;

        SC_METHOD(p_outputs);
        sensitive << state_reg;

#ifdef DEBUG_IFC_HANDSHAKE
        SC_METHOD(p_debug);
        sensitive << clk.pos();
#endif
    }
};

#endif // __IFC_HANDSHAKE__
