/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : Xin (input channel)
FILE   : Xin.h
--------------------------------------------------------------------------------
DESCRIPTION: Input channel module for the generation of the input circuitry for
each router port.
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distribution System (LSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: August 21th, 2006.
--------------------------------------------------------------------------------
*/
#ifndef __XIN_H__
#define __XIN_H__

#include <systemc.h>
#include "parameters.h"
#include "fifo_in.h"
#include "ifc.h"
#include "irs.h"
#include "ic.h"
#include "and2.h"

SC_MODULE(xin)
{
    // System signals
    sc_in_clk   clk; // Clock
    sc_in<bool> rst; // Reset

    // Link signals
    sc_in<sc_uint<FLIT_WIDTH> > in_data;    // Input data channel
    sc_in<bool>  				in_val;     // Data validation
    sc_out<bool> 				in_ret;     // Return (ack or credit)

    // Commands and status signals interconnecting input and output channels
    sc_out<bool> x_reqL;    // Request to Lout
    sc_out<bool> x_reqN;    // Request to Nout
    sc_out<bool> x_reqE;    // Request to Eout
    sc_out<bool> x_reqS;    // Request to Sout
    sc_out<bool> x_reqW;    // Request to Wout
    sc_out<bool> x_rok;     // Rok to the outputs
    sc_in<bool>  x_rd0;     // Read commant from output channel 0
    sc_in<bool>  x_rd1;     // Read commant from output channel 1
    sc_in<bool>  x_rd2;     // Read commant from output channel 2
    sc_in<bool>  x_rd3;     // Read commant from output channel 3
    sc_in<bool>  x_gnt0;    // Grant from the output channel 0
    sc_in<bool>  x_gnt1;    // Grant from the output channel 1
    sc_in<bool>  x_gnt2;    // Grant from the output channel 2
    sc_in<bool>  x_gnt3;    // Grant from the output channel 3
    sc_in<bool>  x_Lidle;   // Status from Lout
    sc_in<bool>  x_Nidle;   // Status from Nout
    sc_in<bool>  x_Eidle;   // Status from Eout
    sc_in<bool>  x_Sidle;   // Status from Sout
    sc_in<bool>  x_Widle;   // Status from Wout

    // Data to the output channels
    sc_out<sc_uint<FLIT_WIDTH> > x_dout; 	// Output data bus

    // Internal signals
    sc_signal<bool> rok_wire;           // FIFO has a data to be read
    sc_signal<bool>	wok_wire;           // FIFO has room to be written
    sc_signal<bool>	x_rd_wire;          // Selected command to read a data
    sc_signal<bool>	rd_wire;            // Command to read a data from the FIFO
    sc_signal<bool>	wr_wire;            // Command to write data into the FIFO
    sc_signal<bool>	requesting_wire;    // There exists someone requesting

    // Internal data structures
    unsigned short int USE_THIS, XID, YID, PORT_ID;

    // Module's processes
    void p_null() {
        in_ret.write(0);
        x_dout.write(0);
        x_reqL.write(0);
        x_reqN.write(0);
        x_reqE.write(0);
        x_reqS.write(0);
        x_reqW.write(0);
        x_rok.write(0);
    };

    SC_HAS_PROCESS(xin);

    //////////////////////////////////////////////////////////////////////////////
    xin(sc_module_name nm,	unsigned short int USE_THIS, unsigned short int XID,
        unsigned short int YID, unsigned short int PORT_ID) :
        sc_module(nm), USE_THIS(USE_THIS), XID(XID), YID(YID), PORT_ID(PORT_ID)
      //////////////////////////////////////////////////////////////////////////////
    {
        if (USE_THIS) {
            ///////////////////////////////////////////////////////
            fifo_in *fifo0 = new fifo_in("fifo",XID, YID, PORT_ID);
            ///////////////////////////////////////////////////////
            fifo0->clk(clk);
            fifo0->rst(rst);
            fifo0->rok(rok_wire); // REVISION: changed from x_rok to rok_wire
            fifo0->wok(wok_wire);
            fifo0->rd(rd_wire);
            fifo0->wr(wr_wire);
            fifo0->din(in_data);
            fifo0->dout(x_dout);

            //////////////////////////////////////////
            ic *ic0 = new ic("ic", XID, YID, PORT_ID);
            //////////////////////////////////////////
            ic0->clk(clk);
            ic0->rst(rst);
            ic0->rok(rok_wire); // REVISION: changed from x_rok to rok_wire
            ic0->rd(rd_wire);
            ic0->data(x_dout);
            ic0->Lidle(x_Lidle);
            ic0->Nidle(x_Nidle);
            ic0->Eidle(x_Eidle);
            ic0->Sidle(x_Sidle);
            ic0->Widle(x_Widle);
            ic0->reqL(x_reqL);
            ic0->reqN(x_reqN);
            ic0->reqE(x_reqE);
            ic0->reqS(x_reqS);
            ic0->reqW(x_reqW);
            ic0->requesting(requesting_wire);  // INCLUDED IN THE REVISION

            ////////////////////////////////////
            and2 *and_rok = new and2("and_rok");// INCLUDED IN THE REVISION
            ////////////////////////////////////
            and_rok->din0(rok_wire);
            and_rok->din1(requesting_wire);
            and_rok->dout(x_rok);

            //////////////////////////////////////////////
            ifc *ifc0 = new ifc("ifc", XID, YID, PORT_ID);
            //////////////////////////////////////////////
            ifc0->clk(clk);
            ifc0->rst(rst);
            ifc0->val(in_val);
            ifc0->ret(in_ret);
            ifc0->rok(x_rok);
            ifc0->rd(rd_wire);
            ifc0->wok(wok_wire);
            ifc0->wr(wr_wire);
            ifc0->data(x_dout);

            ///////////////////////////
            irs *irs0 = new irs("irs");
            ///////////////////////////
            irs0->gnt0(x_gnt0);
            irs0->gnt1(x_gnt1);
            irs0->gnt2(x_gnt2);
            irs0->gnt3(x_gnt3);
            irs0->rd_in0(x_rd0);
            irs0->rd_in1(x_rd1);
            irs0->rd_in2(x_rd2);
            irs0->rd_in3(x_rd3);
            irs0->rd_out(x_rd_wire);

            ////////////////////////////////////
            and2 *and_rd = new and2("and_rd");// INCLUDED IN THE REVISION
            ////////////////////////////////////
            and_rd->din0(x_rd_wire);
            and_rd->din1(requesting_wire);
            and_rd->dout(rd_wire);

        } else { // DON'T USE THIS
            SC_METHOD(p_null);
            sensitive << clk.pos();
        }

    }
};

#endif // __XIN_H__

