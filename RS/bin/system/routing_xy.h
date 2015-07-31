/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : routing_xy
FILE   : routing_xy.h
--------------------------------------------------------------------------------
DESCRIPTION: Implements the XY routing algorithm. It takes as parameters the
router's coordinates and compares them with the destination router's coordi-
specified on the header's RIB field.
--------------------------------------------------------------------------------
AUTHORS: Group of Embedded and Distribution System (GSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: October 29th, 2006.
--------------------------------------------------------------------------------
*/
#ifndef __ROUTING_XY_H__
#define __ROUTING_XY_H__

#include <systemc.h>
#include "parameters.h"

////////////////////////////////////////////////////////////////////////////////
SC_MODULE(routing_xy)
////////////////////////////////////////////////////////////////////////////////
{
    // FIFO interface
    sc_in<bool>                 rok; // FIFO has a data to be read (not empty)
    sc_in<sc_uint<FLIT_WIDTH> > data;// FIFO data output

    // Requests
    sc_out<bool> reqL;  // Request to Lout
    sc_out<bool> reqN;  // Request to Nout
    sc_out<bool> reqE;  // Request to Eout
    sc_out<bool> reqS;  // Request to Sout
    sc_out<bool> reqW;  // Request to Wout

    // Internal data structures
    unsigned short int XID, YID;

    // Module's processes
    void p_req();

    SC_HAS_PROCESS(routing_xy);

    //////////////////////////////////////////////////////////////////////////////
    routing_xy(sc_module_name nm, unsigned short int XID, unsigned short int YID):
        sc_module(nm), XID(XID), YID(YID)
    //////////////////////////////////////////////////////////////////////////////
    {
        SC_METHOD(p_req);
        sensitive << data << rok;
    }
};

#endif // __ROUTING_XY_H__
