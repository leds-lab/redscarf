/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : fifo_out
FILE   : fifo_out.h
--------------------------------------------------------------------------------
DESCRIPTION: Implements a FIFO to be used as output buffers in ParIS router.
This implementation is based on an integration of the original VHDL codes of
fifo_datapath and fifo_controller into a single module.

Note: The only difference between fifo_out and fifo_in is in the use of 
parameters FIFO_XX_DEPTH and FIFO_XX_LOG2_DEPTH in order to allow the use of
buffers with differents depths at the input and output channels.
--------------------------------------------------------------------------------
AUTHORS: Group of Embedded and Distribution System (GSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: October 29th, 2006.
---------------------------------------------------------------------------
*/
#ifndef __FIFO_OUT_H__
#define __FIFO_OUT_H__

#include <systemc.h>
#include "parameters.h"

////////////////////////////////////////////////////////////////////////////////
SC_MODULE(fifo_out)
////////////////////////////////////////////////////////////////////////////////
{
    // System signals
    sc_in_clk                    clk;   // clock
    sc_in<bool>                  rst;   // reset

    // FIFO interface
    sc_out<bool>                 rok;   // FIFO has a data to be read	 (not empty)
    sc_out<bool>                 wok;   // FIFO has room	 to be written (not full)
    sc_in<bool>                  rd;    // command to read a data from the FIFO
    sc_in<bool>                  wr;    // command to write data into
    sc_in<sc_uint<FIFO_WIDTH> >  din;   // input 	data channel
    sc_out<sc_uint<FIFO_WIDTH> > dout;  // output	data channel

    // Signals
    sc_signal<sc_uint<FIFO_OUT_LOG2_DEPTH+1> > state_reg;   // current FIFO state
    sc_signal<sc_uint<FIFO_OUT_LOG2_DEPTH+1> > next_state;  // next    FIFO state
    sc_signal<sc_uint<FIFO_OUT_LOG2_DEPTH+1> > rd_ptr_reg;	// read  pointer
    sc_signal<sc_uint<FIFO_OUT_LOG2_DEPTH+1> > wr_ptr_reg;	// write pointer
    sc_signal<sc_uint<FIFO_OUT_LOG2_DEPTH+1> > next_rd_ptr;	// next	read 	pointer
    sc_signal<sc_uint<FIFO_OUT_LOG2_DEPTH+1> > next_wr_ptr;	// next write pointer
    sc_signal<sc_uint<FIFO_WIDTH> > fifo[FIFO_OUT_DEPTH];   // here is the buffer

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    // Module's processes
    // Controller
    void p_next_state();
    void p_current_state();
    void p_debug_controller();

    // Datapath
    void p_next_wr_ptr();
    void p_next_rd_ptr();
    void p_wr_fifo();
    void p_outputs();
    void p_reg();
    void p_debug_datapath();

    // No fifo
    void p_fifo_null();

    SC_HAS_PROCESS(fifo_out);

    //////////////////////////////////////////////////////////////////////////////
    fifo_out(sc_module_name nm,	unsigned short int XID, unsigned short int YID,
             unsigned short int PORT_ID) : sc_module(nm), XID(XID), YID(YID),
        PORT_ID(PORT_ID)
      //////////////////////////////////////////////////////////////////////////////
    {
        //////////////////////
#if FIFO_OUT_DEPTH > 0
        //////////////////////
        // Controller
        SC_METHOD(p_next_state);
        sensitive << wr << rd << state_reg;

        SC_METHOD(p_current_state);
        sensitive << clk.pos() << rst;

#ifdef DEBUG_FIFO_OUT_CONTROLLER
        SC_METHOD(p_debug_controller);
        sensitive << clk.pos();
#endif

        // Datapath
        SC_METHOD(p_next_wr_ptr);
        sensitive << state_reg << wr << wr_ptr_reg;

        SC_METHOD(p_next_rd_ptr);
        sensitive << state_reg << rd << rd_ptr_reg;

        SC_METHOD(p_reg);
        sensitive << clk.pos() << rst;

        SC_METHOD(p_wr_fifo);
        sensitive << clk.pos() << rst;

        SC_METHOD(p_outputs);
        sensitive << state_reg << rd_ptr_reg;

#ifdef DEBUG_FIFO_OUT_DATAPATH
        SC_METHOD(p_debug_datapath);
        sensitive << clk.pos();
#endif

        ////////////////////////////
#else // FIFO_OUT_DEPTH = 0
        ////////////////////////////
        SC_METHOD(p_fifo_null);
        sensitive << wr << rd << din;
#endif
    }
};
#endif
