/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : ic (input controller)
FILE   : ic.h
--------------------------------------------------------------------------------
DESCRIPTION: Controller responsible to detect the header of an incoming packet, 
schedule an output channel to be requested (by running a routing algorithm), and 
hold the request until the packet trailer is delivered.
--------------------------------------------------------------------------------
AUTHORS: Group of Embedded and Distribution System (GSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: October 29th, 2006.
-----------------------------------------------------------------------------
*/
#ifndef __IC_H__
#define __IC_H__

#include <systemc.h>
#include "parameters.h"
#include "req_reg_cs.h"
#include "req_reg.h"
#include "routing_xy.h"
#include "routing_wf.h"
#include "routing_nf.h"
#include "routing_nl.h"
#include "routing_oe.h"

////////////////////////////////////////////////////////////////////////////////
SC_MODULE(ic)
////////////////////////////////////////////////////////////////////////////////
{
    // System signals
    sc_in_clk           clk;      	 	// Clock
    sc_in<bool>         rst;      	 	// Reset

    // FIFO interface
    sc_in<bool>         rd;             // command to read a data from the FIFO
    sc_in<bool>         rok;            // FIFO has a data to be read (not empty)
    sc_in<sc_uint<FLIT_WIDTH> > data;	// FIFO data output

    // Status of the output channels
    sc_in<bool>         Lidle;          // Lout is idle
    sc_in<bool>         Nidle; 	    	// Nout is idle
    sc_in<bool>         Eidle;  	  	// Eout is idle
    sc_in<bool>         Sidle;  	  	// Sout is idle
    sc_in<bool>         Widle;  	  	// Wout is idle

    // Requests
    sc_out<bool>        reqL;      		// Request to Lout
    sc_out<bool>        reqN;      		// Request to Nout
    sc_out<bool>        reqE;      		// Request to Eout
    sc_out<bool>        reqS;      		// Request to Sout
    sc_out<bool>        reqW;      		// Request to Wout
    sc_out<bool>        requesting; 	// There exists someone requesting

    // Data variables which implement connectors
    sc_signal<bool>	    reqL_wire;		// Request to Lout
    sc_signal<bool>	    reqN_wire;		// Request to Nout
    sc_signal<bool>	    reqE_wire;		// Request to Eout
    sc_signal<bool>	    reqS_wire;		// Request to Sout
    sc_signal<bool>	    reqW_wire;		// Request to Wout

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    SC_HAS_PROCESS(ic);

    //////////////////////////////////////////////////////////////////////////////
    ic(sc_module_name nm, unsigned short int XID, unsigned short int YID,
       unsigned short int PORT_ID) : sc_module(nm), XID(XID), YID(YID),
        PORT_ID(PORT_ID)
    //////////////////////////////////////////////////////////////////////////////
    {
        // Template for new routing algorithms
        // 	1. Copy the fllowing block
        //	2. Edit the IDnumber and the IDname
        //  Examples: IDnumber = 1
        //            IDname   = wf

        /*
    if (ROUTING_TYPE == IDnumber) {
      //////////////////////////////////////////////////////////////////////////
      routing_IDname *rIDname = new routing_IDname("routing_IDname", XID, YID);
      //////////////////////////////////////////////////////////////////////////
            rIDname->rok(rok);
      rIDname->data(data);
      rIDname->reqL(reqL_wire);
      rIDname->reqN(reqN_wire);
      rIDname->reqS(reqS_wire);
      rIDname->reqE(reqE_wire);
      rIDname->reqW(reqW_wire);
      rIDname->Lidle(Lidle);
      rIDname->Nidle(Nidle);
      rIDname->Eidle(Eidle);
      rIDname->Sidle(Sidle);
      rIDname->Widle(Widle);
    }
        */

        if (ROUTING_TYPE == 0) {
            //////////////////////////////////////////////////////////////////////////
            routing_xy *rxy = new routing_xy("routing_xy", XID, YID);
            //////////////////////////////////////////////////////////////////////////
            rxy->rok(rok);
            rxy->data(data);
            rxy->reqL(reqL_wire);
            rxy->reqN(reqN_wire);
            rxy->reqS(reqS_wire);
            rxy->reqE(reqE_wire);
            rxy->reqW(reqW_wire);
        }

        if (ROUTING_TYPE == 1) {
            //////////////////////////////////////////////////////////////////////////
            routing_wf *rwf = new routing_wf("routing_wf", XID, YID);
            //////////////////////////////////////////////////////////////////////////
            rwf->rok(rok);
            rwf->data(data);
            rwf->reqL(reqL_wire);
            rwf->reqN(reqN_wire);
            rwf->reqS(reqS_wire);
            rwf->reqE(reqE_wire);
            rwf->reqW(reqW_wire);
            rwf->Lidle(Lidle);
            rwf->Nidle(Nidle);
            rwf->Eidle(Eidle);
            rwf->Sidle(Sidle);
            rwf->Widle(Widle);
        }

        if (ROUTING_TYPE == 2) {
            //////////////////////////////////////////////////////////////////////////
            routing_nf *rnf = new routing_nf("routing_nf", XID, YID);
            //////////////////////////////////////////////////////////////////////////
            rnf->rok(rok);
            rnf->data(data);
            rnf->reqL(reqL_wire);
            rnf->reqN(reqN_wire);
            rnf->reqS(reqS_wire);
            rnf->reqE(reqE_wire);
            rnf->reqW(reqW_wire);
            rnf->Lidle(Lidle);
            rnf->Nidle(Nidle);
            rnf->Eidle(Eidle);
            rnf->Sidle(Sidle);
            rnf->Widle(Widle);
        }

        if (ROUTING_TYPE == 3) {
            //////////////////////////////////////////////////////////////////////////
            routing_nl *rnl = new routing_nl("routing_nl", XID, YID);
            //////////////////////////////////////////////////////////////////////////
            rnl->rok(rok);
            rnl->data(data);
            rnl->reqL(reqL_wire);
            rnl->reqN(reqN_wire);
            rnl->reqS(reqS_wire);
            rnl->reqE(reqE_wire);
            rnl->reqW(reqW_wire);
            rnl->Lidle(Lidle);
            rnl->Nidle(Nidle);
            rnl->Eidle(Eidle);
            rnl->Sidle(Sidle);
            rnl->Widle(Widle);
        }

        if (ROUTING_TYPE == 4) {
            //////////////////////////////////////////////////////////////////////////
            routing_oe *roe = new routing_oe("routing_oe", XID, YID);
            //////////////////////////////////////////////////////////////////////////
            roe->rok(rok);
            roe->data(data);
            roe->reqL(reqL_wire);
            roe->reqN(reqN_wire);
            roe->reqS(reqS_wire);
            roe->reqE(reqE_wire);
            roe->reqW(reqW_wire);
            roe->Lidle(Lidle);
            roe->Nidle(Nidle);
            roe->Eidle(Eidle);
            roe->Sidle(Sidle);
            roe->Widle(Widle);
        }


        ////////////////////////////////////////////////////////////////////////////
        //	  req_reg *reg = new req_reg("req_reg", XID, YID, PORT_ID);
        req_reg_cs *reg = new req_reg_cs("req_reg", XID, YID, PORT_ID);
        ////////////////////////////////////////////////////////////////////////////
        reg->clk(clk);
        reg->rst(rst);
        reg->rd(rd);
        reg->rok(rok);
        reg->data(data);
        reg->in_reqL(reqL_wire);
        reg->in_reqN(reqN_wire);
        reg->in_reqE(reqE_wire);
        reg->in_reqS(reqS_wire);
        reg->in_reqW(reqW_wire);
        reg->out_reqL(reqL);
        reg->out_reqN(reqN);
        reg->out_reqE(reqE);
        reg->out_reqS(reqS);
        reg->out_reqW(reqW);
        reg->requesting(requesting);
    }
};

#endif // __IC_H__
