////////////////////////////////////////////////////////////////////////////////
// 
// name         reset.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __RESET_H__
#define __RESET_H__

#include <systemc.h>

SC_MODULE(reset)
{
    // INTERFACE
    // System signals
    sc_in_clk    clk;
    sc_out<bool> rst;

    // Module process
    void p_reset() {
        rst.write(1);
        wait();

        rst.write(0);
        wait();
        while(1) wait();
    };

    SC_HAS_PROCESS(reset);

    //////////////////////////////////////////////////////////////////////////////
    reset(sc_module_name nm) : sc_module(nm)
    //////////////////////////////////////////////////////////////////////////////
    {
        SC_CTHREAD(p_reset, clk.pos());
        sensitive << clk.pos();
    }

};
#endif // __RESET_H__
