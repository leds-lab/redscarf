/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : ifc (input flow controller)
--------------------------------------------------------------------------------
DESCRIPTION: Controller responsible to regulate the flow of flits at the 
input channels. It makes the adapting between the link flow control protocol
(eg.credit-based, handshake) and the internal flow control protocol (FIFO).
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distribution System (LSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: July 24th, 2006.
--------------------------------------------------------------------------------
*/
#ifndef __IFC_H__
#define __IFC_H__

#include <systemc.h>
#include "parameters.h"
#include "ifc_handshake.h"
#include "ifc_credit.h"
#include "ifc_credit_vct.h"

SC_MODULE(ifc)
{
    // System signals
    sc_in_clk     clk;                  // clock
    sc_in<bool>   rst;                  // reset

    // Link interface
    sc_in<bool>   val;                  // data validation
    sc_out<bool>  ret;                  // return (credit or acknowledgement)

    // FIFO interface
    sc_out<bool>  wr;                   // command to write a data into de FIFO
    sc_in<bool>   wok;                  // FIFO has room to be written (not full)
    sc_in<bool>   rd;                   // command to read a data from the FIFO
    sc_in<bool>   rok;                  // FIFO has a data to be read (not empty)
    sc_in<sc_uint<FLIT_WIDTH> > data;   // FIFO data output
    // There is no signal

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    SC_HAS_PROCESS(ifc);

    //////////////////////////////////////////////////////////////////////////////
    ifc(sc_module_name nm, unsigned short int XID, unsigned short int YID,
        unsigned short int PORT_ID) : sc_module(nm),  XID(XID), YID(YID),
        PORT_ID(PORT_ID)
    //////////////////////////////////////////////////////////////////////////////
    {
        if (FC_TYPE == 0) {
            /////////////////////////////////////////////////////////////////////////
            ifc_handshake *ifc0 = new ifc_handshake("ifc_handshake",XID,YID,PORT_ID);
            /////////////////////////////////////////////////////////////////////////
            ifc0->clk(clk);
            ifc0->rst(rst);
            ifc0->val(val);
            ifc0->ret(ret);
            ifc0->wr(wr);
            ifc0->wok(wok);

        } else {
            if (FC_TYPE == 1) {
                ////////////////////////////////////////////////////////////////
                ifc_credit *ifc1 = new ifc_credit("ifc_credit",XID,YID,PORT_ID);
                ////////////////////////////////////////////////////////////////
                ifc1->clk(clk);
                ifc1->rst(rst);
                ifc1->val(val);
                ifc1->ret(ret);
                ifc1->wr(wr);
                ifc1->rd(rd);
                ifc1->rok(rok);

            } else {// FC_TYPE = 2
                ////////////////////////////////////////////////////////////////////////////
                ifc_credit_vct *ifc2 = new ifc_credit_vct("ifc_credit_vct",XID,YID,PORT_ID);
                ////////////////////////////////////////////////////////////////////////////
                ifc2->clk(clk);
                ifc2->rst(rst);
                ifc2->val(val);
                ifc2->ret(ret);
                ifc2->wr(wr);
                ifc2->rd(rd);
                ifc2->rok(rok);
                ifc2->data(data);
            }
        }
    }
};

#endif // __IFC__
