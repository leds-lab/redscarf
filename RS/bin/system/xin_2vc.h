/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : XIN-2VC (2-VCs input channel)
FILE   : xin_2vc.h
--------------------------------------------------------------------------------
DESCRIPTION: Input channel module for the generation of the input circuitry for
each router port. It includes two virtual channels which share the physical
input channel.
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distribution System (LSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: May 20th, 2009.
--------------------------------------------------------------------------------
*/
#ifndef __XIN_2VC_H__
#define __XIN_2VC_H__

#include <systemc.h>
#include "parameters.h"
#include "xin.h"
#include "xin_ag.h"

SC_MODULE(xin_2vc)
{
    // System signals
    sc_in_clk   clk; // Clock
    sc_in<bool> rst; // Reset

    // Link signals
    sc_in<sc_uint<FLIT_WIDTH> > in_data;    // Input data channel
    sc_in<bool>                 vc0_in_val; // Data validation
    sc_in<bool>                 vc1_in_val; // Data validation
    sc_out<bool>                vc0_in_ret;	// Return (ack or credit)
    sc_out<bool>                vc1_in_ret;	// Return (ack or credit)

    // Commands and status signals interconnecting input and output channels for VC0
    sc_out<bool>    vc0_x_reqL;     // Request to Lout
    sc_out<bool>    vc0_x_reqN;     // Request to Nout
    sc_out<bool>    vc0_x_reqE;     // Request to Eout
    sc_out<bool>    vc0_x_reqS;     // Request to Sout
    sc_out<bool>    vc0_x_reqW;     // Request to Wout
    sc_out<bool>    vc0_x_rok;      // Rok to the outputs
    sc_in<bool>     vc0_x_rd0;      // Read commant from output channel 0
    sc_in<bool>     vc0_x_rd1;      // Read commant from output channel 1
    sc_in<bool>     vc0_x_rd2;      // Read commant from output channel 2
    sc_in<bool>     vc0_x_rd3;      // Read commant from output channel 3
    sc_in<bool>     vc0_x_gnt0;     // Grant from the output channel 0
    sc_in<bool>     vc0_x_gnt1;     // Grant from the output channel 1
    sc_in<bool>     vc0_x_gnt2;     // Grant from the output channel 2
    sc_in<bool>     vc0_x_gnt3;     // Grant from the output channel 3
    sc_in<bool>     vc0_x_Lidle;    // Status from Lout
    sc_in<bool>     vc0_x_Nidle;    // Status from Nout
    sc_in<bool>     vc0_x_Eidle;    // Status from Eout
    sc_in<bool>     vc0_x_Sidle;    // Status from Sout
    sc_in<bool>     vc0_x_Widle;    // Status from Wout

    // Commands and status signals interconnecting input and output channels for VC1
    sc_out<bool>    vc1_x_reqL;     // Request to Lout
    sc_out<bool>    vc1_x_reqN;     // Request to Nout
    sc_out<bool>    vc1_x_reqE;     // Request to Eout
    sc_out<bool>    vc1_x_reqS;     // Request to Sout
    sc_out<bool>    vc1_x_reqW;     // Request to Wout
    sc_out<bool>    vc1_x_rok;      // Rok to the outputs
    sc_in<bool>     vc1_x_rd0;      // Read commant from output channel 0
    sc_in<bool>     vc1_x_rd1;      // Read commant from output channel 1
    sc_in<bool>     vc1_x_rd2;      // Read commant from output channel 2
    sc_in<bool>     vc1_x_rd3;      // Read commant from output channel 3
    sc_in<bool>     vc1_x_gnt0;     // Grant from the output channel 0
    sc_in<bool>     vc1_x_gnt1;     // Grant from the output channel 1
    sc_in<bool>     vc1_x_gnt2;     // Grant from the output channel 2
    sc_in<bool>     vc1_x_gnt3;     // Grant from the output channel 3
    sc_in<bool>     vc1_x_Lidle;    // Status from Lout
    sc_in<bool>     vc1_x_Nidle;    // Status from Nout
    sc_in<bool>     vc1_x_Eidle;    // Status from Eout
    sc_in<bool>     vc1_x_Sidle;    // Status from Sout
    sc_in<bool>     vc1_x_Widle;    // Status from Wout

    // VCs Data buses
    sc_out<sc_uint<FLIT_WIDTH> > vc0_x_dout;    // Output data bus
    sc_out<sc_uint<FLIT_WIDTH> > vc1_x_dout;    // Output data bus

    // Internal data structures
    unsigned short int USE_THIS, XID, YID, PORT_ID;

    // Module's processes
    void p_null() {
        // If communication port is not used VCs' outputs are set to 0

        // Virtual channel 0
        vc0_in_ret.write(0);
        vc0_x_dout.write(0);
        vc0_x_reqL.write(0);
        vc0_x_reqN.write(0);
        vc0_x_reqE.write(0);
        vc0_x_reqS.write(0);
        vc0_x_reqW.write(0);
        vc0_x_rok.write(0);

        // Virtual channel 1
        vc1_in_ret.write(0);
        vc1_x_dout.write(0);
        vc1_x_reqL.write(0);
        vc1_x_reqN.write(0);
        vc1_x_reqE.write(0);
        vc1_x_reqS.write(0);
        vc1_x_reqW.write(0);
        vc1_x_rok.write(0);
    };

    SC_HAS_PROCESS(xin_2vc);

    //////////////////////////////////////////////////////////////////////////////
    xin_2vc(sc_module_name nm,	unsigned short int USE_THIS, unsigned short int XID,
            unsigned short int YID, unsigned short int PORT_ID) :
        sc_module(nm), USE_THIS(USE_THIS), XID(XID), YID(YID), PORT_ID(PORT_ID)
      //////////////////////////////////////////////////////////////////////////////
    {
        if (USE_THIS) {
            ////////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////////
            // It defines the architecture of VC 0
            ////////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////////
            if (PARIS_VC0_TYPE == PARIS_CHANNEL_NULL) {
            }

            else if (PARIS_VC0_TYPE == PARIS_CHANNEL_DEFAULT) {
                //////////////////////////////////////////////////////////////////////////
                xin *vc0 = new xin("vc0", USE_THIS, XID, YID, PORT_ID);
                //////////////////////////////////////////////////////////////////////////
                vc0->clk    (clk);
                vc0->rst    (rst);
                vc0->in_data(in_data);
                vc0->in_val (vc0_in_val);
                vc0->in_ret (vc0_in_ret);
                vc0->x_reqL (vc0_x_reqL);
                vc0->x_reqN (vc0_x_reqN);
                vc0->x_reqE (vc0_x_reqE);
                vc0->x_reqS (vc0_x_reqS);
                vc0->x_reqW (vc0_x_reqW);
                vc0->x_rok  (vc0_x_rok);
                vc0->x_rd0  (vc0_x_rd0);
                vc0->x_rd1  (vc0_x_rd1);
                vc0->x_rd2  (vc0_x_rd2);
                vc0->x_rd3  (vc0_x_rd3);
                vc0->x_gnt0 (vc0_x_gnt0);
                vc0->x_gnt1 (vc0_x_gnt1);
                vc0->x_gnt2 (vc0_x_gnt2);
                vc0->x_gnt3 (vc0_x_gnt3);
                vc0->x_Lidle(vc0_x_Lidle);
                vc0->x_Nidle(vc0_x_Nidle);
                vc0->x_Eidle(vc0_x_Eidle);
                vc0->x_Sidle(vc0_x_Sidle);
                vc0->x_Widle(vc0_x_Widle);
                vc0->x_dout (vc0_x_dout);
            }

            else if (PARIS_VC0_TYPE == PARIS_CHANNEL_CS) {
                printf("\n [XIN-2VC]: Circuit-switching architecture not implemented for VC0");
            }

            else if (PARIS_VC0_TYPE == PARIS_CHANNEL_AG) {
                //////////////////////////////////////////////////////////////////////////
                xin_ag *vc0 = new xin_ag("vc0", USE_THIS, XID, YID, PORT_ID, 0);
                //////////////////////////////////////////////////////////////////////////
                vc0->clk    (clk);
                vc0->rst    (rst);
                vc0->in_data(in_data);
                vc0->in_val (vc0_in_val);
                vc0->in_ret (vc0_in_ret);
                vc0->x_reqL (vc0_x_reqL);
                vc0->x_reqN (vc0_x_reqN);
                vc0->x_reqE (vc0_x_reqE);
                vc0->x_reqS (vc0_x_reqS);
                vc0->x_reqW (vc0_x_reqW);
                vc0->x_rok  (vc0_x_rok);
                vc0->x_rd0  (vc0_x_rd0);
                vc0->x_rd1  (vc0_x_rd1);
                vc0->x_rd2  (vc0_x_rd2);
                vc0->x_rd3  (vc0_x_rd3);
                vc0->x_gnt0 (vc0_x_gnt0);
                vc0->x_gnt1 (vc0_x_gnt1);
                vc0->x_gnt2 (vc0_x_gnt2);
                vc0->x_gnt3 (vc0_x_gnt3);
                vc0->x_Lidle(vc0_x_Lidle);
                vc0->x_Nidle(vc0_x_Nidle);
                vc0->x_Eidle(vc0_x_Eidle);
                vc0->x_Sidle(vc0_x_Sidle);
                vc0->x_Widle(vc0_x_Widle);
                vc0->x_dout (vc0_x_dout);
            }

            else
                printf("\n [XIN-2VC]: None architecture defined for VC0");

            ////////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////////
            // It defines the architecture of VC 1
            ////////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////////

            if (PARIS_VC1_TYPE == PARIS_CHANNEL_NULL) {
            }

            if (PARIS_VC1_TYPE == PARIS_CHANNEL_DEFAULT) {
                //////////////////////////////////////////////////////////////////////////
                xin *vc1 = new xin("vc1", USE_THIS, XID, YID, PORT_ID);
                //////////////////////////////////////////////////////////////////////////
                vc1->clk    (clk);
                vc1->rst    (rst);
                vc1->in_data(in_data);
                vc1->in_val (vc1_in_val);
                vc1->in_ret (vc1_in_ret);
                vc1->x_reqL (vc1_x_reqL);
                vc1->x_reqN (vc1_x_reqN);
                vc1->x_reqE (vc1_x_reqE);
                vc1->x_reqS (vc1_x_reqS);
                vc1->x_reqW (vc1_x_reqW);
                vc1->x_rok  (vc1_x_rok);
                vc1->x_rd0  (vc1_x_rd0);
                vc1->x_rd1  (vc1_x_rd1);
                vc1->x_rd2  (vc1_x_rd2);
                vc1->x_rd3  (vc1_x_rd3);
                vc1->x_gnt0 (vc1_x_gnt0);
                vc1->x_gnt1 (vc1_x_gnt1);
                vc1->x_gnt2 (vc1_x_gnt2);
                vc1->x_gnt3 (vc1_x_gnt3);
                vc1->x_Lidle(vc1_x_Lidle);
                vc1->x_Nidle(vc1_x_Nidle);
                vc1->x_Eidle(vc1_x_Eidle);
                vc1->x_Sidle(vc1_x_Sidle);
                vc1->x_Widle(vc1_x_Widle);
                vc1->x_dout (vc1_x_dout);
            }

            else if (PARIS_VC0_TYPE == PARIS_CHANNEL_CS) {
                printf("\n [XIN-2VC]: Circuit-switching architecture not implemented for VC1");
            }

            else if (PARIS_VC1_TYPE == PARIS_CHANNEL_AG) {
                //////////////////////////////////////////////////////////////////////////
                xin_ag *vc1 = new xin_ag("vc1", USE_THIS, XID, YID, PORT_ID, 1);
                //////////////////////////////////////////////////////////////////////////
                vc1->clk    (clk);
                vc1->rst    (rst);
                vc1->in_data(in_data);
                vc1->in_val (vc1_in_val);
                vc1->in_ret (vc1_in_ret);
                vc1->x_reqL (vc1_x_reqL);
                vc1->x_reqN (vc1_x_reqN);
                vc1->x_reqE (vc1_x_reqE);
                vc1->x_reqS (vc1_x_reqS);
                vc1->x_reqW (vc1_x_reqW);
                vc1->x_rok  (vc1_x_rok);
                vc1->x_rd0  (vc1_x_rd0);
                vc1->x_rd1  (vc1_x_rd1);
                vc1->x_rd2  (vc1_x_rd2);
                vc1->x_rd3  (vc1_x_rd3);
                vc1->x_gnt0 (vc1_x_gnt0);
                vc1->x_gnt1 (vc1_x_gnt1);
                vc1->x_gnt2 (vc1_x_gnt2);
                vc1->x_gnt3 (vc1_x_gnt3);
                vc1->x_Lidle(vc1_x_Lidle);
                vc1->x_Nidle(vc1_x_Nidle);
                vc1->x_Eidle(vc1_x_Eidle);
                vc1->x_Sidle(vc1_x_Sidle);
                vc1->x_Widle(vc1_x_Widle);
                vc1->x_dout (vc1_x_dout);
            }

            else
                printf("\n [XIN-2VC]: None architecture defined for VC1");

        } else { // IF DO NOT USE THIS
            SC_METHOD(p_null);
            sensitive << clk.pos();
        }
    }


};

#endif // __XIN_2VC_H__

