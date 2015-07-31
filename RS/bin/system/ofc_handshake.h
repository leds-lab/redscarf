/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : ofc_handshake (handshale-based output flow controller )
--------------------------------------------------------------------------------
DESCRIPTION: Controller responsible to regulate the flow of flits at the 
output channels. It makes the adapting between the handshake flow control 
and the internal flow control protocol (FIFO).
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distribution System (LSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: August 21th, 2006.
--------------------------------------------------------------------------------
*/
#ifndef __OFC_HANDSHAKE_H__
#define __OFC_HANDSHAKE_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(ofc_handshake)
{
    // System signals
    sc_in_clk    clk;  // clock
    sc_in<bool>  rst;  // reset

    // Link interface
    sc_out<bool> val; // data validation
    sc_in<bool>  ret; // return (credit or acknowledgement)

    // FIFO interface
    sc_out<bool> rd;  // command to read a data from the FIFO
    sc_in<bool>  rok; // FIFO has data to be read (not empty)

    // Signals
    sc_signal<fsm_state> state_reg;     //current state of the handshake FSM
    sc_signal<fsm_state> next_state;    //next state of the handshake FSM

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    // Module's processes
    void p_state_reg();
    void p_next_state();
    void p_outputs();
    void p_debug();

    SC_HAS_PROCESS(ofc_handshake);

    //////////////////////////////////////////////////////////////////////////////
    ofc_handshake(sc_module_name nm, unsigned short int XID,
                  unsigned short int YID, unsigned short int PORT_ID) : sc_module(nm),
        XID(XID), YID(YID), PORT_ID(PORT_ID)
    //////////////////////////////////////////////////////////////////////////////
    {
        SC_METHOD(p_state_reg);
        sensitive << clk.pos();

        SC_METHOD(p_next_state);
        sensitive << state_reg << ret << rok;

        SC_METHOD(p_outputs);
        sensitive << state_reg;

#ifdef DEBUG_OFC_HANDSHAKE
        SC_METHOD(p_debug);
        sensitive << clk.pos();
#endif
    }
};

#endif // __OFC_HANDSHAKE__
