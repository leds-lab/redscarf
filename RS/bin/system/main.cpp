
/*
--------------------------------------------------------------------------------
PROJECT: SoCIN
FILE   : main.cpp
--------------------------------------------------------------------------------
DESCRIPTION: A SoC with 4 traffic generators attached to a 2x2 SoCIN NoC
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distribution System (LSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
*/

#include <systemc.h>
#include "parameters.h"
#include "reset.h"
#include "global_clock.h"
#include "stopsim.h"
#include "tg.h"
#include "tm_single.h"
#include "socin.h"

#define USE_LOCAL  1
#define USE_NORTH  1
#define USE_EAST   1
#define USE_SOUTH  1
#define USE_WEST   1
#define XID        1
#define YID        1


int sc_main(int argc, char* argv[])
{

    // Obtaining the operation clock frequency
    float tclk;
    if (argc >= 2)
        tclk = atof(argv[2]);
    else
        tclk = 1;
    // System signals
    sc_clock                clk_wire("clk", tclk, SC_NS);  // System clock - Tclk=1 ns
    sc_signal<bool>         rst_wire;                      // Reset
    sc_signal<sc_uint<64> > clock_cycles_wire;             // Number of cycles

    // NoC terminal (0,0)
    sc_signal<sc_uint<FLIT_WIDTH> > L_0_0_in_data_wire; 
    sc_signal<bool>                 L_0_0_in_val_wire;  
    sc_signal<bool>                 L_0_0_in_ret_wire;  
    sc_signal<sc_uint<FLIT_WIDTH> > L_0_0_out_data_wire;
    sc_signal<bool>                 L_0_0_out_val_wire; 
    sc_signal<bool>                 L_0_0_out_ret_wire; 

    // NoC terminal (0,1)
    sc_signal<sc_uint<FLIT_WIDTH> > L_0_1_in_data_wire; 
    sc_signal<bool>                 L_0_1_in_val_wire;  
    sc_signal<bool>                 L_0_1_in_ret_wire;  
    sc_signal<sc_uint<FLIT_WIDTH> > L_0_1_out_data_wire;
    sc_signal<bool>                 L_0_1_out_val_wire; 
    sc_signal<bool>                 L_0_1_out_ret_wire; 

    // NoC terminal (1,0)
    sc_signal<sc_uint<FLIT_WIDTH> > L_1_0_in_data_wire; 
    sc_signal<bool>                 L_1_0_in_val_wire;  
    sc_signal<bool>                 L_1_0_in_ret_wire;  
    sc_signal<sc_uint<FLIT_WIDTH> > L_1_0_out_data_wire;
    sc_signal<bool>                 L_1_0_out_val_wire; 
    sc_signal<bool>                 L_1_0_out_ret_wire; 

    // NoC terminal (1,1)
    sc_signal<sc_uint<FLIT_WIDTH> > L_1_1_in_data_wire; 
    sc_signal<bool>                 L_1_1_in_val_wire;  
    sc_signal<bool>                 L_1_1_in_ret_wire;  
    sc_signal<sc_uint<FLIT_WIDTH> > L_1_1_out_data_wire;
    sc_signal<bool>                 L_1_1_out_val_wire; 
    sc_signal<bool>                 L_1_1_out_ret_wire; 

    // Status signals of the traffic generators attached to the terminals of the NoC
    sc_signal<bool>                 TG_0_0_eot_wire; 
    sc_signal<bool>                 TG_0_1_eot_wire; 
    sc_signal<bool>                 TG_1_0_eot_wire; 
    sc_signal<bool>                 TG_1_1_eot_wire; 
    sc_signal<sc_uint<32> >         TG_0_0_number_of_packets_sent_wire; 
    sc_signal<sc_uint<32> >         TG_0_1_number_of_packets_sent_wire; 
    sc_signal<sc_uint<32> >         TG_1_0_number_of_packets_sent_wire; 
    sc_signal<sc_uint<32> >         TG_1_1_number_of_packets_sent_wire; 
    sc_signal<sc_uint<32> >         TG_0_0_number_of_packets_received_wire; 
    sc_signal<sc_uint<32> >         TG_0_1_number_of_packets_received_wire; 
    sc_signal<sc_uint<32> >         TG_1_0_number_of_packets_received_wire; 
    sc_signal<sc_uint<32> >         TG_1_1_number_of_packets_received_wire; 

    // Status signal saying that stopsim is ready to stop simulation
    sc_signal<bool>                 eos_wire; 

    /////////////////////////////////////////////////////////////
    reset *rst = new reset("rst");
    /////////////////////////////////////////////////////////////
    rst->clk(clk_wire);
    rst->rst(rst_wire);

    //////////////////////////////////////////////////////////////////////////////
    global_clock *clock0 = new global_clock("global_clock");
    //////////////////////////////////////////////////////////////////////////////
    clock0->clk(clk_wire);
    clock0->rst(rst_wire);
    clock0->clock_cycles(clock_cycles_wire);

    //////////////////////////////////////////////////////////////////////////////
    stopsim *stop0 = new stopsim("stopsim", (char *) argv[1],(char*)"stopsim");
    //////////////////////////////////////////////////////////////////////////////
    stop0->clk(clk_wire);
    stop0->rst(rst_wire);
    stop0->eos(eos_wire);
    stop0->clock_cycles(clock_cycles_wire);

    stop0->TG_0_0_number_of_packets_sent(TG_0_0_number_of_packets_sent_wire);
    stop0->TG_0_1_number_of_packets_sent(TG_0_1_number_of_packets_sent_wire);
    stop0->TG_1_0_number_of_packets_sent(TG_1_0_number_of_packets_sent_wire);
    stop0->TG_1_1_number_of_packets_sent(TG_1_1_number_of_packets_sent_wire);
    stop0->TG_0_0_number_of_packets_received(TG_0_0_number_of_packets_received_wire);
    stop0->TG_0_1_number_of_packets_received(TG_0_1_number_of_packets_received_wire);
    stop0->TG_1_0_number_of_packets_received(TG_1_0_number_of_packets_received_wire);
    stop0->TG_1_1_number_of_packets_received(TG_1_1_number_of_packets_received_wire);
    stop0->TG_0_0_eot(TG_0_0_eot_wire);
    stop0->TG_0_1_eot(TG_0_1_eot_wire);
    stop0->TG_1_0_eot(TG_1_0_eot_wire);
    stop0->TG_1_1_eot(TG_1_1_eot_wire);

    //////////////////////////////////////////////////////////////////////////////
    tg *tg_0_0 = new tg("tg_0_0", 0, 0);
    //////////////////////////////////////////////////////////////////////////////
    tg_0_0->clk(clk_wire);
    tg_0_0->rst(rst_wire);
    tg_0_0->clock_cycles(clock_cycles_wire);
    tg_0_0->out_data  (L_0_0_in_data_wire );
    tg_0_0->out_val   (L_0_0_in_val_wire  );
    tg_0_0->out_ret   (L_0_0_in_ret_wire  );
    tg_0_0->in_data   (L_0_0_out_data_wire);
    tg_0_0->in_val    (L_0_0_out_val_wire );
    tg_0_0->in_ret    (L_0_0_out_ret_wire );
    tg_0_0->eot(TG_0_0_eot_wire);
    tg_0_0->number_of_packets_sent(TG_0_0_number_of_packets_sent_wire);
    tg_0_0->number_of_packets_received(TG_0_0_number_of_packets_received_wire);

    //////////////////////////////////////////////////////////////////////////////
    tg *tg_0_1 = new tg("tg_0_1", 0, 1);
    //////////////////////////////////////////////////////////////////////////////
    tg_0_1->clk(clk_wire);
    tg_0_1->rst(rst_wire);
    tg_0_1->clock_cycles(clock_cycles_wire);
    tg_0_1->out_data  (L_0_1_in_data_wire );
    tg_0_1->out_val   (L_0_1_in_val_wire  );
    tg_0_1->out_ret   (L_0_1_in_ret_wire  );
    tg_0_1->in_data   (L_0_1_out_data_wire);
    tg_0_1->in_val    (L_0_1_out_val_wire );
    tg_0_1->in_ret    (L_0_1_out_ret_wire );
    tg_0_1->eot(TG_0_1_eot_wire);
    tg_0_1->number_of_packets_sent(TG_0_1_number_of_packets_sent_wire);
    tg_0_1->number_of_packets_received(TG_0_1_number_of_packets_received_wire);

    //////////////////////////////////////////////////////////////////////////////
    tg *tg_1_0 = new tg("tg_1_0", 1, 0);
    //////////////////////////////////////////////////////////////////////////////
    tg_1_0->clk(clk_wire);
    tg_1_0->rst(rst_wire);
    tg_1_0->clock_cycles(clock_cycles_wire);
    tg_1_0->out_data  (L_1_0_in_data_wire );
    tg_1_0->out_val   (L_1_0_in_val_wire  );
    tg_1_0->out_ret   (L_1_0_in_ret_wire  );
    tg_1_0->in_data   (L_1_0_out_data_wire);
    tg_1_0->in_val    (L_1_0_out_val_wire );
    tg_1_0->in_ret    (L_1_0_out_ret_wire );
    tg_1_0->eot(TG_1_0_eot_wire);
    tg_1_0->number_of_packets_sent(TG_1_0_number_of_packets_sent_wire);
    tg_1_0->number_of_packets_received(TG_1_0_number_of_packets_received_wire);

    //////////////////////////////////////////////////////////////////////////////
    tg *tg_1_1 = new tg("tg_1_1", 1, 1);
    //////////////////////////////////////////////////////////////////////////////
    tg_1_1->clk(clk_wire);
    tg_1_1->rst(rst_wire);
    tg_1_1->clock_cycles(clock_cycles_wire);
    tg_1_1->out_data  (L_1_1_in_data_wire );
    tg_1_1->out_val   (L_1_1_in_val_wire  );
    tg_1_1->out_ret   (L_1_1_in_ret_wire  );
    tg_1_1->in_data   (L_1_1_out_data_wire);
    tg_1_1->in_val    (L_1_1_out_val_wire );
    tg_1_1->in_ret    (L_1_1_out_ret_wire );
    tg_1_1->eot(TG_1_1_eot_wire);
    tg_1_1->number_of_packets_sent(TG_1_1_number_of_packets_sent_wire);
    tg_1_1->number_of_packets_received(TG_1_1_number_of_packets_received_wire);

    //////////////////////////////////////////////////////////////////////////////
    tm_single *tm_0_0_out = new tm_single("tm_0_0_out", 0, 0,(char *) argv[1], (char*)"ext_0_0_out");
    //////////////////////////////////////////////////////////////////////////////
    tm_0_0_out->clk(clk_wire);
    tm_0_0_out->rst(rst_wire);
    tm_0_0_out->eos(eos_wire);
    tm_0_0_out->clock_cycles(clock_cycles_wire);
    tm_0_0_out->data(L_0_0_out_data_wire );
    tm_0_0_out->val (L_0_0_out_val_wire  );
    tm_0_0_out->ret (L_0_0_out_ret_wire  );

    //////////////////////////////////////////////////////////////////////////////
    tm_single *tm_0_1_out = new tm_single("tm_0_1_out", 0, 1,(char *) argv[1], (char*)"ext_0_1_out");
    //////////////////////////////////////////////////////////////////////////////
    tm_0_1_out->clk(clk_wire);
    tm_0_1_out->rst(rst_wire);
    tm_0_1_out->eos(eos_wire);
    tm_0_1_out->clock_cycles(clock_cycles_wire);
    tm_0_1_out->data(L_0_1_out_data_wire );
    tm_0_1_out->val (L_0_1_out_val_wire  );
    tm_0_1_out->ret (L_0_1_out_ret_wire  );

    //////////////////////////////////////////////////////////////////////////////
    tm_single *tm_1_0_out = new tm_single("tm_1_0_out", 1, 0,(char *) argv[1], (char*)"ext_1_0_out");
    //////////////////////////////////////////////////////////////////////////////
    tm_1_0_out->clk(clk_wire);
    tm_1_0_out->rst(rst_wire);
    tm_1_0_out->eos(eos_wire);
    tm_1_0_out->clock_cycles(clock_cycles_wire);
    tm_1_0_out->data(L_1_0_out_data_wire );
    tm_1_0_out->val (L_1_0_out_val_wire  );
    tm_1_0_out->ret (L_1_0_out_ret_wire  );

    //////////////////////////////////////////////////////////////////////////////
    tm_single *tm_1_1_out = new tm_single("tm_1_1_out", 1, 1,(char *) argv[1], (char*)"ext_1_1_out");
    //////////////////////////////////////////////////////////////////////////////
    tm_1_1_out->clk(clk_wire);
    tm_1_1_out->rst(rst_wire);
    tm_1_1_out->eos(eos_wire);
    tm_1_1_out->clock_cycles(clock_cycles_wire);
    tm_1_1_out->data(L_1_1_out_data_wire );
    tm_1_1_out->val (L_1_1_out_val_wire  );
    tm_1_1_out->ret (L_1_1_out_ret_wire  );

    //////////////////////////////////////////////////////////////////////////////
    socin *socin0 = new socin("socin");
    //////////////////////////////////////////////////////////////////////////////
    // System signals
    socin0->clk(clk_wire);
    socin0->rst(rst_wire);

    // NoC terminal 0 0
    socin0->L_0_0_in_data (L_0_0_in_data_wire );
    socin0->L_0_0_in_val  (L_0_0_in_val_wire  );
    socin0->L_0_0_in_ret  (L_0_0_in_ret_wire  );
    socin0->L_0_0_out_data(L_0_0_out_data_wire);
    socin0->L_0_0_out_val (L_0_0_out_val_wire );
    socin0->L_0_0_out_ret (L_0_0_out_ret_wire );

    // NoC terminal 0 1
    socin0->L_0_1_in_data (L_0_1_in_data_wire );
    socin0->L_0_1_in_val  (L_0_1_in_val_wire  );
    socin0->L_0_1_in_ret  (L_0_1_in_ret_wire  );
    socin0->L_0_1_out_data(L_0_1_out_data_wire);
    socin0->L_0_1_out_val (L_0_1_out_val_wire );
    socin0->L_0_1_out_ret (L_0_1_out_ret_wire );

    // NoC terminal 1 0
    socin0->L_1_0_in_data (L_1_0_in_data_wire );
    socin0->L_1_0_in_val  (L_1_0_in_val_wire  );
    socin0->L_1_0_in_ret  (L_1_0_in_ret_wire  );
    socin0->L_1_0_out_data(L_1_0_out_data_wire);
    socin0->L_1_0_out_val (L_1_0_out_val_wire );
    socin0->L_1_0_out_ret (L_1_0_out_ret_wire );

    // NoC terminal 1 1
    socin0->L_1_1_in_data (L_1_1_in_data_wire );
    socin0->L_1_1_in_val  (L_1_1_in_val_wire  );
    socin0->L_1_1_in_ret  (L_1_1_in_ret_wire  );
    socin0->L_1_1_out_data(L_1_1_out_data_wire);
    socin0->L_1_1_out_val (L_1_1_out_val_wire );
    socin0->L_1_1_out_ret (L_1_1_out_ret_wire );

    // Start the simulation (the testbench will stop it)
    sc_start();
    return 0;
}
