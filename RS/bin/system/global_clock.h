////////////////////////////////////////////////////////////////////////////////
// 
// name         global_clock.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __GLOBAL_CLOCK_H__
#define __GLOBAL_CLOCK_H__

#include <systemc.h>

SC_MODULE(global_clock)
{
    // INTERFACE
    // System signals
    sc_in_clk       	  clk;
    sc_in<bool>           rst;
    sc_out<sc_uint<64> >  clock_cycles;

    // Signals
    sc_signal<sc_uint<64> > clock_counter;

    // Internal data structures

    // Module's process
    void p_clock();

    SC_HAS_PROCESS(global_clock);

    //////////////////////////////////////////////////////////////////////////////
    global_clock(sc_module_name nm) : sc_module(nm)
      //////////////////////////////////////////////////////////////////////////////
    {
        SC_CTHREAD(p_clock, clk.pos());
        sensitive << clk.pos() << rst.pos();
    }

};
#endif // __GLOBAL_CLOCK_H__
