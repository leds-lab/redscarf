/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : pg (priority_generator)
FILE	 : pg.h
--------------------------------------------------------------------------------
DESCRIPTION: That is a function which determines the next priority levels by 
implementing a round-robin algorithm. At each clock cycle, defined by a new 
grant to a pending request, it rotates left the current granting status and 
ensures that the request being granted will have the lowest priority level at 
the next arbitration cycle.
--------------------------------------------------------------------------------
AUTHORS: Group of Embedded and Distribution System (GSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: October 29th, 2006.
--------------------------------------------------------------------------------
*/
#ifndef __PG_H__
#define __PG_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(pg)
{
    // System signals
    sc_in_clk   clk; // clock
    sc_in<bool> rst; // reset

    // Arbitration signals
    sc_in<sc_uint<N> >  G;  // Grants
    sc_out<sc_uint<N> > P;  // Priorities

    // Internal signals
    sc_signal<bool>        update_register;	// Command to up to date Preg
    sc_signal<sc_uint<N> > Gdelayed;        // G delayed in 1 cycle
    sc_signal<sc_uint<N> > nextP;           // Next priorities values
    sc_signal<sc_uint<N> > Preg;            // Priorities register

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    // Module's processes
    void p_gdelayed();
    void p_update_register();
    void p_nextp();
    void p_preg();
    void p_outputs();
    void p_debug();

    SC_HAS_PROCESS(pg);

    //////////////////////////////////////////////////////////////////////////////
    pg(sc_module_name nm, unsigned short int XID, unsigned short int YID,
       unsigned short int PORT_ID) : sc_module(nm),  XID(XID), YID(YID),
        PORT_ID(PORT_ID)
    //////////////////////////////////////////////////////////////////////////////
    {
        SC_METHOD(p_gdelayed);
        sensitive << clk.pos() << rst;

        SC_METHOD(p_update_register);
        sensitive << G << Gdelayed;

        SC_METHOD(p_nextp);
        sensitive << G;

        SC_METHOD(p_preg);
        sensitive << clk.pos() << rst;

        SC_METHOD(p_outputs);
        sensitive << Preg;

#ifdef DEBUG_PG
        SC_METHOD(p_debug);
        sensitive << clk.pos();
#endif
    }
};
#endif // __PG__
