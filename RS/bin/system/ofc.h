/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : ofc (output flow controller )
FILE	 : ofc.h
--------------------------------------------------------------------------------
DESCRIPTION: Controller responsible to regulate the flow of flits at the 
input channels. It makes the adapting between the link flow control protocol
(eg.credit-based, handshake) and the internal flow control protocol (FIFO).FO).
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distribution System (LSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: August 21th, 2006.
--------------------------------------------------------------------------------

*/
#ifndef __OFC_H__
#define __OFC_H__

#include <systemc.h>
#include "ofc_handshake.h"
#include "ofc_credit.h"
#include "ofc_credit_vct.h"
#include "parameters.h"

SC_MODULE(ofc)
{
    // System signals
    sc_in_clk     clk;  // clock
    sc_in<bool>   rst;  // reset

    // Link interface
    sc_out<bool>  val;  // data validation
    sc_in<bool>   ret;	// return (credit or acknowledgement)

    // FIFO interface
    sc_out<bool>  rd;   // command to read a data from the FIFO
    sc_in<bool>   rok;  // FIFO has a data to be read (not empty)

    // There is no signal

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    SC_HAS_PROCESS(ofc);

    //////////////////////////////////////////////////////////////////////////////
    ofc(sc_module_name nm,	unsigned short int XID, unsigned short int YID,
        unsigned short int PORT_ID) : sc_module(nm),  XID(XID), YID(YID),
        PORT_ID(PORT_ID)
    //////////////////////////////////////////////////////////////////////////////
    {
        if (FC_TYPE == 0) {
            ////////////////////////////////////////////////////////////////////////
            ofc_handshake *ofc0= new ofc_handshake("ofc_handshake",XID,YID,PORT_ID);
            ////////////////////////////////////////////////////////////////////////
            ofc0->clk(clk);
            ofc0->rst(rst);
            ofc0->val(val);
            ofc0->ret(ret);
            ofc0->rd(rd);
            ofc0->rok(rok);

        } else {
            if (FC_TYPE == 1) {
                ////////////////////////////////////////////////////////////////
                ofc_credit *ofc1 = new ofc_credit("ofc_credit",XID,YID,PORT_ID);
                ////////////////////////////////////////////////////////////////
                ofc1->clk(clk);
                ofc1->rst(rst);
                ofc1->val(val);
                ofc1->ret(ret);
                ofc1->rd(rd);
                ofc1->rok(rok);
            } else { // FC_TYPE == 2
                ////////////////////////////////////////////////////////////////////////////
                ofc_credit_vct *ofc2 = new ofc_credit_vct("ofc_credit_vct",XID,YID,PORT_ID);
                ////////////////////////////////////////////////////////////////////////////
                ofc2->clk(clk);
                ofc2->rst(rst);
                ofc2->val(val);
                ofc2->ret(ret);
                ofc2->rd(rd);
                ofc2->rok(rok);
            }
        }
    }
};

#endif // __OFC__
