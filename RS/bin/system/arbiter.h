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
AUTHORS: Laboratory of Embedded and Distributed Systems (LEDS - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: June 1st, 2015.
By: Eduardo Alves da Silva
--------------------------------------------------------------------------------
*/

#ifndef __ARBITER_H__
#define __ARBITER_H__

#include <systemc.h>
#include "parameters.h"
#include "ppe.h"
#include "pg_rr.h"
#include "pg_st.h"
#include "pg_rt.h"
#include "pg_rd_c_style.h"
#include "pg_rd_lfsr_fibo4.h"
#include "pg_rd_lfsr_fibo_var.h"
#include "pg_rd_lfsr_galois8.h"

////////////////////////////////////////////////////////////////////////////////
SC_MODULE(arbiter)
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

    SC_HAS_PROCESS(arbiter);

    //////////////////////////////////////////////////////////////////////////////
    arbiter(sc_module_name nm, unsigned short int XID, unsigned short int YID,
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


        if (ARBITER_TYPE == 0) { // Round-Robin arbiter
            /////////////////////////////////////////////
            pg_rr *pg0 = new pg_rr("pg",XID,YID,PORT_ID);
            /////////////////////////////////////////////
            pg0->clk(clk);
            pg0->rst(rst);
            pg0->G(G);
            pg0->P(P_wire);
        }

        if (ARBITER_TYPE == 1) { // Static (fixed) arbiter
            /////////////////////////////////////////////
            pg_st *pg0 = new pg_st("pg",XID,YID,PORT_ID);
            /////////////////////////////////////////////
            pg0->clk(clk);
            pg0->rst(rst);
            pg0->G(G);
            pg0->P(P_wire);
        }

        if (ARBITER_TYPE == 2) { // Rotating arbiter
            /////////////////////////////////////////////
            pg_rt *pg0 = new pg_rt("pg",XID,YID,PORT_ID);
            /////////////////////////////////////////////
            pg0->clk(clk);
            pg0->rst(rst);
            pg0->G(G);
            pg0->P(P_wire);
        }

        if (ARBITER_TYPE == 3) { // Random C-style arbiter
            /////////////////////////////////////////////
            pg_rd_c_style *pg0 = new pg_rd_c_style("pg",XID,YID,PORT_ID);
            /////////////////////////////////////////////
            pg0->clk(clk);
            pg0->rst(rst);
            pg0->G(G);
            pg0->P(P_wire);
        }
        if (ARBITER_TYPE == 4) { // Random LFSR Fibonacci 4-bit
            /////////////////////////////////////////////
            pg_rd_lfsr_fibo4 *pg0 = new pg_rd_lfsr_fibo4("pg",XID,YID,PORT_ID);
            /////////////////////////////////////////////
            pg0->clk(clk);
            pg0->rst(rst);
            pg0->G(G);
            pg0->P(P_wire);
        }
        if (ARBITER_TYPE == 5) { // Random LSFR Fibonacci variable order
            /////////////////////////////////////////////
            pg_rd_lfsr_fibo_var *pg0 = new pg_rd_lfsr_fibo_var("pg",XID,YID,PORT_ID);
            /////////////////////////////////////////////
            pg0->clk(clk);
            pg0->rst(rst);
            pg0->G(G);
            pg0->P(P_wire);
        }
        if (ARBITER_TYPE == 6) { // Random LFSR Galois 8-bit
            /////////////////////////////////////////////
            pg_rd_lfsr_galois8 *pg0 = new pg_rd_lfsr_galois8("pg",XID,YID,PORT_ID);
            /////////////////////////////////////////////
            pg0->clk(clk);
            pg0->rst(rst);
            pg0->G(G);
            pg0->P(P_wire);
        }
    }
};

#endif // __ARBITER_H__
