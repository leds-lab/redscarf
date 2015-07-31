/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : routing_oe
FILE   : routing_oe.h
--------------------------------------------------------------------------------
DESCRIPTION: Implements the OE routing algorithm. It takes as parameters the
router's coordinates and compares them with the destination router's coordinates
specified on the header's RIB field.
--------------------------------------------------------------------------------
AUTHORS: Group of Embedded and Distribution System (GSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: October 29th, 2006.
--------------------------------------------------------------------------------
*/
#ifndef __ROUTING_OE_H__
#define __ROUTING_OE_H__

#include <systemc.h>
#include "parameters.h"

////////////////////////////////////////////////////////////////////////////////
SC_MODULE(routing_oe)
////////////////////////////////////////////////////////////////////////////////
{
    // FIFO interface
    sc_in<bool>                 rok;    // FIFO has a data to be read (not empty)
    sc_in<sc_uint<FLIT_WIDTH> > data;   // FIFO data output

    // Status of the output channels
    sc_in<bool> Lidle; // Lout is idle
    sc_in<bool> Nidle; // Nout is idle
    sc_in<bool> Eidle; // Eout is idle
    sc_in<bool> Sidle; // Sout is idle
    sc_in<bool> Widle; // Wout is idle

    // Requests
    sc_out<bool> reqL;  // Request to Lout
    sc_out<bool> reqN;  // Request to Nout
    sc_out<bool> reqE;  // Request to Eout
    sc_out<bool> reqS;  // Request to Sout
    sc_out<bool> reqW;  // Request to Wout

    // Internal data structures
    unsigned short int XID, YID;        // Node's coordinates

    // Module's process
    void p_req();

    SC_HAS_PROCESS(routing_oe);

    //////////////////////////////////////////////////////////////////////////////
    routing_oe(sc_module_name nm, unsigned short int XID, unsigned short int YID):
        sc_module(nm), XID(XID), YID(YID)
    //////////////////////////////////////////////////////////////////////////////
    {
        SC_METHOD(p_req);
        sensitive << data << rok << Lidle << Nidle << Eidle << Sidle << Widle;
    }
};
#endif // __ROUTING_OE_H__
