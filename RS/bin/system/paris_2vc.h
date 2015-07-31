/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : paris_2vc (parameterizable interconnect switch)
FILE   : paris_2vc.h
--------------------------------------------------------------------------------
DESCRIPTION: ParIS router with 2 virtual channels
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distribution System (LSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: August 21th, 2006.
--------------------------------------------------------------------------------
*/
#ifndef __PARIS_2VC_H__
#define __PARIS_2VC_H__

#include <systemc.h>
#include "parameters.h"
#include "xin_2vc.h"
#include "xout_2vc.h"


SC_MODULE(paris_2vc)
{
    // INTERFACE
    // System signals
    sc_in_clk       clk;
    sc_in<bool>     rst;

    // Local Communication Port
    sc_in<sc_uint<FLIT_WIDTH> >  Lin_data;
    sc_in<bool>                  Lin_vc0_val;
    sc_out<bool>                 Lin_vc0_ret;
    sc_in<bool>                  Lin_vc1_val;
    sc_out<bool>                 Lin_vc1_ret;

    sc_out<sc_uint<FLIT_WIDTH> > Lout_data;
    sc_out<bool>                 Lout_vc0_val;
    sc_in<bool>                  Lout_vc0_ret;
    sc_out<bool>                 Lout_vc1_val;
    sc_in<bool>                  Lout_vc1_ret;

    // North Communication Port
    sc_in<sc_uint<FLIT_WIDTH> >  Nin_data;
    sc_in<bool>                  Nin_vc0_val;
    sc_out<bool>                 Nin_vc0_ret;
    sc_in<bool>                  Nin_vc1_val;
    sc_out<bool>                 Nin_vc1_ret;

    sc_out<sc_uint<FLIT_WIDTH> > Nout_data;
    sc_out<bool>                 Nout_vc0_val;
    sc_in<bool>                  Nout_vc0_ret;
    sc_out<bool>                 Nout_vc1_val;
    sc_in<bool>                  Nout_vc1_ret;


    // East Communication Port
    sc_in<sc_uint<FLIT_WIDTH> >  Ein_data;
    sc_in<bool>                  Ein_vc0_val;
    sc_out<bool>                 Ein_vc0_ret;
    sc_in<bool>                  Ein_vc1_val;
    sc_out<bool>                 Ein_vc1_ret;

    sc_out<sc_uint<FLIT_WIDTH> > Eout_data;
    sc_out<bool>                 Eout_vc0_val;
    sc_in<bool>                  Eout_vc0_ret;
    sc_out<bool>                 Eout_vc1_val;
    sc_in<bool>                  Eout_vc1_ret;

    // South Communication Port
    sc_in<sc_uint<FLIT_WIDTH> >  Sin_data;
    sc_in<bool>                  Sin_vc0_val;
    sc_out<bool>                 Sin_vc0_ret;
    sc_in<bool>                  Sin_vc1_val;
    sc_out<bool>                 Sin_vc1_ret;

    sc_out<sc_uint<FLIT_WIDTH> > Sout_data;
    sc_out<bool>                 Sout_vc0_val;
    sc_in<bool>                  Sout_vc0_ret;
    sc_out<bool>                 Sout_vc1_val;
    sc_in<bool>                  Sout_vc1_ret;

    // West Communication Port
    sc_in<sc_uint<FLIT_WIDTH> >  Win_data;
    sc_in<bool>                  Win_vc0_val;
    sc_out<bool>                 Win_vc0_ret;
    sc_in<bool>                  Win_vc1_val;
    sc_out<bool>                 Win_vc1_ret;

    sc_out<sc_uint<FLIT_WIDTH> > Wout_data;
    sc_out<bool>                 Wout_vc0_val;
    sc_in<bool>                  Wout_vc0_ret;
    sc_out<bool>                 Wout_vc1_val;
    sc_in<bool>                  Wout_vc1_ret;

    // SIGNALS
    // Requests from L
    sc_signal<bool> vc0_Lunused_wire;
    sc_signal<bool> vc0_LreqN_wire;
    sc_signal<bool> vc0_LreqE_wire;
    sc_signal<bool> vc0_LreqS_wire;
    sc_signal<bool> vc0_LreqW_wire;

    sc_signal<bool> vc1_Lunused_wire;
    sc_signal<bool> vc1_LreqN_wire;
    sc_signal<bool> vc1_LreqE_wire;
    sc_signal<bool> vc1_LreqS_wire;
    sc_signal<bool> vc1_LreqW_wire;

    // Requests from N
    sc_signal<bool> vc0_Nunused_wire;
    sc_signal<bool> vc0_NreqL_wire;
    sc_signal<bool> vc0_NreqE_wire;
    sc_signal<bool> vc0_NreqS_wire;
    sc_signal<bool> vc0_NreqW_wire;

    sc_signal<bool> vc1_Nunused_wire;
    sc_signal<bool> vc1_NreqL_wire;
    sc_signal<bool> vc1_NreqE_wire;
    sc_signal<bool> vc1_NreqS_wire;
    sc_signal<bool> vc1_NreqW_wire;

    // Requests from E
    sc_signal<bool> vc0_Eunused_wire;
    sc_signal<bool> vc0_EreqL_wire;
    sc_signal<bool> vc0_EreqN_wire;
    sc_signal<bool> vc0_EreqS_wire;
    sc_signal<bool> vc0_EreqW_wire;

    sc_signal<bool> vc1_Eunused_wire;
    sc_signal<bool> vc1_EreqL_wire;
    sc_signal<bool> vc1_EreqN_wire;
    sc_signal<bool> vc1_EreqS_wire;
    sc_signal<bool> vc1_EreqW_wire;

    // Requests from S
    sc_signal<bool> vc0_Sunused_wire;
    sc_signal<bool> vc0_SreqL_wire;
    sc_signal<bool> vc0_SreqN_wire;
    sc_signal<bool> vc0_SreqE_wire;
    sc_signal<bool> vc0_SreqW_wire;

    sc_signal<bool> vc1_Sunused_wire;
    sc_signal<bool> vc1_SreqL_wire;
    sc_signal<bool> vc1_SreqN_wire;
    sc_signal<bool> vc1_SreqE_wire;
    sc_signal<bool> vc1_SreqW_wire;

    // Requests from W
    sc_signal<bool> vc0_Wunused_wire;
    sc_signal<bool> vc0_WreqL_wire;
    sc_signal<bool> vc0_WreqN_wire;
    sc_signal<bool> vc0_WreqE_wire;
    sc_signal<bool> vc0_WreqS_wire;

    sc_signal<bool> vc1_Wunused_wire;
    sc_signal<bool> vc1_WreqL_wire;
    sc_signal<bool> vc1_WreqN_wire;
    sc_signal<bool> vc1_WreqE_wire;
    sc_signal<bool> vc1_WreqS_wire;

    // Grants from L
    sc_signal<bool> vc0_LgntN_wire;
    sc_signal<bool> vc0_LgntE_wire;
    sc_signal<bool> vc0_LgntS_wire;
    sc_signal<bool> vc0_LgntW_wire;

    sc_signal<bool> vc1_LgntN_wire;
    sc_signal<bool> vc1_LgntE_wire;
    sc_signal<bool> vc1_LgntS_wire;
    sc_signal<bool> vc1_LgntW_wire;

    // Grants from N
    sc_signal<bool> vc0_NgntL_wire;
    sc_signal<bool> vc0_NgntE_wire;
    sc_signal<bool> vc0_NgntS_wire;
    sc_signal<bool> vc0_NgntW_wire;

    sc_signal<bool> vc1_NgntL_wire;
    sc_signal<bool> vc1_NgntE_wire;
    sc_signal<bool> vc1_NgntS_wire;
    sc_signal<bool> vc1_NgntW_wire;

    // Grants from E
    sc_signal<bool> vc0_EgntL_wire;
    sc_signal<bool> vc0_EgntN_wire;
    sc_signal<bool> vc0_EgntS_wire;
    sc_signal<bool> vc0_EgntW_wire;

    sc_signal<bool> vc1_EgntL_wire;
    sc_signal<bool> vc1_EgntN_wire;
    sc_signal<bool> vc1_EgntS_wire;
    sc_signal<bool> vc1_EgntW_wire;

    // Grants from S
    sc_signal<bool> vc0_SgntL_wire;
    sc_signal<bool> vc0_SgntN_wire;
    sc_signal<bool> vc0_SgntE_wire;
    sc_signal<bool> vc0_SgntW_wire;

    sc_signal<bool> vc1_SgntL_wire;
    sc_signal<bool> vc1_SgntN_wire;
    sc_signal<bool> vc1_SgntE_wire;
    sc_signal<bool> vc1_SgntW_wire;

    // Grants from W
    sc_signal<bool> vc0_WgntL_wire;
    sc_signal<bool> vc0_WgntN_wire;
    sc_signal<bool> vc0_WgntE_wire;
    sc_signal<bool> vc0_WgntS_wire;

    sc_signal<bool> vc1_WgntL_wire;
    sc_signal<bool> vc1_WgntN_wire;
    sc_signal<bool> vc1_WgntE_wire;
    sc_signal<bool> vc1_WgntS_wire;

    // Read Status
    sc_signal<bool> vc0_Lrok_wire;
    sc_signal<bool> vc0_Nrok_wire;
    sc_signal<bool> vc0_Erok_wire;
    sc_signal<bool> vc0_Srok_wire;
    sc_signal<bool> vc0_Wrok_wire;

    sc_signal<bool> vc1_Lrok_wire;
    sc_signal<bool> vc1_Nrok_wire;
    sc_signal<bool> vc1_Erok_wire;
    sc_signal<bool> vc1_Srok_wire;
    sc_signal<bool> vc1_Wrok_wire;

    // Read Command
    sc_signal<bool> vc0_Lrd_wire;
    sc_signal<bool> vc0_Nrd_wire;
    sc_signal<bool> vc0_Erd_wire;
    sc_signal<bool> vc0_Srd_wire;
    sc_signal<bool> vc0_Wrd_wire;

    sc_signal<bool> vc1_Lrd_wire;
    sc_signal<bool> vc1_Nrd_wire;
    sc_signal<bool> vc1_Erd_wire;
    sc_signal<bool> vc1_Srd_wire;
    sc_signal<bool> vc1_Wrd_wire;

    // Idle status
    sc_signal<bool> vc0_Lidle_wire;
    sc_signal<bool> vc0_Nidle_wire;
    sc_signal<bool> vc0_Eidle_wire;
    sc_signal<bool> vc0_Sidle_wire;
    sc_signal<bool> vc0_Widle_wire;

    sc_signal<bool> vc1_Lidle_wire;
    sc_signal<bool> vc1_Nidle_wire;
    sc_signal<bool> vc1_Eidle_wire;
    sc_signal<bool> vc1_Sidle_wire;
    sc_signal<bool> vc1_Widle_wire;

    // Data buses
    sc_signal<sc_uint<FLIT_WIDTH> > vc0_Ldata_wire;
    sc_signal<sc_uint<FLIT_WIDTH> > vc0_Ndata_wire;
    sc_signal<sc_uint<FLIT_WIDTH> > vc0_Edata_wire;
    sc_signal<sc_uint<FLIT_WIDTH> > vc0_Sdata_wire;
    sc_signal<sc_uint<FLIT_WIDTH> > vc0_Wdata_wire;

    sc_signal<sc_uint<FLIT_WIDTH> > vc1_Ldata_wire;
    sc_signal<sc_uint<FLIT_WIDTH> > vc1_Ndata_wire;
    sc_signal<sc_uint<FLIT_WIDTH> > vc1_Edata_wire;
    sc_signal<sc_uint<FLIT_WIDTH> > vc1_Sdata_wire;
    sc_signal<sc_uint<FLIT_WIDTH> > vc1_Wdata_wire;


    // Module's process
#ifdef DEBUG_PARIS_INTERCONNECT
    void p_debug () {
        cout << "\n Lreq[]=";
        cout << vc0_LreqN_wire.read();
        cout << vc0_LreqE_wire.read();
        cout << vc0_LreqS_wire.read();
        cout << vc0_LreqW_wire.read();
        cout << vc1_LreqN_wire.read();
        cout << vc1_LreqE_wire.read();
        cout << vc1_LreqS_wire.read();
        cout << vc1_LreqW_wire.read();


        cout << " Lrok=" << Lrok_wire.read();

        cout << " Wgnt[]=";
        cout << vc0_WgntL_wire.read();
        cout << vc0_WgntN_wire.read();
        cout << vc0_WgntE_wire.read();
        cout << vc0_WgntS_wire.read();
        cout << vc1_WgntL_wire.read();
        cout << vc1_WgntN_wire.read();
        cout << vc1_WgntE_wire.read();
        cout << vc1_WgntS_wire.read();

        cout << " Wrd="	<< Wrd_wire.read();
    }
#endif


    // Internal data structures
    unsigned short int USE_LOCAL, USE_NORTH, USE_EAST, USE_SOUTH, USE_WEST;
    unsigned short int XID, YID;

    SC_HAS_PROCESS(paris_2vc);

    //////////////////////////////////////////////////////////////////////////////
    paris_2vc(sc_module_name nm,
              unsigned short int USE_LOCAL,
              unsigned short int USE_NORTH,
              unsigned short int USE_EAST,
              unsigned short int USE_SOUTH,
              unsigned short int USE_WEST,
              unsigned short int XID,
              unsigned short int YID) :
        sc_module(nm),
        USE_LOCAL(USE_LOCAL),
        USE_NORTH(USE_NORTH),
        USE_EAST(USE_EAST),
        USE_SOUTH(USE_SOUTH),
        USE_WEST(USE_WEST),
        XID(XID),
        YID(YID)

    //////////////////////////////////////////////////////////////////////////////
    {
#ifdef DEBUG_PARIS_INTERCONNECT
        SC_METHOD(p_debug);
        sensitive << clk.pos();
#endif

        ////////////////////////////////////////////////////////////////////////////
        xin_2vc *Lin = new xin_2vc("Lin", USE_LOCAL, XID, YID, LOCAL_ID);
        ////////////////////////////////////////////////////////////////////////////
        Lin->clk(clk);
        Lin->rst(rst);
        Lin->in_data    (Lin_data);
        Lin->vc0_in_val (Lin_vc0_val);
        Lin->vc0_in_ret (Lin_vc0_ret);
        Lin->vc1_in_val (Lin_vc1_val);
        Lin->vc1_in_ret (Lin_vc1_ret);

        Lin->vc0_x_reqL (vc0_Lunused_wire);
        Lin->vc0_x_reqN (vc0_LreqN_wire);
        Lin->vc0_x_reqE (vc0_LreqE_wire);
        Lin->vc0_x_reqS (vc0_LreqS_wire);
        Lin->vc0_x_reqW (vc0_LreqW_wire);
        Lin->vc0_x_rok  (vc0_Lrok_wire);
        Lin->vc0_x_rd0  (vc0_Nrd_wire);
        Lin->vc0_x_rd1  (vc0_Erd_wire);
        Lin->vc0_x_rd2  (vc0_Srd_wire);
        Lin->vc0_x_rd3  (vc0_Wrd_wire);
        Lin->vc0_x_gnt0 (vc0_NgntL_wire);
        Lin->vc0_x_gnt1 (vc0_EgntL_wire);
        Lin->vc0_x_gnt2 (vc0_SgntL_wire);
        Lin->vc0_x_gnt3 (vc0_WgntL_wire);
        Lin->vc0_x_Lidle(vc0_Lidle_wire);
        Lin->vc0_x_Nidle(vc0_Nidle_wire);
        Lin->vc0_x_Eidle(vc0_Eidle_wire);
        Lin->vc0_x_Sidle(vc0_Sidle_wire);
        Lin->vc0_x_Widle(vc0_Widle_wire);
        Lin->vc0_x_dout (vc0_Ldata_wire);

        Lin->vc1_x_reqL (vc1_Lunused_wire);
        Lin->vc1_x_reqN (vc1_LreqN_wire);
        Lin->vc1_x_reqE (vc1_LreqE_wire);
        Lin->vc1_x_reqS (vc1_LreqS_wire);
        Lin->vc1_x_reqW (vc1_LreqW_wire);
        Lin->vc1_x_rok  (vc1_Lrok_wire);
        Lin->vc1_x_rd0  (vc1_Nrd_wire);
        Lin->vc1_x_rd1  (vc1_Erd_wire);
        Lin->vc1_x_rd2  (vc1_Srd_wire);
        Lin->vc1_x_rd3  (vc1_Wrd_wire);
        Lin->vc1_x_gnt0 (vc1_NgntL_wire);
        Lin->vc1_x_gnt1 (vc1_EgntL_wire);
        Lin->vc1_x_gnt2 (vc1_SgntL_wire);
        Lin->vc1_x_gnt3 (vc1_WgntL_wire);
        Lin->vc1_x_Lidle(vc1_Lidle_wire);
        Lin->vc1_x_Nidle(vc1_Nidle_wire);
        Lin->vc1_x_Eidle(vc1_Eidle_wire);
        Lin->vc1_x_Sidle(vc1_Sidle_wire);
        Lin->vc1_x_Widle(vc1_Widle_wire);
        Lin->vc1_x_dout (vc1_Ldata_wire);


        ////////////////////////////////////////////////////////////////////////////
        xin_2vc *Nin = new xin_2vc("Nin", USE_NORTH, XID, YID, NORTH_ID);
        ////////////////////////////////////////////////////////////////////////////
        Nin->clk(clk);
        Nin->rst(rst);
        Nin->in_data    (Nin_data);
        Nin->vc0_in_val (Nin_vc0_val);
        Nin->vc0_in_ret (Nin_vc0_ret);
        Nin->vc1_in_val (Nin_vc1_val);
        Nin->vc1_in_ret (Nin_vc1_ret);

        Nin->vc0_x_reqL (vc0_NreqL_wire);
        Nin->vc0_x_reqN (vc0_Nunused_wire);
        Nin->vc0_x_reqE (vc0_NreqE_wire);
        Nin->vc0_x_reqS (vc0_NreqS_wire);
        Nin->vc0_x_reqW (vc0_NreqW_wire);
        Nin->vc0_x_rok  (vc0_Nrok_wire);
        Nin->vc0_x_rd0  (vc0_Lrd_wire);
        Nin->vc0_x_rd1  (vc0_Erd_wire);
        Nin->vc0_x_rd2  (vc0_Srd_wire);
        Nin->vc0_x_rd3  (vc0_Wrd_wire);
        Nin->vc0_x_gnt0 (vc0_LgntN_wire);
        Nin->vc0_x_gnt1 (vc0_EgntN_wire);
        Nin->vc0_x_gnt2 (vc0_SgntN_wire);
        Nin->vc0_x_gnt3 (vc0_WgntN_wire);
        Nin->vc0_x_Lidle(vc0_Lidle_wire);
        Nin->vc0_x_Nidle(vc0_Nidle_wire);
        Nin->vc0_x_Eidle(vc0_Eidle_wire);
        Nin->vc0_x_Sidle(vc0_Sidle_wire);
        Nin->vc0_x_Widle(vc0_Widle_wire);
        Nin->vc0_x_dout (vc0_Ndata_wire);

        Nin->vc1_x_reqL (vc1_NreqL_wire);
        Nin->vc1_x_reqN (vc1_Nunused_wire);
        Nin->vc1_x_reqE (vc1_NreqE_wire);
        Nin->vc1_x_reqS (vc1_NreqS_wire);
        Nin->vc1_x_reqW (vc1_NreqW_wire);
        Nin->vc1_x_rok  (vc1_Nrok_wire);
        Nin->vc1_x_rd0  (vc1_Lrd_wire);
        Nin->vc1_x_rd1  (vc1_Erd_wire);
        Nin->vc1_x_rd2  (vc1_Srd_wire);
        Nin->vc1_x_rd3  (vc1_Wrd_wire);
        Nin->vc1_x_gnt0 (vc1_LgntN_wire);
        Nin->vc1_x_gnt1 (vc1_EgntN_wire);
        Nin->vc1_x_gnt2 (vc1_SgntN_wire);
        Nin->vc1_x_gnt3 (vc1_WgntN_wire);
        Nin->vc1_x_Lidle(vc1_Lidle_wire);
        Nin->vc1_x_Nidle(vc1_Nidle_wire);
        Nin->vc1_x_Eidle(vc1_Eidle_wire);
        Nin->vc1_x_Sidle(vc1_Sidle_wire);
        Nin->vc1_x_Widle(vc1_Widle_wire);
        Nin->vc1_x_dout (vc1_Ndata_wire);


        ////////////////////////////////////////////////////////////////////////////
        xin_2vc *Ein = new xin_2vc("Ein", USE_EAST, XID, YID, EAST_ID);
        ////////////////////////////////////////////////////////////////////////////
        Ein->clk(clk);
        Ein->rst(rst);
        Ein->in_data    (Ein_data);
        Ein->vc0_in_val (Ein_vc0_val);
        Ein->vc0_in_ret (Ein_vc0_ret);
        Ein->vc1_in_val (Ein_vc1_val);
        Ein->vc1_in_ret (Ein_vc1_ret);

        Ein->vc0_x_reqL (vc0_EreqL_wire);
        Ein->vc0_x_reqN (vc0_EreqN_wire);
        Ein->vc0_x_reqE (vc0_Eunused_wire);
        Ein->vc0_x_reqS (vc0_EreqS_wire);
        Ein->vc0_x_reqW (vc0_EreqW_wire);
        Ein->vc0_x_rok  (vc0_Erok_wire);
        Ein->vc0_x_rd0  (vc0_Lrd_wire);
        Ein->vc0_x_rd1  (vc0_Nrd_wire);
        Ein->vc0_x_rd2  (vc0_Srd_wire);
        Ein->vc0_x_rd3  (vc0_Wrd_wire);
        Ein->vc0_x_gnt0 (vc0_LgntE_wire);
        Ein->vc0_x_gnt1 (vc0_NgntE_wire);
        Ein->vc0_x_gnt2 (vc0_SgntE_wire);
        Ein->vc0_x_gnt3 (vc0_WgntE_wire);
        Ein->vc0_x_Lidle(vc0_Lidle_wire);
        Ein->vc0_x_Nidle(vc0_Nidle_wire);
        Ein->vc0_x_Eidle(vc0_Eidle_wire);
        Ein->vc0_x_Sidle(vc0_Sidle_wire);
        Ein->vc0_x_Widle(vc0_Widle_wire);
        Ein->vc0_x_dout (vc0_Edata_wire);

        Ein->vc1_x_reqL (vc1_EreqL_wire);
        Ein->vc1_x_reqN (vc1_EreqN_wire);
        Ein->vc1_x_reqE (vc1_Eunused_wire);
        Ein->vc1_x_reqS (vc1_EreqS_wire);
        Ein->vc1_x_reqW (vc1_EreqW_wire);
        Ein->vc1_x_rok  (vc1_Erok_wire);
        Ein->vc1_x_rd0  (vc1_Lrd_wire);
        Ein->vc1_x_rd1  (vc1_Nrd_wire);
        Ein->vc1_x_rd2  (vc1_Srd_wire);
        Ein->vc1_x_rd3  (vc1_Wrd_wire);
        Ein->vc1_x_gnt0 (vc1_LgntE_wire);
        Ein->vc1_x_gnt1 (vc1_NgntE_wire);
        Ein->vc1_x_gnt2 (vc1_SgntE_wire);
        Ein->vc1_x_gnt3 (vc1_WgntE_wire);
        Ein->vc1_x_Lidle(vc1_Lidle_wire);
        Ein->vc1_x_Nidle(vc1_Nidle_wire);
        Ein->vc1_x_Eidle(vc1_Eidle_wire);
        Ein->vc1_x_Sidle(vc1_Sidle_wire);
        Ein->vc1_x_Widle(vc1_Widle_wire);
        Ein->vc1_x_dout (vc1_Edata_wire);

        ////////////////////////////////////////////////////////////////////////////
        xin_2vc *Sin = new xin_2vc("Sin", USE_SOUTH, XID, YID, SOUTH_ID);
        ////////////////////////////////////////////////////////////////////////////
        Sin->clk(clk);
        Sin->rst(rst);
        Sin->in_data    (Sin_data);
        Sin->vc0_in_val (Sin_vc0_val);
        Sin->vc0_in_ret (Sin_vc0_ret);
        Sin->vc1_in_val (Sin_vc1_val);
        Sin->vc1_in_ret (Sin_vc1_ret);

        Sin->vc0_x_reqL (vc0_SreqL_wire);
        Sin->vc0_x_reqN (vc0_SreqN_wire);
        Sin->vc0_x_reqE (vc0_SreqE_wire);
        Sin->vc0_x_reqS (vc0_Sunused_wire);
        Sin->vc0_x_reqW (vc0_SreqW_wire);
        Sin->vc0_x_rok  (vc0_Srok_wire);
        Sin->vc0_x_rd0  (vc0_Lrd_wire);
        Sin->vc0_x_rd1  (vc0_Nrd_wire);
        Sin->vc0_x_rd2  (vc0_Erd_wire);
        Sin->vc0_x_rd3  (vc0_Wrd_wire);
        Sin->vc0_x_gnt0 (vc0_LgntS_wire);
        Sin->vc0_x_gnt1 (vc0_NgntS_wire);
        Sin->vc0_x_gnt2 (vc0_EgntS_wire);
        Sin->vc0_x_gnt3 (vc0_WgntS_wire);
        Sin->vc0_x_Lidle(vc0_Lidle_wire);
        Sin->vc0_x_Nidle(vc0_Nidle_wire);
        Sin->vc0_x_Eidle(vc0_Eidle_wire);
        Sin->vc0_x_Sidle(vc0_Sidle_wire);
        Sin->vc0_x_Widle(vc0_Widle_wire);
        Sin->vc0_x_dout (vc0_Sdata_wire);

        Sin->vc1_x_reqL (vc1_SreqL_wire);
        Sin->vc1_x_reqN (vc1_SreqN_wire);
        Sin->vc1_x_reqE (vc1_SreqE_wire);
        Sin->vc1_x_reqS (vc1_Sunused_wire);
        Sin->vc1_x_reqW (vc1_SreqW_wire);
        Sin->vc1_x_rok  (vc1_Srok_wire);
        Sin->vc1_x_rd0  (vc1_Lrd_wire);
        Sin->vc1_x_rd1  (vc1_Nrd_wire);
        Sin->vc1_x_rd2  (vc1_Erd_wire);
        Sin->vc1_x_rd3  (vc1_Wrd_wire);
        Sin->vc1_x_gnt0 (vc1_LgntS_wire);
        Sin->vc1_x_gnt1 (vc1_NgntS_wire);
        Sin->vc1_x_gnt2 (vc1_EgntS_wire);
        Sin->vc1_x_gnt3 (vc1_WgntS_wire);
        Sin->vc1_x_Lidle(vc1_Lidle_wire);
        Sin->vc1_x_Nidle(vc1_Nidle_wire);
        Sin->vc1_x_Eidle(vc1_Eidle_wire);
        Sin->vc1_x_Sidle(vc1_Sidle_wire);
        Sin->vc1_x_Widle(vc1_Widle_wire);
        Sin->vc1_x_dout (vc1_Sdata_wire);


        ////////////////////////////////////////////////////////////////////////////
        xin_2vc *Win = new xin_2vc("Win", USE_WEST, XID, YID, WEST_ID);
        ////////////////////////////////////////////////////////////////////////////
        Win->clk(clk);
        Win->rst(rst);
        Win->in_data    (Win_data);
        Win->vc0_in_val (Win_vc0_val);
        Win->vc0_in_ret (Win_vc0_ret);
        Win->vc1_in_val (Win_vc1_val);
        Win->vc1_in_ret (Win_vc1_ret);

        Win->vc0_x_reqL (vc0_WreqL_wire);
        Win->vc0_x_reqN (vc0_WreqN_wire);
        Win->vc0_x_reqE (vc0_WreqE_wire);
        Win->vc0_x_reqS (vc0_WreqS_wire);
        Win->vc0_x_reqW (vc0_Wunused_wire);
        Win->vc0_x_rok  (vc0_Wrok_wire);
        Win->vc0_x_rd0  (vc0_Lrd_wire);
        Win->vc0_x_rd1  (vc0_Nrd_wire);
        Win->vc0_x_rd2  (vc0_Erd_wire);
        Win->vc0_x_rd3  (vc0_Srd_wire);
        Win->vc0_x_gnt0 (vc0_LgntW_wire);
        Win->vc0_x_gnt1 (vc0_NgntW_wire);
        Win->vc0_x_gnt2 (vc0_EgntW_wire);
        Win->vc0_x_gnt3 (vc0_SgntW_wire);
        Win->vc0_x_Lidle(vc0_Lidle_wire);
        Win->vc0_x_Nidle(vc0_Nidle_wire);
        Win->vc0_x_Eidle(vc0_Eidle_wire);
        Win->vc0_x_Sidle(vc0_Sidle_wire);
        Win->vc0_x_Widle(vc0_Widle_wire);
        Win->vc0_x_dout (vc0_Wdata_wire);

        Win->vc1_x_reqL (vc1_WreqL_wire);
        Win->vc1_x_reqN (vc1_WreqN_wire);
        Win->vc1_x_reqE (vc1_WreqE_wire);
        Win->vc1_x_reqS (vc1_WreqS_wire);
        Win->vc1_x_reqW (vc1_Wunused_wire);
        Win->vc1_x_rok  (vc1_Wrok_wire);
        Win->vc1_x_rd0  (vc1_Lrd_wire);
        Win->vc1_x_rd1  (vc1_Nrd_wire);
        Win->vc1_x_rd2  (vc1_Erd_wire);
        Win->vc1_x_rd3  (vc1_Srd_wire);
        Win->vc1_x_gnt0 (vc1_LgntW_wire);
        Win->vc1_x_gnt1 (vc1_NgntW_wire);
        Win->vc1_x_gnt2 (vc1_EgntW_wire);
        Win->vc1_x_gnt3 (vc1_SgntW_wire);
        Win->vc1_x_Lidle(vc1_Lidle_wire);
        Win->vc1_x_Nidle(vc1_Nidle_wire);
        Win->vc1_x_Eidle(vc1_Eidle_wire);
        Win->vc1_x_Sidle(vc1_Sidle_wire);
        Win->vc1_x_Widle(vc1_Widle_wire);
        Win->vc1_x_dout (vc1_Wdata_wire);


        ////////////////////////////////////////////////////////////////////////////
        xout_2vc *Lout = new xout_2vc("Lout", USE_LOCAL, XID, YID, LOCAL_ID);
        ////////////////////////////////////////////////////////////////////////////
        Lout->clk(clk);
        Lout->rst(rst);
        Lout->out_data(Lout_data);
        Lout->vc0_out_val(Lout_vc0_val);
        Lout->vc0_out_ret(Lout_vc0_ret);
        Lout->vc1_out_val(Lout_vc1_val);
        Lout->vc1_out_ret(Lout_vc1_ret);

        Lout->vc0_x_req0 (vc0_NreqL_wire);
        Lout->vc0_x_req1 (vc0_EreqL_wire);
        Lout->vc0_x_req2 (vc0_SreqL_wire);
        Lout->vc0_x_req3 (vc0_WreqL_wire);
        Lout->vc0_x_rok0 (vc0_Nrok_wire);
        Lout->vc0_x_rok1 (vc0_Erok_wire);
        Lout->vc0_x_rok2 (vc0_Srok_wire);
        Lout->vc0_x_rok3 (vc0_Wrok_wire);
        Lout->vc0_x_gnt0 (vc0_LgntN_wire);
        Lout->vc0_x_gnt1 (vc0_LgntE_wire);
        Lout->vc0_x_gnt2 (vc0_LgntS_wire);
        Lout->vc0_x_gnt3 (vc0_LgntW_wire);
        Lout->vc0_x_idle (vc0_Lidle_wire);
        Lout->vc0_x_rd   (vc0_Lrd_wire);
        Lout->vc0_x_din0 (vc0_Ndata_wire);
        Lout->vc0_x_din1 (vc0_Edata_wire);
        Lout->vc0_x_din2 (vc0_Sdata_wire);
        Lout->vc0_x_din3 (vc0_Wdata_wire);

        Lout->vc1_x_req0 (vc1_NreqL_wire);
        Lout->vc1_x_req1 (vc1_EreqL_wire);
        Lout->vc1_x_req2 (vc1_SreqL_wire);
        Lout->vc1_x_req3 (vc1_WreqL_wire);
        Lout->vc1_x_rok0 (vc1_Nrok_wire);
        Lout->vc1_x_rok1 (vc1_Erok_wire);
        Lout->vc1_x_rok2 (vc1_Srok_wire);
        Lout->vc1_x_rok3 (vc1_Wrok_wire);
        Lout->vc1_x_gnt0 (vc1_LgntN_wire);
        Lout->vc1_x_gnt1 (vc1_LgntE_wire);
        Lout->vc1_x_gnt2 (vc1_LgntS_wire);
        Lout->vc1_x_gnt3 (vc1_LgntW_wire);
        Lout->vc1_x_idle (vc1_Lidle_wire);
        Lout->vc1_x_rd   (vc1_Lrd_wire);
        Lout->vc1_x_din0 (vc1_Ndata_wire);
        Lout->vc1_x_din1 (vc1_Edata_wire);
        Lout->vc1_x_din2 (vc1_Sdata_wire);
        Lout->vc1_x_din3 (vc1_Wdata_wire);

        ////////////////////////////////////////////////////////////////////////////
        xout_2vc *Nout = new xout_2vc("Nout", USE_NORTH, XID, YID, NORTH_ID);
        ////////////////////////////////////////////////////////////////////////////
        Nout->clk(clk);
        Nout->rst(rst);
        Nout->out_data(Nout_data);
        Nout->vc0_out_val(Nout_vc0_val);
        Nout->vc0_out_ret(Nout_vc0_ret);
        Nout->vc1_out_val(Nout_vc1_val);
        Nout->vc1_out_ret(Nout_vc1_ret);

        Nout->vc0_x_req0 (vc0_LreqN_wire);
        Nout->vc0_x_req1 (vc0_EreqN_wire);
        Nout->vc0_x_req2 (vc0_SreqN_wire);
        Nout->vc0_x_req3 (vc0_WreqN_wire);
        Nout->vc0_x_rok0 (vc0_Lrok_wire);
        Nout->vc0_x_rok1 (vc0_Erok_wire);
        Nout->vc0_x_rok2 (vc0_Srok_wire);
        Nout->vc0_x_rok3 (vc0_Wrok_wire);
        Nout->vc0_x_gnt0 (vc0_NgntL_wire);
        Nout->vc0_x_gnt1 (vc0_NgntE_wire);
        Nout->vc0_x_gnt2 (vc0_NgntS_wire);
        Nout->vc0_x_gnt3 (vc0_NgntW_wire);
        Nout->vc0_x_idle (vc0_Nidle_wire);
        Nout->vc0_x_rd   (vc0_Nrd_wire);
        Nout->vc0_x_din0 (vc0_Ldata_wire);
        Nout->vc0_x_din1 (vc0_Edata_wire);
        Nout->vc0_x_din2 (vc0_Sdata_wire);
        Nout->vc0_x_din3 (vc0_Wdata_wire);

        Nout->vc1_x_req0 (vc1_LreqN_wire);
        Nout->vc1_x_req1 (vc1_EreqN_wire);
        Nout->vc1_x_req2 (vc1_SreqN_wire);
        Nout->vc1_x_req3 (vc1_WreqN_wire);
        Nout->vc1_x_rok0 (vc1_Lrok_wire);
        Nout->vc1_x_rok1 (vc1_Erok_wire);
        Nout->vc1_x_rok2 (vc1_Srok_wire);
        Nout->vc1_x_rok3 (vc1_Wrok_wire);
        Nout->vc1_x_gnt0 (vc1_NgntL_wire);
        Nout->vc1_x_gnt1 (vc1_NgntE_wire);
        Nout->vc1_x_gnt2 (vc1_NgntS_wire);
        Nout->vc1_x_gnt3 (vc1_NgntW_wire);
        Nout->vc1_x_idle (vc1_Nidle_wire);
        Nout->vc1_x_rd   (vc1_Nrd_wire);
        Nout->vc1_x_din0 (vc1_Ldata_wire);
        Nout->vc1_x_din1 (vc1_Edata_wire);
        Nout->vc1_x_din2 (vc1_Sdata_wire);
        Nout->vc1_x_din3 (vc1_Wdata_wire);

        ////////////////////////////////////////////////////////////////////////////
        xout_2vc *Eout = new xout_2vc("Eout", USE_EAST, XID, YID, EAST_ID);
        ////////////////////////////////////////////////////////////////////////////
        Eout->clk(clk);
        Eout->rst(rst);
        Eout->out_data   (Eout_data);
        Eout->vc0_out_val(Eout_vc0_val);
        Eout->vc0_out_ret(Eout_vc0_ret);
        Eout->vc1_out_val(Eout_vc1_val);
        Eout->vc1_out_ret(Eout_vc1_ret);

        Eout->vc0_x_req0 (vc0_LreqE_wire);
        Eout->vc0_x_req1 (vc0_NreqE_wire);
        Eout->vc0_x_req2 (vc0_SreqE_wire);
        Eout->vc0_x_req3 (vc0_WreqE_wire);
        Eout->vc0_x_rok0 (vc0_Lrok_wire);
        Eout->vc0_x_rok1 (vc0_Nrok_wire);
        Eout->vc0_x_rok2 (vc0_Srok_wire);
        Eout->vc0_x_rok3 (vc0_Wrok_wire);
        Eout->vc0_x_gnt0 (vc0_EgntL_wire);
        Eout->vc0_x_gnt1 (vc0_EgntN_wire);
        Eout->vc0_x_gnt2 (vc0_EgntS_wire);
        Eout->vc0_x_gnt3 (vc0_EgntW_wire);
        Eout->vc0_x_idle (vc0_Eidle_wire);
        Eout->vc0_x_rd   (vc0_Erd_wire);
        Eout->vc0_x_din0 (vc0_Ldata_wire);
        Eout->vc0_x_din1 (vc0_Ndata_wire);
        Eout->vc0_x_din2 (vc0_Sdata_wire);
        Eout->vc0_x_din3 (vc0_Wdata_wire);

        Eout->vc1_x_req0 (vc1_LreqE_wire);
        Eout->vc1_x_req1 (vc1_NreqE_wire);
        Eout->vc1_x_req2 (vc1_SreqE_wire);
        Eout->vc1_x_req3 (vc1_WreqE_wire);
        Eout->vc1_x_rok0 (vc1_Lrok_wire);
        Eout->vc1_x_rok1 (vc1_Nrok_wire);
        Eout->vc1_x_rok2 (vc1_Srok_wire);
        Eout->vc1_x_rok3 (vc1_Wrok_wire);
        Eout->vc1_x_gnt0 (vc1_EgntL_wire);
        Eout->vc1_x_gnt1 (vc1_EgntN_wire);
        Eout->vc1_x_gnt2 (vc1_EgntS_wire);
        Eout->vc1_x_gnt3 (vc1_EgntW_wire);
        Eout->vc1_x_idle (vc1_Eidle_wire);
        Eout->vc1_x_rd   (vc1_Erd_wire);
        Eout->vc1_x_din0 (vc1_Ldata_wire);
        Eout->vc1_x_din1 (vc1_Ndata_wire);
        Eout->vc1_x_din2 (vc1_Sdata_wire);
        Eout->vc1_x_din3 (vc1_Wdata_wire);

        ////////////////////////////////////////////////////////////////////////////
        xout_2vc *Sout = new xout_2vc("Sout", USE_SOUTH, XID, YID, SOUTH_ID);
        ////////////////////////////////////////////////////////////////////////////
        Sout->clk(clk);
        Sout->rst(rst);
        Sout->out_data   (Sout_data);
        Sout->vc0_out_val(Sout_vc0_val);
        Sout->vc0_out_ret(Sout_vc0_ret);
        Sout->vc1_out_val(Sout_vc1_val);
        Sout->vc1_out_ret(Sout_vc1_ret);

        Sout->vc0_x_req0 (vc0_LreqS_wire);
        Sout->vc0_x_req1 (vc0_NreqS_wire);
        Sout->vc0_x_req2 (vc0_EreqS_wire);
        Sout->vc0_x_req3 (vc0_WreqS_wire);
        Sout->vc0_x_rok0 (vc0_Lrok_wire);
        Sout->vc0_x_rok1 (vc0_Nrok_wire);
        Sout->vc0_x_rok2 (vc0_Erok_wire);
        Sout->vc0_x_rok3 (vc0_Wrok_wire);
        Sout->vc0_x_gnt0 (vc0_SgntL_wire);
        Sout->vc0_x_gnt1 (vc0_SgntN_wire);
        Sout->vc0_x_gnt2 (vc0_SgntE_wire);
        Sout->vc0_x_gnt3 (vc0_SgntW_wire);
        Sout->vc0_x_idle (vc0_Sidle_wire);
        Sout->vc0_x_rd   (vc0_Srd_wire);
        Sout->vc0_x_din0 (vc0_Ldata_wire);
        Sout->vc0_x_din1 (vc0_Ndata_wire);
        Sout->vc0_x_din2 (vc0_Edata_wire);
        Sout->vc0_x_din3 (vc0_Wdata_wire);

        Sout->vc1_x_req0 (vc1_LreqS_wire);
        Sout->vc1_x_req1 (vc1_NreqS_wire);
        Sout->vc1_x_req2 (vc1_EreqS_wire);
        Sout->vc1_x_req3 (vc1_WreqS_wire);
        Sout->vc1_x_rok0 (vc1_Lrok_wire);
        Sout->vc1_x_rok1 (vc1_Nrok_wire);
        Sout->vc1_x_rok2 (vc1_Erok_wire);
        Sout->vc1_x_rok3 (vc1_Wrok_wire);
        Sout->vc1_x_gnt0 (vc1_SgntL_wire);
        Sout->vc1_x_gnt1 (vc1_SgntN_wire);
        Sout->vc1_x_gnt2 (vc1_SgntE_wire);
        Sout->vc1_x_gnt3 (vc1_SgntW_wire);
        Sout->vc1_x_idle (vc1_Sidle_wire);
        Sout->vc1_x_rd   (vc1_Srd_wire);
        Sout->vc1_x_din0 (vc1_Ldata_wire);
        Sout->vc1_x_din1 (vc1_Ndata_wire);
        Sout->vc1_x_din2 (vc1_Edata_wire);
        Sout->vc1_x_din3 (vc1_Wdata_wire);

        ////////////////////////////////////////////////////////////////////////////
        xout_2vc *Wout = new xout_2vc("Wout", USE_WEST, XID, YID, WEST_ID);
        ////////////////////////////////////////////////////////////////////////////
        Wout->clk(clk);
        Wout->rst(rst);
        Wout->out_data   (Wout_data);
        Wout->vc0_out_val(Wout_vc0_val);
        Wout->vc0_out_ret(Wout_vc0_ret);
        Wout->vc1_out_val(Wout_vc1_val);
        Wout->vc1_out_ret(Wout_vc1_ret);

        Wout->vc0_x_req0(vc0_LreqW_wire);
        Wout->vc0_x_req1(vc0_NreqW_wire);
        Wout->vc0_x_req2(vc0_EreqW_wire);
        Wout->vc0_x_req3(vc0_SreqW_wire);
        Wout->vc0_x_rok0(vc0_Lrok_wire);
        Wout->vc0_x_rok1(vc0_Nrok_wire);
        Wout->vc0_x_rok2(vc0_Erok_wire);
        Wout->vc0_x_rok3(vc0_Srok_wire);
        Wout->vc0_x_gnt0(vc0_WgntL_wire);
        Wout->vc0_x_gnt1(vc0_WgntN_wire);
        Wout->vc0_x_gnt2(vc0_WgntE_wire);
        Wout->vc0_x_gnt3(vc0_WgntS_wire);
        Wout->vc0_x_idle(vc0_Widle_wire);
        Wout->vc0_x_rd  (vc0_Wrd_wire);
        Wout->vc0_x_din0(vc0_Ldata_wire);
        Wout->vc0_x_din1(vc0_Ndata_wire);
        Wout->vc0_x_din2(vc0_Edata_wire);
        Wout->vc0_x_din3(vc0_Sdata_wire);

        Wout->vc1_x_req0(vc1_LreqW_wire);
        Wout->vc1_x_req1(vc1_NreqW_wire);
        Wout->vc1_x_req2(vc1_EreqW_wire);
        Wout->vc1_x_req3(vc1_SreqW_wire);
        Wout->vc1_x_rok0(vc1_Lrok_wire);
        Wout->vc1_x_rok1(vc1_Nrok_wire);
        Wout->vc1_x_rok2(vc1_Erok_wire);
        Wout->vc1_x_rok3(vc1_Srok_wire);
        Wout->vc1_x_gnt0(vc1_WgntL_wire);
        Wout->vc1_x_gnt1(vc1_WgntN_wire);
        Wout->vc1_x_gnt2(vc1_WgntE_wire);
        Wout->vc1_x_gnt3(vc1_WgntS_wire);
        Wout->vc1_x_idle(vc1_Widle_wire);
        Wout->vc1_x_rd  (vc1_Wrd_wire);
        Wout->vc1_x_din0(vc1_Ldata_wire);
        Wout->vc1_x_din1(vc1_Ndata_wire);
        Wout->vc1_x_din2(vc1_Edata_wire);
        Wout->vc1_x_din3(vc1_Sdata_wire);
    }
};

#endif // __PARIS_2VC_H__

