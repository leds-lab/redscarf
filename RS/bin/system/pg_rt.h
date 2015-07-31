/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : pg_rt (priority_generator_rotative)
FILE	 : pg_rt.h
--------------------------------------------------------------------------------
DESCRIPTION: That is a function which determines the next priority levels by 
implementing a rotative algorithm. At each clock cycle, defined by a new
grant to a pending request, it rotates left the current granting status.
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distributed Systems (LEDS - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: May 29th, 2015.
By: Eduardo Alves da Silva (eduardoalves@edu.univali.br)
--------------------------------------------------------------------------------
*/
#ifndef __PG_RT_H__
#define __PG_RT_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(pg_rt)
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

    SC_HAS_PROCESS(pg_rt);

    //////////////////////////////////////////////////////////////////////////////
    pg_rt(sc_module_name nm, unsigned short int XID, unsigned short int YID,
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
#endif // __PG_RT_H__
