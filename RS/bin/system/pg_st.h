/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : pg_st (priority_generator_static)
FILE	 : pg_st.h
--------------------------------------------------------------------------------
DESCRIPTION: That is a function which determines the priority levels by
implementing a static policy. The priority is fixed to 1 when the highest
priority is set to 1st port defined
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distributed Systems (LEDS - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: May 29th, 2015.
By: Eduardo Alves da Silva (eduardoalves@edu.univali.br)
--------------------------------------------------------------------------------
*/
#ifndef __PG_ST_H__
#define __PG_ST_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(pg_st)
{
    // System signals
    sc_in_clk   clk; // clock
    sc_in<bool> rst; // reset

    // Arbitration signals
    sc_in<sc_uint<N> >  G;  // Grants
    sc_out<sc_uint<N> > P;  // Priorities

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    SC_HAS_PROCESS(pg_st);

    //////////////////////////////////////////////////////////////////////////////
    pg_st(sc_module_name nm, unsigned short int XID, unsigned short int YID,
       unsigned short int PORT_ID) : sc_module(nm),  XID(XID), YID(YID),
        PORT_ID(PORT_ID)
    //////////////////////////////////////////////////////////////////////////////
    {
        P.initialize(1); // 0b0001
    }
};
#endif // __PG_ST_H__
