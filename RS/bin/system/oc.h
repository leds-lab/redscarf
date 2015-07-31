/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : oc (output controller)
FILE   : oc.h
--------------------------------------------------------------------------------
DESCRIPTION: Controller responsible to schedule the use of the associated
output channel. It is based on an arbiter that receives requests and, based
on an arbitration algorithm, selects one request to be granted. A grant is
held at the high level while the request equals 1. 
--------------------------------------------------------------------------------
PARAMETERS:
# ARBITER_TYPE = (0=Round-Robin)(1=Static)(2=Rotative)(3=Random)
# N            >= 2 (number of bits in the priority genarator)
# XID		   >= 0
# YID		   >= 0
# PORT_ID	   = (0=L, 1=N, 2=E, 3=S or 4=W)
--------------------------------------------------------------------------------
NOTES: 
(1) N is is received by means of a #define (defined,by default, in parameters.h)
(2) XID, YID and PORT_ID were included for debugging purpose
--------------------------------------------------------------------------------
AUTHORS: Thiago Felski Pereira
         Cesar Albenes Zeferino
CONTACT: zeferino@univali.br OR cesar.zeferino@gmail.com
--------------------------------------------------------------------------------
Last update on: July 29th, 2006.
--------------------------------------------------------------------------------
*/
#ifndef __OC_H__
#define __OC_H__

#include <systemc.h>
#include "parameters.h"
#include "arbiter.h"

////////////////////////////////////////////////////////////////////////////////
SC_MODULE(oc)
////////////////////////////////////////////////////////////////////////////////
{
    // System signals
    sc_in_clk    clk;   	// Clock
    sc_in<bool>  rst;   	// Reset

    // Arbitration signals
    sc_in<bool>	 req0;	  // Request from the input channel 0
    sc_in<bool>	 req1;	  // Request from the input channel 1
    sc_in<bool>	 req2;	  // Request from the input channel 2
    sc_in<bool>	 req3;	  // Request from the input channel 3
    sc_out<bool> gnt0;	  // Grant to the input channel 0
    sc_out<bool> gnt1;	  // Grant to the input channel 1
    sc_out<bool> gnt2;	  // Grant to the input channel 2
    sc_out<bool> gnt3;	  // Grant to the input channel 3
    sc_out<bool> idle; 	  // Status

    // Internal signals
    sc_signal<sc_uint<N> > R_wire;	// Requests
    sc_signal<sc_uint<N> > G_wire;	// Grants

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    // Module's processes
    void p_request() {
        sc_uint<N> request;
        request[0] = req0.read();
        request[1] = req1.read();
        request[2] = req2.read();
        request[3] = req3.read();
        R_wire.write(request);
    }

    void p_grant() {
        sc_uint<N> grant;
        grant = G_wire.read();
        gnt0.write(grant[0]);
        gnt1.write(grant[1]);
        gnt2.write(grant[2]);
        gnt3.write(grant[3]);
    }

    SC_HAS_PROCESS(oc);

    //////////////////////////////////////////////////////////////////////////////
    oc(sc_module_name nm, unsigned short int XID, unsigned short int YID,
       unsigned short int PORT_ID) : sc_module(nm), XID(XID), YID(YID),
        PORT_ID(PORT_ID)
    //////////////////////////////////////////////////////////////////////////////
    {
        SC_METHOD(p_request);
        sensitive << req0 << req1 << req2 << req3;

        SC_METHOD(p_grant);
        sensitive << G_wire;

        arbiter *arbiter0 = new arbiter("arbiter",XID,YID,PORT_ID);
        arbiter0->clk(clk);
        arbiter0->rst(rst);
        arbiter0->R(R_wire);
        arbiter0->G(G_wire);
        arbiter0->idle(idle);

    }
};

#endif // __OC_H__
