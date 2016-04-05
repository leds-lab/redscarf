/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : pg_rd_lfsr_galois8 (priority_generator_random)
FILE   : pg_rd_lfsr_galois8.h
--------------------------------------------------------------------------------
DESCRIPTION: That is a function which determines the next priority levels by 
implementing a random algorithm. The seed is generated by XID, YID (coordinates
in mesh network) and PORT_ID (L,N,E,S,W), so each arbiter have different seed
according node position (X,Y) in network and PORT ID.
 - LFSR (one-to-many) 8-bit Galois
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distributed Systems (LEDS - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: May 29th, 2015.
By: Eduardo Alves da Silva (eduardoalves@edu.univali.br)
--------------------------------------------------------------------------------
*/
#ifndef __PG_RD_LFSR_GALOIS8_H__
#define __PG_RD_LFSR_GALOIS8_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(pg_rd_lfsr_galois8) {

    // System signals
    sc_in_clk   clk; // clock
    sc_in<bool> rst; // reset

    // Arbitration signals
    sc_in<sc_uint<N> >  G;  // Grants
    sc_out<sc_uint<N> > P;  // Priorities

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;
    unsigned short seed;  // Seed

    sc_signal<sc_uint<8> > s_REGS;          // Registers to 8-bit Galois LFSR one-to-many
    // Module's processes
    void p_update_registers();
    void p_outputs();
    void p_debug();

    // Initialize seed and order
    // - Calculates the seed according XID, YID and PORT_ID
    // and limits the minimum order to 4
    void init();

    SC_HAS_PROCESS(pg_rd_lfsr_galois8);
    //////////////////////////////////////////////////////////////////////////////
    pg_rd_lfsr_galois8(sc_module_name nm, unsigned short int XID, unsigned short int YID,
       unsigned short int PORT_ID) : sc_module(nm),  XID(XID), YID(YID),
        PORT_ID(PORT_ID)
    //////////////////////////////////////////////////////////////////////////////
    {
        init();

        SC_METHOD(p_update_registers);
        sensitive << clk.pos() << rst;

        SC_METHOD(p_outputs);
        sensitive << s_REGS;

#ifdef DEBUG_PG
        SC_METHOD(p_debug);
        sensitive << clk.pos();
#endif
    }
};
#endif // __PG_RD_LFSR_GALOIS8_H__