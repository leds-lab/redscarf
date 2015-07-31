/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : req_req_cs (request register with circuit-switching function)
FILE	 : req_reg_cs.h
--------------------------------------------------------------------------------
DESCRIPTION: Register responsible to hold a request after the routing function
schedules an output channel to be used by an incoming packet. A registered 
request is hold until the packet trailer is delivered. It includes a circuitry
to set a circuit in order to reduce the latency for a communication flow
(the headers' hop latency is reduced from 3 cycles to 1 cycle)
--------------------------------------------------------------------------------
AUTHORS: Group of Embedded and Distribution System (GSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: June 1st, 2009.
--------------------------------------------------------------------------------
*/
#ifndef __REQ_REG_CS_H__
#define __REQ_REG_CS_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(req_reg_cs)
{
    // System signals
    sc_in_clk   clk; // Clock
    sc_in<bool> rst; // Reset

    // FIFO interface
    sc_in<bool>                 rok;    // FIFO has a data to be read (not empty)
    sc_in<bool>                 rd;     // Command to read a data from the FIFO
    sc_in<sc_uint<FLIT_WIDTH> > data;   // FIFO data output

    // Requests
    sc_in<bool>  in_reqL;    // Request to Lout (input)
    sc_in<bool>  in_reqN;    // Request to Nout (input)
    sc_in<bool>  in_reqE;    // Request to Eout (input)
    sc_in<bool>  in_reqS;    // Request to Sout (input)
    sc_in<bool>  in_reqW;    // Request to Wout (input)
    sc_out<bool> out_reqL;   // Request to Lout (output)
    sc_out<bool> out_reqN;	 // Request to Nout (output)
    sc_out<bool> out_reqE;   // Request to Eout (output)
    sc_out<bool> out_reqS;   // Request to Sout (output)
    sc_out<bool> out_reqW;	 // Request to Wout (output)
    sc_out<bool> requesting; // There exists someone requesting

    // Internal signals
    sc_signal<bool> reqL;         // Request to Lout
    sc_signal<bool> reqN;         // Request to Nout
    sc_signal<bool> reqE;         // Request to Eout
    sc_signal<bool> reqS;         // Request to Sout
    sc_signal<bool> reqW;         // Request to Wout
    sc_signal<bool> bop;          // Begin-of-Packet framing bit
    sc_signal<bool> cs_allocate;  // Command to allocate a circuit
    sc_signal<bool> cs_release;   // Command to release a circuit connection
    sc_signal<bool> circuit_set;  // Circuit is set
    sc_signal<bool> trailer_sent; // Trailer was sent

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    // Module's processes
    void p_reg();
    void p_internal_signals();
    void p_circuit_set();
    void p_outputs();
    void p_debug();

    SC_HAS_PROCESS(req_reg_cs);

    //////////////////////////////////////////////////////////////////////////////
    req_reg_cs(sc_module_name nm, unsigned short int XID, unsigned short int YID,
               unsigned short int PORT_ID): sc_module(nm), XID(XID), YID(YID),
        PORT_ID(PORT_ID)
      //////////////////////////////////////////////////////////////////////////////
    {
        SC_METHOD(p_internal_signals);
        sensitive << data << rok << rd << circuit_set << requesting;

        SC_METHOD(p_reg);
        sensitive << clk.pos() << rst;

        SC_METHOD(p_circuit_set);
        sensitive << clk.pos() << rst;

        SC_METHOD(p_outputs);
        sensitive << reqL << reqN << reqE << reqS << reqW;

#ifdef DEBUG_REQ_REG_CS
        SC_METHOD(p_debug);
        sensitive << clk.pos();
#endif
    }
};

#endif // __REQ_REG_CS_H__
