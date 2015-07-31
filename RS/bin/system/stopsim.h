
////////////////////////////////////////////////////////////////////////////////
// 
// name         stopsim.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __STOPSIM_H__
#define __STOPSIM_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(stopsim)
{
    // INTERFACE
    // System signals
    sc_in_clk           clk;
    sc_in<bool>         rst;
    sc_out<bool>        eos;
    sc_in<sc_uint<64> > clock_cycles;

    sc_in<bool>         TG_0_0_eot; 
    sc_in<bool>         TG_0_1_eot; 
    sc_in<bool>         TG_1_0_eot; 
    sc_in<bool>         TG_1_1_eot; 

    sc_in<sc_uint<32> > TG_0_0_number_of_packets_sent; 
    sc_in<sc_uint<32> > TG_0_1_number_of_packets_sent; 
    sc_in<sc_uint<32> > TG_1_0_number_of_packets_sent; 
    sc_in<sc_uint<32> > TG_1_1_number_of_packets_sent; 

    sc_in<sc_uint<32> > TG_0_0_number_of_packets_received; 
    sc_in<sc_uint<32> > TG_0_1_number_of_packets_received; 
    sc_in<sc_uint<32> > TG_1_0_number_of_packets_received; 
    sc_in<sc_uint<32> > TG_1_1_number_of_packets_received; 

    // Signals
    sc_signal<sc_uint<32> > total_packets_received;
    sc_signal<sc_uint<32> > total_packets_sent;
    sc_signal<bool>         eot;

    // Internal data structures
    char *PATH;
    char *FILENAME;

    // Module's process
    void p_stop();

    SC_HAS_PROCESS(stopsim);

    //////////////////////////////////////////////////////////////////////////////
    stopsim(sc_module_name nm,
            char *PATH,
            char *FILENAME) :
            sc_module(nm),
            PATH(PATH),
            FILENAME(FILENAME)
    //////////////////////////////////////////////////////////////////////////////
    { 
        SC_CTHREAD(p_stop, clk.pos());
        sensitive << clk.pos() << rst;
    }

};
#endif // __STOPSIM_H__
