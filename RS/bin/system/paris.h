/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : paris (parameterizable interconnect switch)
FILE   : paris.h
--------------------------------------------------------------------------------
DESCRIPTION: ParIS router
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distribution System (LSED - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
--------------------------------------------------------------------------------
Last update on: August 21th, 2006.
--------------------------------------------------------------------------------
*/
#ifndef __PARIS_H__
#define __PARIS_H__

#include <systemc.h>
#include "parameters.h"
#include "xin.h"
#include "xout.h"


SC_MODULE(paris)
{
    // INTERFACE
    // System signals
    sc_in_clk   clk;
    sc_in<bool> rst;

    // Local Communication Port
    sc_in<sc_uint<FLIT_WIDTH> >  Lin_data;
    sc_in<bool>                  Lin_val;
    sc_out<bool>                 Lin_ret;
    sc_out<sc_uint<FLIT_WIDTH> > Lout_data;
    sc_out<bool>                 Lout_val;
    sc_in<bool>                  Lout_ret;

    // North Communication Port
    sc_in<sc_uint<FLIT_WIDTH> >  Nin_data;
    sc_in<bool>                  Nin_val;
    sc_out<bool>                 Nin_ret;
    sc_out<sc_uint<FLIT_WIDTH> > Nout_data;
    sc_out<bool>                 Nout_val;
    sc_in<bool>                  Nout_ret;

    // East Communication Port
    sc_in<sc_uint<FLIT_WIDTH> >  Ein_data;
    sc_in<bool>  				 Ein_val;
    sc_out<bool>                 Ein_ret;
    sc_out<sc_uint<FLIT_WIDTH> > Eout_data;
    sc_out<bool>                 Eout_val;
    sc_in<bool>                  Eout_ret;

    // South Communication Port
    sc_in<sc_uint<FLIT_WIDTH> >  Sin_data;
    sc_in<bool>                  Sin_val;
    sc_out<bool>                 Sin_ret;
    sc_out<sc_uint<FLIT_WIDTH> > Sout_data;
    sc_out<bool>                 Sout_val;
    sc_in<bool>                  Sout_ret;

    // West Communication Port
    sc_in<sc_uint<FLIT_WIDTH> >  Win_data;
    sc_in<bool>                  Win_val;
    sc_out<bool>                 Win_ret;
    sc_out<sc_uint<FLIT_WIDTH> > Wout_data;
    sc_out<bool>                 Wout_val;
    sc_in<bool>                  Wout_ret;

    // SIGNALS
    // Requests from L
    sc_signal<bool> Lunused_wire;
    sc_signal<bool> LreqN_wire;
    sc_signal<bool> LreqE_wire;
    sc_signal<bool> LreqS_wire;
    sc_signal<bool> LreqW_wire;

    // Requests from N
    sc_signal<bool> Nunused_wire;
    sc_signal<bool> NreqL_wire;
    sc_signal<bool> NreqE_wire;
    sc_signal<bool> NreqS_wire;
    sc_signal<bool> NreqW_wire;

    // Requests from E
    sc_signal<bool> Eunused_wire;
    sc_signal<bool> EreqL_wire;
    sc_signal<bool> EreqN_wire;
    sc_signal<bool> EreqS_wire;
    sc_signal<bool> EreqW_wire;

    // Requests from S
    sc_signal<bool> Sunused_wire;
    sc_signal<bool> SreqL_wire;
    sc_signal<bool> SreqN_wire;
    sc_signal<bool> SreqE_wire;
    sc_signal<bool> SreqW_wire;

    // Requests from W
    sc_signal<bool> Wunused_wire;
    sc_signal<bool> WreqL_wire;
    sc_signal<bool> WreqN_wire;
    sc_signal<bool> WreqE_wire;
    sc_signal<bool> WreqS_wire;

    // Grants from L
    sc_signal<bool> LgntN_wire;
    sc_signal<bool> LgntE_wire;
    sc_signal<bool> LgntS_wire;
    sc_signal<bool> LgntW_wire;

    // Grants from N
    sc_signal<bool> NgntL_wire;
    sc_signal<bool> NgntE_wire;
    sc_signal<bool> NgntS_wire;
    sc_signal<bool> NgntW_wire;

    // Grants from E
    sc_signal<bool> EgntL_wire;
    sc_signal<bool> EgntN_wire;
    sc_signal<bool> EgntS_wire;
    sc_signal<bool> EgntW_wire;

    // Grants from S
    sc_signal<bool> SgntL_wire;
    sc_signal<bool> SgntN_wire;
    sc_signal<bool> SgntE_wire;
    sc_signal<bool> SgntW_wire;

    // Grants from W
    sc_signal<bool> WgntL_wire;
    sc_signal<bool> WgntN_wire;
    sc_signal<bool> WgntE_wire;
    sc_signal<bool> WgntS_wire;


    // Read Status
    sc_signal<bool> Lrok_wire;
    sc_signal<bool> Nrok_wire;
    sc_signal<bool> Erok_wire;
    sc_signal<bool> Srok_wire;
    sc_signal<bool> Wrok_wire;

    // Read Command
    sc_signal<bool> Lrd_wire;
    sc_signal<bool> Nrd_wire;
    sc_signal<bool> Erd_wire;
    sc_signal<bool> Srd_wire;
    sc_signal<bool> Wrd_wire;

    // Idle status
    sc_signal<bool> Lidle_wire;
    sc_signal<bool> Nidle_wire;
    sc_signal<bool> Eidle_wire;
    sc_signal<bool> Sidle_wire;
    sc_signal<bool> Widle_wire;

    // Data buses
    sc_signal<sc_uint<FLIT_WIDTH> > Ldata_wire;
    sc_signal<sc_uint<FLIT_WIDTH> > Ndata_wire;
    sc_signal<sc_uint<FLIT_WIDTH> > Edata_wire;
    sc_signal<sc_uint<FLIT_WIDTH> > Sdata_wire;
    sc_signal<sc_uint<FLIT_WIDTH> > Wdata_wire;

    sc_signal<bool> gnd_wire;

    // Module's process
#ifdef DEBUG_PARIS_INTERCONNECT
    void p_debug () {
        cout << "\n Lreq[]=";
        cout << LreqN_wire.read();
        cout << LreqE_wire.read();
        cout << LreqS_wire.read();
        cout << LreqW_wire.read();

        cout << " Lrok=" <<d Lrok_wire.read();

        cout << " Wgnt[]=";
        cout << WgntL_wire.read();
        cout << WgntN_wire.read();
        cout << WgntE_wire.read();
        cout << WgntS_wire.read();

        cout << " Wrd="	<< Wrd_wire.read();
    }
#endif

    void p_gnd() {
        gnd_wire.write(0);
    }

    // Internal data structures
    unsigned short int USE_LOCAL, USE_NORTH, USE_EAST, USE_SOUTH, USE_WEST;
    unsigned short int XID, YID;

    SC_HAS_PROCESS(paris);

    //////////////////////////////////////////////////////////////////////////////
    paris(sc_module_name nm,
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
        xin *Lin = new xin("Lin", USE_LOCAL, XID, YID, LOCAL_ID);
        ////////////////////////////////////////////////////////////////////////////
        Lin->clk(clk);
        Lin->rst(rst);
        Lin->in_data(Lin_data);
        Lin->in_val(Lin_val);
        Lin->in_ret(Lin_ret);
        Lin->x_reqL(Lunused_wire);
        Lin->x_reqN(LreqN_wire);
        Lin->x_reqE(LreqE_wire);
        Lin->x_reqS(LreqS_wire);
        Lin->x_reqW(LreqW_wire);
        Lin->x_rok(Lrok_wire);
        Lin->x_rd0(Nrd_wire);
        Lin->x_rd1(Erd_wire);
        Lin->x_rd2(Srd_wire);
        Lin->x_rd3(Wrd_wire);
        Lin->x_gnt0(NgntL_wire);
        Lin->x_gnt1(EgntL_wire);
        Lin->x_gnt2(SgntL_wire);
        Lin->x_gnt3(WgntL_wire);
        Lin->x_Lidle(Lidle_wire);
        Lin->x_Nidle(Nidle_wire);
        Lin->x_Eidle(Eidle_wire);
        Lin->x_Sidle(Sidle_wire);
        Lin->x_Widle(Widle_wire);
        Lin->x_dout(Ldata_wire);

        ////////////////////////////////////////////////////////////////////////////
        xin *Nin = new xin("Nin", USE_NORTH, XID, YID, NORTH_ID);
        ////////////////////////////////////////////////////////////////////////////
        Nin->clk(clk);
        Nin->rst(rst);
        Nin->in_data(Nin_data);
        Nin->in_val(Nin_val);
        Nin->in_ret(Nin_ret);
        Nin->x_reqL(NreqL_wire);
        Nin->x_reqN(Nunused_wire);
        Nin->x_reqE(NreqE_wire);
        Nin->x_reqS(NreqS_wire);
        Nin->x_reqW(NreqW_wire);
        Nin->x_rok(Nrok_wire);
        Nin->x_rd0(Lrd_wire);
        Nin->x_rd1(Erd_wire);
        Nin->x_rd2(Srd_wire);
        Nin->x_rd3(Wrd_wire);
        Nin->x_gnt0(LgntN_wire);
        Nin->x_gnt1(EgntN_wire);
        Nin->x_gnt2(SgntN_wire);
        Nin->x_gnt3(WgntN_wire);
        Nin->x_Lidle(Lidle_wire);
        Nin->x_Nidle(Nidle_wire);
        Nin->x_Eidle(Eidle_wire);
        Nin->x_Sidle(Sidle_wire);
        Nin->x_Widle(Widle_wire);
        Nin->x_dout(Ndata_wire);

        ////////////////////////////////////////////////////////////////////////////
        xin *Ein = new xin("Ein", USE_EAST, XID, YID, EAST_ID);
        ////////////////////////////////////////////////////////////////////////////
        Ein->clk(clk);
        Ein->rst(rst);
        Ein->in_data(Ein_data);
        Ein->in_val(Ein_val);
        Ein->in_ret(Ein_ret);
        Ein->x_reqL(EreqL_wire);
        Ein->x_reqN(EreqN_wire);
        Ein->x_reqE(Eunused_wire);
        Ein->x_reqS(EreqS_wire);
        Ein->x_reqW(EreqW_wire);
        Ein->x_rok(Erok_wire);
        Ein->x_rd0(Lrd_wire);
        Ein->x_rd1(Nrd_wire);
        Ein->x_rd2(Srd_wire);
        Ein->x_rd3(Wrd_wire);
        Ein->x_gnt0(LgntE_wire);
        Ein->x_gnt1(NgntE_wire);
        Ein->x_gnt2(SgntE_wire);
        Ein->x_gnt3(WgntE_wire);
        Ein->x_Lidle(Lidle_wire);
        Ein->x_Nidle(Nidle_wire);
        Ein->x_Eidle(Eidle_wire);
        Ein->x_Sidle(Sidle_wire);
        Ein->x_Widle(Widle_wire);
        Ein->x_dout(Edata_wire);

        ////////////////////////////////////////////////////////////////////////////
        xin *Sin = new xin("Sin", USE_SOUTH, XID, YID, SOUTH_ID);
        ////////////////////////////////////////////////////////////////////////////
        Sin->clk(clk);
        Sin->rst(rst);
        Sin->in_data(Sin_data);
        Sin->in_val(Sin_val);
        Sin->in_ret(Sin_ret);
        Sin->x_reqL(SreqL_wire);
        Sin->x_reqN(SreqN_wire);
        Sin->x_reqE(SreqE_wire);
        Sin->x_reqS(Sunused_wire);
        Sin->x_reqW(SreqW_wire);
        Sin->x_rok(Srok_wire);
        Sin->x_rd0(Lrd_wire);
        Sin->x_rd1(Nrd_wire);
        Sin->x_rd2(Erd_wire);
        Sin->x_rd3(Wrd_wire);
        Sin->x_gnt0(LgntS_wire);
        Sin->x_gnt1(NgntS_wire);
        Sin->x_gnt2(EgntS_wire);
        Sin->x_gnt3(WgntS_wire);
        Sin->x_Lidle(Lidle_wire);
        Sin->x_Nidle(Nidle_wire);
        Sin->x_Eidle(Eidle_wire);
        Sin->x_Sidle(Sidle_wire);
        Sin->x_Widle(Widle_wire);
        Sin->x_dout(Sdata_wire);

        ////////////////////////////////////////////////////////////////////////////
        xin *Win = new xin("Win", USE_WEST, XID, YID, WEST_ID);
        ////////////////////////////////////////////////////////////////////////////
        Win->clk(clk);
        Win->rst(rst);
        Win->in_data(Win_data);
        Win->in_val(Win_val);
        Win->in_ret(Win_ret);
        Win->x_reqL(WreqL_wire);
        Win->x_reqN(WreqN_wire);
        Win->x_reqE(WreqE_wire);
        Win->x_reqS(WreqS_wire);
        Win->x_reqW(Wunused_wire);
        Win->x_rok(Wrok_wire);
        Win->x_rd0(Lrd_wire);
        Win->x_rd1(Nrd_wire);
        Win->x_rd2(Erd_wire);
        Win->x_rd3(Srd_wire);
        Win->x_gnt0(LgntW_wire);
        Win->x_gnt1(NgntW_wire);
        Win->x_gnt2(EgntW_wire);
        Win->x_gnt3(SgntW_wire);
        Win->x_Lidle(Lidle_wire);
        Win->x_Nidle(Nidle_wire);
        Win->x_Eidle(Eidle_wire);
        Win->x_Sidle(Sidle_wire);
        Win->x_Widle(Widle_wire);
        Win->x_dout(Wdata_wire);

        ////////////////////////////////////////////////////////////////////////////
        xout *Lout = new xout("Lout", USE_LOCAL, XID, YID, LOCAL_ID);
        ////////////////////////////////////////////////////////////////////////////
        Lout->clk(clk);
        Lout->rst(rst);
        Lout->out_data(Lout_data);
        Lout->out_val(Lout_val);
        Lout->out_ret(Lout_ret);
        Lout->hold_send(gnd_wire);
        Lout->x_req0(NreqL_wire);
        Lout->x_req1(EreqL_wire);
        Lout->x_req2(SreqL_wire);
        Lout->x_req3(WreqL_wire);
        Lout->x_rok0(Nrok_wire);
        Lout->x_rok1(Erok_wire);
        Lout->x_rok2(Srok_wire);
        Lout->x_rok3(Wrok_wire);
        Lout->x_gnt0(LgntN_wire);
        Lout->x_gnt1(LgntE_wire);
        Lout->x_gnt2(LgntS_wire);
        Lout->x_gnt3(LgntW_wire);
        Lout->x_idle(Lidle_wire);
        Lout->x_rd(Lrd_wire);
        Lout->x_din0(Ndata_wire);
        Lout->x_din1(Edata_wire);
        Lout->x_din2(Sdata_wire);
        Lout->x_din3(Wdata_wire);

        ////////////////////////////////////////////////////////////////////////////
        xout *Nout = new xout("Nout", USE_NORTH, XID, YID, NORTH_ID);
        ////////////////////////////////////////////////////////////////////////////
        Nout->clk(clk);
        Nout->rst(rst);
        Nout->out_data(Nout_data);
        Nout->out_val(Nout_val);
        Nout->out_ret(Nout_ret);
        Nout->hold_send(gnd_wire);
        Nout->x_req0(LreqN_wire);
        Nout->x_req1(EreqN_wire);
        Nout->x_req2(SreqN_wire);
        Nout->x_req3(WreqN_wire);
        Nout->x_rok0(Lrok_wire);
        Nout->x_rok1(Erok_wire);
        Nout->x_rok2(Srok_wire);
        Nout->x_rok3(Wrok_wire);
        Nout->x_gnt0(NgntL_wire);
        Nout->x_gnt1(NgntE_wire);
        Nout->x_gnt2(NgntS_wire);
        Nout->x_gnt3(NgntW_wire);
        Nout->x_idle(Nidle_wire);
        Nout->x_rd(Nrd_wire);
        Nout->x_din0(Ldata_wire);
        Nout->x_din1(Edata_wire);
        Nout->x_din2(Sdata_wire);
        Nout->x_din3(Wdata_wire);

        ////////////////////////////////////////////////////////////////////////////
        xout *Eout = new xout("Eout", USE_EAST, XID, YID, EAST_ID);
        ////////////////////////////////////////////////////////////////////////////
        Eout->clk(clk);
        Eout->rst(rst);
        Eout->out_data(Eout_data);
        Eout->out_val(Eout_val);
        Eout->out_ret(Eout_ret);
        Eout->hold_send(gnd_wire);
        Eout->x_req0(LreqE_wire);
        Eout->x_req1(NreqE_wire);
        Eout->x_req2(SreqE_wire);
        Eout->x_req3(WreqE_wire);
        Eout->x_rok0(Lrok_wire);
        Eout->x_rok1(Nrok_wire);
        Eout->x_rok2(Srok_wire);
        Eout->x_rok3(Wrok_wire);
        Eout->x_gnt0(EgntL_wire);
        Eout->x_gnt1(EgntN_wire);
        Eout->x_gnt2(EgntS_wire);
        Eout->x_gnt3(EgntW_wire);
        Eout->x_idle(Eidle_wire);
        Eout->x_rd(Erd_wire);

        Eout->x_din0(Ldata_wire);
        Eout->x_din1(Ndata_wire);
        Eout->x_din2(Sdata_wire);
        Eout->x_din3(Wdata_wire);

        ////////////////////////////////////////////////////////////////////////////
        xout *Sout = new xout("Sout", USE_SOUTH, XID, YID, SOUTH_ID);
        ////////////////////////////////////////////////////////////////////////////
        Sout->clk(clk);
        Sout->rst(rst);
        Sout->out_data(Sout_data);
        Sout->out_val(Sout_val);
        Sout->out_ret(Sout_ret);
        Sout->hold_send(gnd_wire);
        Sout->x_req0(LreqS_wire);
        Sout->x_req1(NreqS_wire);
        Sout->x_req2(EreqS_wire);
        Sout->x_req3(WreqS_wire);
        Sout->x_rok0(Lrok_wire);
        Sout->x_rok1(Nrok_wire);
        Sout->x_rok2(Erok_wire);
        Sout->x_rok3(Wrok_wire);
        Sout->x_gnt0(SgntL_wire);
        Sout->x_gnt1(SgntN_wire);
        Sout->x_gnt2(SgntE_wire);
        Sout->x_gnt3(SgntW_wire);
        Sout->x_idle(Sidle_wire);
        Sout->x_rd(Srd_wire);
        Sout->x_din0(Ldata_wire);
        Sout->x_din1(Ndata_wire);
        Sout->x_din2(Edata_wire);
        Sout->x_din3(Wdata_wire);

        ////////////////////////////////////////////////////////////////////////////
        xout *Wout = new xout("Wout", USE_WEST, XID, YID, WEST_ID);
        ////////////////////////////////////////////////////////////////////////////
        Wout->clk(clk);
        Wout->rst(rst);
        Wout->out_data(Wout_data);
        Wout->out_val(Wout_val);
        Wout->out_ret(Wout_ret);
        Wout->hold_send(gnd_wire);
        Wout->x_req0(LreqW_wire);
        Wout->x_req1(NreqW_wire);
        Wout->x_req2(EreqW_wire);
        Wout->x_req3(SreqW_wire);
        Wout->x_rok0(Lrok_wire);
        Wout->x_rok1(Nrok_wire);
        Wout->x_rok2(Erok_wire);
        Wout->x_rok3(Srok_wire);
        Wout->x_gnt0(WgntL_wire);
        Wout->x_gnt1(WgntN_wire);
        Wout->x_gnt2(WgntE_wire);
        Wout->x_gnt3(WgntS_wire);
        Wout->x_idle(Widle_wire);
        Wout->x_rd(Wrd_wire);
        Wout->x_din0(Ldata_wire);
        Wout->x_din1(Ndata_wire);
        Wout->x_din2(Edata_wire);
        Wout->x_din3(Sdata_wire);

        SC_METHOD(p_gnd);
        sensitive << rst;
    }
};

#endif // __PARIS_H__

