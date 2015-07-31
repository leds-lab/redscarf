////////////////////////////////////////////////////////////////////////////////
// 
// name         tm_2vc.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __TM_2VC_H__
#define __TM_2VC_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(tm_2vc)
{
    // INTERFACE
    // System signals
    sc_in_clk           clk;
    sc_in<bool>         rst;
    sc_in<bool>         eos;
    sc_in<sc_uint<64> > clock_cycles;

    // Communication Port
    sc_in<sc_uint<FLIT_WIDTH> > data;
    sc_in<bool>                 vc0_val;
    sc_in<bool>                 vc0_ret;
    sc_in<bool>                 vc1_val;
    sc_in<bool>                 vc1_ret;

    // Internal data structures
    unsigned short int XID, YID;
    char *PATH;
    char *FILENAME;

    // Variables
    unsigned int       file_is_open;
	unsigned int       pck_received;

    unsigned int       vc0_pck_received;
    unsigned int       vc0_pck_size;
    //unsigned int       vc0_nb_of_cycles_per_flit;
    unsigned int       vc0_x_src;
    unsigned int       vc0_y_src;
    unsigned int       vc0_x_dest;
    unsigned int       vc0_y_dest;
    unsigned int       vc0_flow_id;
    unsigned int       vc0_traffic_class;
    unsigned int       vc0_required_bw;
    unsigned long long vc0_header;
    unsigned long long vc0_deadline;
    unsigned long long vc0_cycle_to_send;
    unsigned long long vc0_cycle_of_arriving;

    unsigned int       vc1_pck_received;
    unsigned int       vc1_pck_size;
    //unsigned int       vc1_nb_of_cycles_per_flit;
    unsigned int       vc1_x_src;
    unsigned int       vc1_y_src;
    unsigned int       vc1_x_dest;
    unsigned int       vc1_y_dest;
    unsigned int       vc1_flow_id;
    unsigned int       vc1_traffic_class;
    unsigned int       vc1_required_bw;
    unsigned long long vc1_header;
    unsigned long long vc1_deadline;
    unsigned long long vc1_cycle_to_send;
    unsigned long long vc1_cycle_of_arriving;


    //  unsigned long int cycle_of_injection;

    //  unsigned long int delay_of_injection;
    //  unsigned long int latency;
    //  double latency;

    char path_filename[256];
    FILE *fp_out;

    // Module process
    void p_vc0_probe();
    void p_vc1_probe();
    void p_start();
    void p_stop();

    // Member functions
    void f_write_received_flit_vc0(FILE *);
    void f_write_received_flit_vc1(FILE *);


    SC_HAS_PROCESS(tm_2vc);

    //////////////////////////////////////////////////////////////////////////////
    tm_2vc(sc_module_name nm,
           unsigned short int XID,
           unsigned short int YID,
           char *PATH,
           char *FILENAME) :
        sc_module(nm),
        XID(XID),
        YID(YID),
        PATH(PATH),
        FILENAME(FILENAME)
    //////////////////////////////////////////////////////////////////////////////
    {
        SC_CTHREAD(p_vc0_probe, clk.pos());
        sensitive << clk.pos() << rst.pos();

        SC_CTHREAD(p_vc1_probe, clk.pos());
        sensitive << clk.pos() << rst.pos();

        SC_CTHREAD(p_stop, clk.pos());
        sensitive << clk.pos() << rst.pos();

        SC_CTHREAD(p_start, clk.pos());
        sensitive << clk.pos() << rst.pos();

    }

};
#endif // __TM_2VC_H__
