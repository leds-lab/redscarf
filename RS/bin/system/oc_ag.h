/*******************************************************************************

 Company: 	UNIVALI
 Engineer:	Thiago Felski Pereira
 
 Create Date:	21/05/2009
 Design Name:	oc_ag
 Module Name:   oc_ag.h
 Project Name:
 Target Devices:
 Tool versions:
 Description:

 Dependencies:

 Revision:
 Revision 0.01 - File Created
 Additional Comments:

*/
#ifndef __OC_AG_H__
#define __OC_AG_H__

#include <systemc.h>
#include "parameters.h"
#include "arb_ag.h"
#include "stamp_checker.h"
#include "class_checker.h"
#include "ppe.h"

////////////////////////////////////////////////////////////////////////////////
SC_MODULE(oc_ag)
////////////////////////////////////////////////////////////////////////////////
{
    // System signals
    sc_in_clk    clk;   // Clock
    sc_in<bool>  rst;   // Reset

    // Arbitration signals
    sc_in<bool>	 req0;  // Request from the input channel 0
    sc_in<bool>	 req1;  // Request from the input channel 1
    sc_in<bool>	 req2;  // Request from the input channel 2
    sc_in<bool>	 req3;  // Request from the input channel 3

    sc_in<sc_uint<FLIT_WIDTH> > i_DIN_0;
    sc_in<sc_uint<FLIT_WIDTH> >	i_DIN_1;
    sc_in<sc_uint<FLIT_WIDTH> >	i_DIN_2;
    sc_in<sc_uint<FLIT_WIDTH> >	i_DIN_3;

    sc_in<bool>	i_ROK_0;
    sc_in<bool>	i_ROK_1;
    sc_in<bool> i_ROK_2;
    sc_in<bool>	i_ROK_3;

    sc_out<bool> gnt0;  // Grant to the input channel 0
    sc_out<bool> gnt1;  // Grant to the input channel 1
    sc_out<bool> gnt2;  // Grant to the input channel 2
    sc_out<bool> gnt3;  // Grant to the input channel 3
    sc_out<bool> idle;  // Status

    // Internal signals
    sc_signal<sc_uint<N> > R_wire;	// Requests
    sc_signal<sc_uint<N> > G_wire;	// Grants

    // Internal data structures
    unsigned short int XID, YID, PORT_ID, CHANNEL_ID;

    // Module's processes
    void p_debug() {
        printf(" \n[oc_ag]:");
        printf(" - req[3..0] = %d%d%d%d", req3.read(),req2.read(),req1.read(),req0.read());
        printf(" - rok[3..0] = %d%d%d%d", i_ROK_3.read(),i_ROK_2.read(),i_ROK_1.read(),i_ROK_0.read());
        printf(" - din0 = %#X", (unsigned int) i_DIN_0.read());
        printf(" - din1 = %#X", (unsigned int) i_DIN_1.read());
        printf(" - din2 = %#X", (unsigned int) i_DIN_2.read());
        printf(" - din3 = %#X", (unsigned int) i_DIN_3.read());
        printf(" --- gnt[3..0] = %d%d%d%d", gnt3.read(),gnt2.read(),gnt1.read(),gnt0.read());
        printf(" - G = 0x%X", (unsigned int) G_wire.read());
    }

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

    SC_HAS_PROCESS(oc_ag);

    //////////////////////////////////////////////////////////////////////////////
    oc_ag(sc_module_name nm, unsigned short int XID, unsigned short int YID,
          unsigned short int PORT_ID, unsigned short int CHANNEL_ID) :
        sc_module(nm), XID(XID), YID(YID), PORT_ID(PORT_ID),
        CHANNEL_ID(CHANNEL_ID)
    //////////////////////////////////////////////////////////////////////////////
    {
        SC_METHOD(p_request);
        sensitive << req0 << req1 << req2 << req3;

        SC_METHOD(p_grant);
        sensitive << G_wire;

#ifdef DEBUG_OC_AG
        SC_METHOD(p_debug);
        sensitive << clk.pos();
#endif

        //    if (ARBITER_TYPE == 1) {

        ///////////////////////////////////////////////////////////////////
        arb_ag *arb_ag0 = new arb_ag("arb_ag",XID,YID,PORT_ID,CHANNEL_ID);
        ///////////////////////////////////////////////////////////////////
        arb_ag0->i_CLK(clk);
        arb_ag0->i_RST(rst);
        arb_ag0->i_REQ(R_wire);

        arb_ag0->i_DIN_0(i_DIN_0);
        arb_ag0->i_DIN_1(i_DIN_1);
        arb_ag0->i_DIN_2(i_DIN_2);
        arb_ag0->i_DIN_3(i_DIN_3);

        arb_ag0->i_ROK_0(i_ROK_0);
        arb_ag0->i_ROK_1(i_ROK_1);
        arb_ag0->i_ROK_2(i_ROK_2);
        arb_ag0->i_ROK_3(i_ROK_3);

        arb_ag0->o_GNT(G_wire);
        arb_ag0->o_IDLE(idle);
        //    }
    }
};

#endif // __OC_AG_H__
