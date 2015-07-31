/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : ppe (priority_generator)
FILE   : ppe.h
--------------------------------------------------------------------------------
DESCRIPTION: Programmable priority encoder that receives a set of requests and 
priorities, and, based on the current priorities, schedules one of the pending 
requests by giving it a grant. It is composed by "N" arbitration cells 
interconnected in a ripple loop (wrap-around connection), implemented by signals 
which notify the next cell if some of the previous cells has already granted a 
request. This entity also include a register which holds the last granting until 
the granted request return to 0. A new grant can only be given after the arbiter 
returns to the idle state.
--------------------------------------------------------------------------------
AUTHORS: Group of Embedded and Distribution System (GSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: October 29th, 2006.
--------------------------------------------------------------------------------------------
*/
#ifndef __PPE_H__
#define __PPE_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(ppe)
{
    // System signals
    sc_in_clk   clk;            // clock
    sc_in<bool> rst;  			// reset

    // Arbitration signals
    sc_in<sc_uint<N> >  R;      // Requests
    sc_in<sc_uint<N> >  P;      // Priorities
    sc_out<sc_uint<N> > G;      // Grants
    sc_out<bool>        idle;   // Status

    // Internal signals
    sc_signal<sc_uint<N> > Imed_in;   // Some of the previous cells granted a req.
    sc_signal<sc_uint<N> > Imed_out;  // A grant was already given
    sc_signal<sc_uint<N> > Grant;     // Grant signals
    sc_signal<sc_uint<N> > Grant_reg; // Registered grant signals
    sc_signal<bool>        s_idle;    // Signal for the idle output

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    // Module's processes
    void p_imed_in();
    void p_imed_out();
    void p_grant();
    void p_grant_reg();
    void p_idle();
    void p_outputs();
    void p_debug();

    SC_HAS_PROCESS(ppe);

    //////////////////////////////////////////////////////////////////////////////
    ppe(sc_module_name nm, unsigned short int XID, unsigned short int YID,
        unsigned short int PORT_ID) : sc_module(nm),  XID(XID), YID(YID),
        PORT_ID(PORT_ID)
    //////////////////////////////////////////////////////////////////////////////
    {
        SC_METHOD(p_imed_in);
        sensitive << Imed_out;

        SC_METHOD(p_imed_out);
        sensitive << Imed_in << R << P;

        SC_METHOD(p_grant);
        sensitive << Imed_in << R << P;

        SC_METHOD(p_grant_reg);
        sensitive << clk.pos() << rst;

        SC_METHOD(p_idle);
        sensitive << Grant_reg;

        SC_METHOD(p_outputs);
        sensitive << Grant_reg << s_idle;

#ifdef DEBUG_PPE
        SC_METHOD(p_debug);
        sensitive << clk.pos();
#endif
    }
};
#endif // __PPE__
