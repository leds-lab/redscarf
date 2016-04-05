/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : arb_rr (round-robin arbiter)
FILE	 : arb_rr.h
--------------------------------------------------------------------------------
DESCRIPTION: A round-robin arbiter based on a programmable priority encoder
and on a circular priority generator wharb_rrh updates the priorities order at 
each arbitration cycle. It ensuring that the request granted at the current 
arbitration cycle will have the lowest priority level at the next one.
--------------------------------------------------------------------------------
AUTHORS: Group of Embedded and Distribution System (GSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: October 29th, 2006.
--------------------------------------------------------------------------------
*/

#ifndef __ARB_RR_H__
#define __ARB_RR_H__

#include <systemc.h>
#include "parameters.h"
#include "pg.h"
#include "ppe.h"

////////////////////////////////////////////////////////////////////////////////
SC_MODULE(arb_rr)
////////////////////////////////////////////////////////////////////////////////
{
    // System signals
    sc_in_clk           clk;    // Clock
    sc_in<bool>         rst;    // Reset

    // Arbitration signals
    sc_in<sc_uint<N> >  R;      // Requests
    sc_out<sc_uint<N> > G;      // Grants
    sc_out<bool>        idle;   // Status

    // Data variables which implement connectors
    sc_signal<sc_uint<N> >  P_wire;	// Priorities

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    SC_HAS_PROCESS(arb_rr);

    //////////////////////////////////////////////////////////////////////////////
    arb_rr(sc_module_name nm, unsigned short int XID, unsigned short int YID,
           unsigned short int PORT_ID) : sc_module(nm),  XID(XID), YID(YID),
        PORT_ID(PORT_ID)
    //////////////////////////////////////////////////////////////////////////////
    {
        ///////////////////////////////////////////
        ppe *ppe0 = new ppe("ppe",XID,YID,PORT_ID);
        ///////////////////////////////////////////
        ppe0->clk(clk);
        ppe0->rst(rst);
        ppe0->R(R);
        ppe0->P(P_wire);
        ppe0->G(G);
        ppe0->idle(idle);

        ///////////////////////////////////////
        pg *pg0 = new pg("pg",XID,YID,PORT_ID);
        ///////////////////////////////////////
        pg0->clk(clk);
        pg0->rst(rst);
        pg0->G(G);
        pg0->P(P_wire);
    }
};

#endif // __ARB_RR_H__