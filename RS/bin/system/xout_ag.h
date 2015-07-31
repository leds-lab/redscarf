/*******************************************************************************

 Company: 				UNIVALI
 Engineer:				Thiago Felski Pereira
 
 Create Date:			22/05/2009
 Design Name:			xout_ag
 Module Name:    	xout_ag.h
 Project Name:
 Target Devices:
 Tool versions:
 Description:

 Dependencies:

 Revision:
 Revision 0.01 - File Created
 Additional Comments:

*******************************************************************************/

#ifndef __XOUT_AG_H__
#define __XOUT_AG_H__

#include <systemc.h>
#include "parameters.h"
#include "fifo_out.h"
#include "oc_ag.h"
#include "ofc.h"
#include "ows.h"
#include "ods.h"

SC_MODULE(xout_ag)
{
    // System signals
    sc_in_clk    clk;   // Clock
    sc_in<bool>  rst;   // Reset

    // Link signals
    sc_out<sc_uint<FLIT_WIDTH> >    out_data;   // Output data channel
    sc_out<bool>                    out_val;    // Data validation
    sc_in<bool>                     out_ret;    // Return (ack or credit)

    // Commands and status signals interconnecting input and output channels
    sc_in<bool>                 x_req0; // Request from input channel 0
    sc_in<bool>                 x_req1;	// Request from input channel 1
    sc_in<bool>                 x_req2;	// Request from input channel 2
    sc_in<bool>                 x_req3;	// Request from input channel 3
    sc_in<bool>                 x_rok0;	// Rok from input channel0
    sc_in<bool>                 x_rok1;	// Rok from input channel1
    sc_in<bool>                 x_rok2;	// Rok from input channel2
    sc_in<bool>                 x_rok3;	// Rok from input channel3

    sc_in<bool>                 hold_send;

    sc_out<bool>                x_gnt0;	// Grant to input channel 0
    sc_out<bool>                x_gnt1;	// Grant to input channel 1
    sc_out<bool>                x_gnt2;	// Grant to input channel 2
    sc_out<bool>                x_gnt3;	// Grant to input channel 3
    sc_out<bool>                x_idle;	// Status to the input channels
    sc_out<bool>                x_rd;	// Rd command to the input channels
    sc_in<sc_uint<FLIT_WIDTH> > x_din0; // Data from input channel 0
    sc_in<sc_uint<FLIT_WIDTH> > x_din1; // Data from input channel 1
    sc_in<sc_uint<FLIT_WIDTH> > x_din2; // Data from input channel 2
    sc_in<sc_uint<FLIT_WIDTH> > x_din3; // Data from input channel 3

    // Internal signals
    sc_signal<sc_uint<FLIT_WIDTH> > din_wire;   // FIFO data input
    sc_signal<bool>                 rok_wire;   // FIFO has a data to be read

    sc_signal<bool>                 rok2_wire;

    sc_signal<bool>                 wok_wire;   // FIFO has room to be written
    sc_signal<bool>                 rd_wire;    // Cmd to rd  a data from the FIFO
    sc_signal<bool>					wr_wire;    // Cmd to wr a data into the FIFO

    // Internal data structures
    unsigned short int USE_THIS, XID, YID, PORT_ID, CHANNEL_ID;

    // Module's processes
    void p_null() {
        out_val.write(0);
        out_data.write(0);
        x_rd.write(0);
        x_gnt0.write(0);
        x_gnt1.write(0);
        x_gnt2.write(0);
        x_gnt3.write(0);
        x_idle.write(0);
    };

    // rok_wire controlled by the signal used  to pause the sending of flits
    void p_hold_send() {
        rok2_wire.write(rok_wire.read() and (not hold_send.read()));
    };

    SC_HAS_PROCESS(xout_ag);

    //////////////////////////////////////////////////////////////////////////////
    xout_ag(sc_module_name nm,	unsigned short int USE_THIS,
            unsigned short int XID, unsigned short int YID,
            unsigned short int PORT_ID, unsigned short int CHANNEL_ID) :
        sc_module(nm), USE_THIS(USE_THIS), XID(XID), YID(YID),
        PORT_ID(PORT_ID), CHANNEL_ID(CHANNEL_ID)
    //////////////////////////////////////////////////////////////////////////////
    {
        if (USE_THIS) {

            SC_METHOD(p_hold_send);
            sensitive << rok_wire
                      << hold_send;

            //////////////////////////////////////////////////////////
            fifo_out *fifo0 = new fifo_out("fifo", XID, YID, PORT_ID);
            //////////////////////////////////////////////////////////
            fifo0->clk(clk);
            fifo0->rst(rst);
            fifo0->rok(rok_wire);
            fifo0->wok(x_rd);
            fifo0->rd(rd_wire);
            fifo0->wr(wr_wire);
            fifo0->din(din_wire);
            fifo0->dout(out_data);

            //////////////////////////////////////////////////////////////////
            oc_ag *oc_ag0 = new oc_ag("oc_ag", XID, YID, PORT_ID, CHANNEL_ID);
            //////////////////////////////////////////////////////////////////
            oc_ag0->clk(clk);
            oc_ag0->rst(rst);
            oc_ag0->req0(x_req0);
            oc_ag0->req1(x_req1);
            oc_ag0->req2(x_req2);
            oc_ag0->req3(x_req3);
            oc_ag0->i_DIN_0(x_din0);
            oc_ag0->i_DIN_1(x_din1);
            oc_ag0->i_DIN_2(x_din2);
            oc_ag0->i_DIN_3(x_din3);
            oc_ag0->i_ROK_0(x_rok0);
            oc_ag0->i_ROK_1(x_rok1);
            oc_ag0->i_ROK_2(x_rok2);
            oc_ag0->i_ROK_3(x_rok3);
            oc_ag0->gnt0(x_gnt0);
            oc_ag0->gnt1(x_gnt1);
            oc_ag0->gnt2(x_gnt2);
            oc_ag0->gnt3(x_gnt3);
            oc_ag0->idle(x_idle);


            //////////////////////////////////////////////
            ofc *ofc0 = new ofc("ofc", XID, YID, PORT_ID);
            //////////////////////////////////////////////
            ofc0->clk(clk);
            ofc0->rst(rst);
            ofc0->val(out_val);
            ofc0->ret(out_ret);
            ofc0->rok(rok2_wire);
            ofc0->rd(rd_wire);

            ///////////////////////////
            ows *ows0 = new ows("ows");
            ///////////////////////////
            ows0->gnt0(x_gnt0);
            ows0->gnt1(x_gnt1);
            ows0->gnt2(x_gnt2);
            ows0->gnt3(x_gnt3);
            ows0->wr_in0(x_rok0);
            ows0->wr_in1(x_rok1);
            ows0->wr_in2(x_rok2);
            ows0->wr_in3(x_rok3);
            ows0->wr_out(wr_wire);

            ///////////////////////////
            ods *ods0 = new ods("ods");
            ///////////////////////////
            ods0->gnt0(x_gnt0);
            ods0->gnt1(x_gnt1);
            ods0->gnt2(x_gnt2);
            ods0->gnt3(x_gnt3);
            ods0->din0(x_din0);
            ods0->din1(x_din1);
            ods0->din2(x_din2);
            ods0->din3(x_din3);
            ods0->dout(din_wire);

        } else { // DON'T USE THIS
            SC_METHOD(p_null);
            sensitive << clk.pos();
        }
    }
};

#endif // __XOUT_AG_H__

