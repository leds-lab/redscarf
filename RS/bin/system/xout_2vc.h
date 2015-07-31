/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : XOU-2VC (2-VCs output channel)
FILE   : xout_2vc.h
--------------------------------------------------------------------------------
DESCRIPTION: Output channel module for the generation of the output circuitry 
for each router port.It includes two virtual channels which share the physical
output channel.
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distribution System (LSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: May 20th, 2009.
--------------------------------------------------------------------------------
*/
#ifndef __XOUT_2VC_H__
#define __XOUT_2VC_H__

#include <systemc.h>
#include "parameters.h"
#include "xout.h"
#include "xout_ag.h"

SC_MODULE(xout_2vc)
{
    // System signals
    sc_in_clk    clk;   // Clock
    sc_in<bool>  rst;   // Reset

    // Link signals
    sc_out<sc_uint<FLIT_WIDTH> >    out_data;   // Output data channel
    sc_out<bool>                    vc0_out_val;// Data validation
    sc_in<bool>                     vc0_out_ret;// Return (ack or credit)
    sc_out<bool>                    vc1_out_val;// Data validation
    sc_in<bool>                     vc1_out_ret;// Return (ack or credit)

    // Commands and status signals interconnecting input and output channels for VC0
    sc_in<bool>                 vc0_x_req0; // Request from input channel 0
    sc_in<bool>                 vc0_x_req1;	// Request from input channel 1
    sc_in<bool>                 vc0_x_req2;	// Request from input channel 2
    sc_in<bool>                 vc0_x_req3;	// Request from input channel 3
    sc_in<bool>                 vc0_x_rok0;	// Rok from input channel0
    sc_in<bool>                 vc0_x_rok1;	// Rok from input channel1
    sc_in<bool>                 vc0_x_rok2;	// Rok from input channel2
    sc_in<bool>                 vc0_x_rok3;	// Rok from input channel3
    sc_out<bool>                vc0_x_gnt0;	// Grant to input channel 0
    sc_out<bool>                vc0_x_gnt1;	// Grant to input channel 1
    sc_out<bool>                vc0_x_gnt2;	// Grant to input channel 2
    sc_out<bool>                vc0_x_gnt3;	// Grant to input channel 3
    sc_out<bool>                vc0_x_idle;	// Status to the input channels
    sc_out<bool>                vc0_x_rd;	// Rd command to the input channels
    sc_in<sc_uint<FLIT_WIDTH> > vc0_x_din0; // Data from input channel 0
    sc_in<sc_uint<FLIT_WIDTH> > vc0_x_din1; // Data from input channel 1
    sc_in<sc_uint<FLIT_WIDTH> > vc0_x_din2; // Data from input channel 2
    sc_in<sc_uint<FLIT_WIDTH> > vc0_x_din3; // Data from input channel 3

    // Commands and status signals interconnecting input and output channels for VC1
    sc_in<bool> 				vc1_x_req0; // Request from input channel 0
    sc_in<bool> 				vc1_x_req1; // Request from input channel 1
    sc_in<bool> 				vc1_x_req2;	// Request from input channel 2
    sc_in<bool>                 vc1_x_req3;	// Request from input channel 3
    sc_in<bool>                 vc1_x_rok0;	// Rok from input channel0
    sc_in<bool>                 vc1_x_rok1;	// Rok from input channel1
    sc_in<bool>                 vc1_x_rok2;	// Rok from input channel2
    sc_in<bool>                 vc1_x_rok3;	// Rok from input channel3
    sc_out<bool>                vc1_x_gnt0;	// Grant to input channel 0
    sc_out<bool>                vc1_x_gnt1;	// Grant to input channel 1
    sc_out<bool>                vc1_x_gnt2;	// Grant to input channel 2
    sc_out<bool>                vc1_x_gnt3;	// Grant to input channel 3
    sc_out<bool>                vc1_x_idle;	// Status to the input channels
    sc_out<bool>                vc1_x_rd;	// Rd command to the input channels
    sc_in<sc_uint<FLIT_WIDTH> > vc1_x_din0; // Data from input channel 0
    sc_in<sc_uint<FLIT_WIDTH> > vc1_x_din1; // Data from input channel 1
    sc_in<sc_uint<FLIT_WIDTH> > vc1_x_din2; // Data from input channel 2
    sc_in<sc_uint<FLIT_WIDTH> > vc1_x_din3; // Data from input channel 3

    // Internal signals
    sc_signal<sc_uint<FLIT_WIDTH> > vc0_out_data_wire; 	// VC0 Output data channel
    sc_signal<sc_uint<FLIT_WIDTH> > vc1_out_data_wire;	// VC1 Output data channel

    // Internal data structures
    unsigned short int USE_THIS, XID, YID, PORT_ID;

    // Module's processes
    void p_null() {
        out_data.write(0);

        vc0_out_val.write(0);
        vc0_x_gnt0.write(0);
        vc0_x_gnt1.write(0);
        vc0_x_gnt2.write(0);
        vc0_x_gnt3.write(0);
        vc0_x_idle.write(0);
        vc0_x_rd.write(0);

        vc1_out_val.write(0);
        vc1_x_gnt0.write(0);
        vc1_x_gnt1.write(0);
        vc1_x_gnt2.write(0);
        vc1_x_gnt3.write(0);
        vc1_x_idle.write(0);
        vc1_x_rd.write(0);
    };


    // Module's processes
    void p_sel_channel() {
        if (vc0_out_val.read()) {
            out_data.write(vc0_out_data_wire.read());
        } else {
            out_data.write(vc1_out_data_wire.read());
        }
    };

    SC_HAS_PROCESS(xout_2vc);

    //////////////////////////////////////////////////////////////////////////////
    xout_2vc(sc_module_name nm,	unsigned short int USE_THIS, unsigned short int XID,
             unsigned short int YID, unsigned short int PORT_ID)	:
        sc_module(nm), USE_THIS(USE_THIS), XID(XID), YID(YID), PORT_ID(PORT_ID)
      //////////////////////////////////////////////////////////////////////////////
    {
        if (USE_THIS) {
            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////
            // It defines the architecture of VC 0
            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////
            if (PARIS_VC0_TYPE == PARIS_CHANNEL_NULL) {
            }

            else if (PARIS_VC0_TYPE == PARIS_CHANNEL_DEFAULT) {
                ////////////////////////////////////////////////////////////////////////
                xout *vc0 = new xout("vc0", USE_THIS, XID, YID, PORT_ID);//CHANNEL_ID=0
                ////////////////////////////////////////////////////////////////////////
                vc0->clk(clk);
                vc0->rst(rst);
                vc0->out_data(vc0_out_data_wire);
                vc0->out_val(vc0_out_val);
                vc0->out_ret(vc0_out_ret);
                vc0->hold_send(rst);			// it emulates a stuck at ground after reset
                vc0->x_req0(vc0_x_req0);
                vc0->x_req1(vc0_x_req1);
                vc0->x_req2(vc0_x_req2);
                vc0->x_req3(vc0_x_req3);
                vc0->x_rok0(vc0_x_rok0);
                vc0->x_rok1(vc0_x_rok1);
                vc0->x_rok2(vc0_x_rok2);
                vc0->x_rok3(vc0_x_rok3);
                vc0->x_gnt0(vc0_x_gnt0);
                vc0->x_gnt1(vc0_x_gnt1);
                vc0->x_gnt2(vc0_x_gnt2);
                vc0->x_gnt3(vc0_x_gnt3);
                vc0->x_idle(vc0_x_idle);
                vc0->x_rd	 (vc0_x_rd  );
                vc0->x_din0(vc0_x_din0);
                vc0->x_din1(vc0_x_din1);
                vc0->x_din2(vc0_x_din2);
                vc0->x_din3(vc0_x_din3);
            }

            else if (PARIS_VC0_TYPE == PARIS_CHANNEL_CS) {
                printf("\n [XOUT-2VC]: Circuit-switching architecture not implemented for VC0");
            }

            else if (PARIS_VC0_TYPE == PARIS_CHANNEL_AG) {
                ////////////////////////////////////////////////////////////////////////
                xout_ag *vc0 = new xout_ag("vc0",USE_THIS, XID, YID, PORT_ID, 0);
                ////////////////////////////////////////////////////////////////////////
                vc0->clk(clk);
                vc0->rst(rst);
                vc0->out_data(vc0_out_data_wire);
                vc0->out_val(vc0_out_val);
                vc0->out_ret(vc0_out_ret);
                vc0->hold_send(rst);
                vc0->x_req0(vc0_x_req0);
                vc0->x_req1(vc0_x_req1);
                vc0->x_req2(vc0_x_req2);
                vc0->x_req3(vc0_x_req3);
                vc0->x_rok0(vc0_x_rok0);
                vc0->x_rok1(vc0_x_rok1);
                vc0->x_rok2(vc0_x_rok2);
                vc0->x_rok3(vc0_x_rok3);
                vc0->x_gnt0(vc0_x_gnt0);
                vc0->x_gnt1(vc0_x_gnt1);
                vc0->x_gnt2(vc0_x_gnt2);
                vc0->x_gnt3(vc0_x_gnt3);
                vc0->x_idle(vc0_x_idle);
                vc0->x_rd  (vc0_x_rd  );
                vc0->x_din0(vc0_x_din0);
                vc0->x_din1(vc0_x_din1);
                vc0->x_din2(vc0_x_din2);
                vc0->x_din3(vc0_x_din3);

            }

            else
                printf("\n [XOUT-2VC]: None architecture defined for VC0");


            ////////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////////
            // It defines the architecture of VC 1
            ////////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////////
            if (PARIS_VC1_TYPE == PARIS_CHANNEL_NULL) {
            }

            else if ((PARIS_VC1_TYPE == PARIS_CHANNEL_DEFAULT)  ||
                     (PARIS_VC1_TYPE == PARIS_CHANNEL_AG)){
                ////////////////////////////////////////////////////////////////////////
                xout *vc1 = new xout("vc1", USE_THIS, XID, YID, PORT_ID);//CHANNEL_ID=1
                ////////////////////////////////////////////////////////////////////////
                vc1->clk(clk);
                vc1->rst(rst);
                vc1->out_data(vc1_out_data_wire);
                vc1->out_val(vc1_out_val);
                vc1->out_ret(vc1_out_ret);
                vc1->hold_send(vc0_out_val);
                vc1->x_req0(vc1_x_req0);
                vc1->x_req1(vc1_x_req1);
                vc1->x_req2(vc1_x_req2);
                vc1->x_req3(vc1_x_req3);
                vc1->x_rok0(vc1_x_rok0);
                vc1->x_rok1(vc1_x_rok1);
                vc1->x_rok2(vc1_x_rok2);
                vc1->x_rok3(vc1_x_rok3);
                vc1->x_gnt0(vc1_x_gnt0);
                vc1->x_gnt1(vc1_x_gnt1);
                vc1->x_gnt2(vc1_x_gnt2);
                vc1->x_gnt3(vc1_x_gnt3);
                vc1->x_idle(vc1_x_idle);
                vc1->x_rd  (vc1_x_rd  );
                vc1->x_din0(vc1_x_din0);
                vc1->x_din1(vc1_x_din1);
                vc1->x_din2(vc1_x_din2);
                vc1->x_din3(vc1_x_din3);
            }

            else if (PARIS_VC1_TYPE == PARIS_CHANNEL_CS) {
                printf("\n [XOUT-2VC]: Circuit-switching architecture not implemented for VC1");
            }

            else if (PARIS_VC1_TYPE == PARIS_CHANNEL_AG) {
                ////////////////////////////////////////////////////////////////////////
                xout_ag *vc1 = new xout_ag("vc1",USE_THIS, XID, YID, PORT_ID, 1);
                ////////////////////////////////////////////////////////////////////////
                vc1->clk(clk);
                vc1->rst(rst);
                vc1->out_data(vc1_out_data_wire);
                vc1->out_val(vc1_out_val);
                vc1->out_ret(vc1_out_ret);
                vc1->hold_send(vc0_out_val);
                vc1->x_req0(vc1_x_req0);
                vc1->x_req1(vc1_x_req1);
                vc1->x_req2(vc1_x_req2);
                vc1->x_req3(vc1_x_req3);
                vc1->x_rok0(vc1_x_rok0);
                vc1->x_rok1(vc1_x_rok1);
                vc1->x_rok2(vc1_x_rok2);
                vc1->x_rok3(vc1_x_rok3);
                vc1->x_gnt0(vc1_x_gnt0);
                vc1->x_gnt1(vc1_x_gnt1);
                vc1->x_gnt2(vc1_x_gnt2);
                vc1->x_gnt3(vc1_x_gnt3);
                vc1->x_idle(vc1_x_idle);
                vc1->x_rd  (vc1_x_rd  );
                vc1->x_din0(vc1_x_din0);
                vc1->x_din1(vc1_x_din1);
                vc1->x_din2(vc1_x_din2);
                vc1->x_din3(vc1_x_din3);
            }

            else
                printf("\n [XOUT-2VC]: None architecture defined for VC1");


            SC_METHOD(p_sel_channel);
            sensitive << vc0_out_val
                      << vc0_out_data_wire
                      << vc1_out_data_wire;

        } else { // IF DO NOT USE THIS
            SC_METHOD(p_null);
            sensitive << clk.pos();
        }
    }
};

#endif // __XOUT_2VC_H__


