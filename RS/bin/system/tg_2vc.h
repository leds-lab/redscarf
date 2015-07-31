////////////////////////////////////////////////////////////////////////////////
// 
// name         tg_2vc.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __TG_2VC_H__
#define __TG_2VC_H__

#include <systemc.h>
#include "parameters.h"
#include "fg_2vc.h"
#include "fifo_in.h"
#include "fifo_out.h"
#include "ifc.h"
#include "ofc.h"

SC_MODULE(tg_2vc)
{
    // INTERFACE
    // System signals
    sc_in_clk           clk;
    sc_in<bool>         rst;
    sc_in<sc_uint<64> > clock_cycles;

    // Link signals
    sc_out<sc_uint<FLIT_WIDTH> >    out_data;       // Output data channel
    sc_out<bool>                    vc0_out_val;    // VC0 Data validation
    sc_in<bool>                     vc0_out_ret;    // VC0 Return (ack or credit)
    sc_out<bool>                    vc1_out_val;    // VC1 Data validation
    sc_in<bool>                     vc1_out_ret;    // VC1 Return (ack or credit)

    sc_in<sc_uint<FLIT_WIDTH> > 	in_data;    // Input data channel
    sc_in<bool>                     vc0_in_val; // VC0 Data validation
    sc_out<bool>                    vc0_in_ret;	// VC0 Return (ack or credit)
    sc_in<bool>                     vc1_in_val; // VC1 Data validation
    sc_out<bool>                    vc1_in_ret; // VC1 Return (ack or credit)


    // Status signals
    sc_out<bool>                    eot;
    sc_out<sc_uint<32> >            number_of_packets_sent;
    sc_out<sc_uint<32> >            number_of_packets_received;


    // Internal signals
    sc_signal<sc_uint<FLIT_WIDTH> > vc0_snd_data_wire;
    sc_signal<bool>                 vc0_snd_wok_wire;
    sc_signal<bool>                 vc0_snd_wr_wire;
    sc_signal<bool>                 vc0_snd_rok_wire;
    sc_signal<bool>                 vc0_snd_rd_wire;
    sc_signal<sc_uint<FLIT_WIDTH> > vc0_out_data_wire;

    sc_signal<sc_uint<FLIT_WIDTH> > vc0_rcv_data_wire;
    sc_signal<bool>                 vc0_rcv_wok_wire;
    sc_signal<bool>                 vc0_rcv_wr_wire;
    sc_signal<bool>                 vc0_rcv_rok_wire;
    sc_signal<bool>                 vc0_rcv_rd_wire;

    sc_signal<sc_uint<FLIT_WIDTH> > vc1_snd_data_wire;
    sc_signal<bool>                 vc1_snd_wok_wire;
    sc_signal<bool>                 vc1_snd_wr_wire;
    sc_signal<bool>                 vc1_snd_rok_wire;
    sc_signal<bool>                 vc1_snd_rok_wire2;
    sc_signal<bool>                 vc1_snd_rd_wire;
    sc_signal<sc_uint<FLIT_WIDTH> > vc1_out_data_wire;

    sc_signal<sc_uint<FLIT_WIDTH> > vc1_rcv_data_wire;
    sc_signal<bool>                 vc1_rcv_wok_wire;
    sc_signal<bool>                 vc1_rcv_wr_wire;
    sc_signal<bool>                 vc1_rcv_rok_wire;
    sc_signal<bool>                 vc1_rcv_rd_wire;

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    void p_debug(void) {
        if ((XID+YID)==0) {
            if (vc0_snd_wr_wire.read()) {
                printf("\n");
                printf("vc0_snd_wok = %d"      ,(unsigned int) vc0_snd_wok_wire.read());
                printf("\tvc0_snd_wr = %d"     ,(unsigned int) vc0_snd_wr_wire.read());
                printf("\tvc0_snd_data = 0x%8X",(unsigned int) vc0_snd_data_wire.read());
                printf("\tvc0_snd_rok = %d"    ,(unsigned int) vc0_snd_rok_wire.read());
                printf("\tvc0_snd_rd = %d"     ,(unsigned int) vc0_snd_rd_wire.read());

                printf("\tvc1_snd_wok = %d"    ,(unsigned int) vc1_snd_wok_wire.read());
                printf("\tvc1_snd_wr = %d"     ,(unsigned int) vc1_snd_wr_wire.read());
                printf("\tvc1_snd_data = 0x%8X",(unsigned int) vc1_snd_data_wire.read());
                printf("\tvc1_snd_rok = %d"    ,(unsigned int) vc1_snd_rok_wire.read());
                printf("\tvc1_snd_rd = %d"     ,(unsigned int) vc1_snd_rd_wire.read());

                printf("\tout_data = 0x%8X"    ,(unsigned int) out_data.read());
            }
        }
    }

    // Module's processes
    void p_sel_channel() {
        if (vc0_out_val.read()) {
            out_data.write(vc0_out_data_wire.read());
            vc1_snd_rok_wire.write(0);
        } else {
            out_data.write(vc1_out_data_wire.read());
            vc1_snd_rok_wire.write(vc1_snd_rok_wire2.read());
        }
    };


    SC_HAS_PROCESS(tg_2vc);

    //////////////////////////////////////////////////////////////////////////////
    tg_2vc(sc_module_name nm,
           unsigned short int XID,
           unsigned short int YID) :
        sc_module(nm),
        XID(XID),
        YID(YID)
      //////////////////////////////////////////////////////////////////////////////
    {
        //   SC_METHOD(p_debug);
        //    sensitive << clk.pos();


        //////////////////////////////////////////
        fg_2vc *fg0 = new fg_2vc("fg0", XID, YID);
        //////////////////////////////////////////
        fg0->clk(clk);
        fg0->rst(rst);
        fg0->clock_cycles(clock_cycles);

        fg0->vc0_snd_data(vc0_snd_data_wire);
        fg0->vc0_snd_wr  (vc0_snd_wr_wire);
        fg0->vc0_snd_wok (vc0_snd_wok_wire);
        fg0->vc0_rcv_data(vc0_rcv_data_wire);
        fg0->vc0_rcv_rok (vc0_rcv_rok_wire);
        fg0->vc0_rcv_rd  (vc0_rcv_rd_wire);

        fg0->vc1_snd_data(vc1_snd_data_wire);
        fg0->vc1_snd_wr  (vc1_snd_wr_wire);
        fg0->vc1_snd_wok (vc1_snd_wok_wire);
        fg0->vc1_rcv_data(vc1_rcv_data_wire);
        fg0->vc1_rcv_rok (vc1_rcv_rok_wire);
        fg0->vc1_rcv_rd  (vc1_rcv_rd_wire);

        fg0->eot(eot);
        fg0->number_of_packets_sent(number_of_packets_sent);
        fg0->number_of_packets_received(number_of_packets_received);


        ////////////////////////////////////////////////////////////////////////////
        //
        //             V I R T U A L   C H A N N E L   0
        //
        ////////////////////////////////////////////////////////////////////////////

        /////////////////////////////////////////////////////////////////////////
        fifo_out *vc0_fifo_out = new fifo_out("vc0_fifo_out", XID, YID, PORT_ID);
        /////////////////////////////////////////////////////////////////////////
        vc0_fifo_out->clk (clk);
        vc0_fifo_out->rst (rst);
        vc0_fifo_out->rok (vc0_snd_rok_wire);
        vc0_fifo_out->wok (vc0_snd_wok_wire);
        vc0_fifo_out->rd  (vc0_snd_rd_wire);
        vc0_fifo_out->wr  (vc0_snd_wr_wire);
        vc0_fifo_out->din (vc0_snd_data_wire);
        vc0_fifo_out->dout(vc0_out_data_wire);

        /////////////////////////////////////////////////////
        ofc *vc0_ofc = new ofc("vc0_ofc", XID, YID, PORT_ID);
        /////////////////////////////////////////////////////
        vc0_ofc->clk(clk);
        vc0_ofc->rst(rst);
        vc0_ofc->val(vc0_out_val);
        vc0_ofc->ret(vc0_out_ret);
        vc0_ofc->rok(vc0_snd_rok_wire);
        vc0_ofc->rd (vc0_snd_rd_wire);

        /////////////////////////////////////////////////////////////////////
        fifo_in *vc0_fifo_in = new fifo_in("vc0_fifo_in", XID, YID, PORT_ID);
        /////////////////////////////////////////////////////////////////////
        vc0_fifo_in->clk (clk);
        vc0_fifo_in->rst (rst);
        vc0_fifo_in->rok (vc0_rcv_rok_wire);
        vc0_fifo_in->wok (vc0_rcv_wok_wire);
        vc0_fifo_in->rd  (vc0_rcv_rd_wire);
        vc0_fifo_in->wr  (vc0_rcv_wr_wire);
        vc0_fifo_in->din (in_data);
        vc0_fifo_in->dout(vc0_rcv_data_wire);

        /////////////////////////////////////////////////
        ifc *vc0_ifc = new ifc("vc0_ifc", XID, YID, PORT_ID);
        /////////////////////////////////////////////////
        vc0_ifc->clk (clk);
        vc0_ifc->rst (rst);
        vc0_ifc->val (vc0_in_val);
        vc0_ifc->ret (vc0_in_ret);
        vc0_ifc->rok (vc0_rcv_rok_wire);
        vc0_ifc->rd  (vc0_rcv_rd_wire);
        vc0_ifc->wok (vc0_rcv_wok_wire);
        vc0_ifc->wr  (vc0_rcv_wr_wire);
        vc0_ifc->data(vc0_rcv_data_wire);


        ////////////////////////////////////////////////////////////////////////////
        //
        //             V I R T U A L   C H A N N E L   1
        //
        ////////////////////////////////////////////////////////////////////////////

        /////////////////////////////////////////////////////////////////////////
        fifo_out *vc1_fifo_out = new fifo_out("vc1_fifo_out", XID, YID, PORT_ID);
        /////////////////////////////////////////////////////////////////////////
        vc1_fifo_out->clk (clk);
        vc1_fifo_out->rst (rst);
        vc1_fifo_out->rok (vc1_snd_rok_wire2);
        vc1_fifo_out->wok (vc1_snd_wok_wire);
        vc1_fifo_out->rd  (vc1_snd_rd_wire);
        vc1_fifo_out->wr  (vc1_snd_wr_wire);
        vc1_fifo_out->din (vc1_snd_data_wire);
        vc1_fifo_out->dout(vc1_out_data_wire);

        /////////////////////////////////////////////////////
        ofc *vc1_ofc = new ofc("vc1_ofc", XID, YID, PORT_ID);
        /////////////////////////////////////////////////////
        vc1_ofc->clk(clk);
        vc1_ofc->rst(rst);
        vc1_ofc->val(vc1_out_val);
        vc1_ofc->ret(vc1_out_ret);
        vc1_ofc->rok(vc1_snd_rok_wire);
        vc1_ofc->rd (vc1_snd_rd_wire);

        /////////////////////////////////////////////////////////////////////
        fifo_in *vc1_fifo_in = new fifo_in("vc1_fifo_in", XID, YID, PORT_ID);
        /////////////////////////////////////////////////////////////////////
        vc1_fifo_in->clk (clk);
        vc1_fifo_in->rst (rst);
        vc1_fifo_in->rok (vc1_rcv_rok_wire);
        vc1_fifo_in->wok (vc1_rcv_wok_wire);
        vc1_fifo_in->rd  (vc1_rcv_rd_wire);
        vc1_fifo_in->wr  (vc1_rcv_wr_wire);
        vc1_fifo_in->din (in_data);
        vc1_fifo_in->dout(vc1_rcv_data_wire);

        /////////////////////////////////////////////////
        ifc *vc1_ifc = new ifc("vc1_ifc", XID, YID, PORT_ID);
        /////////////////////////////////////////////////
        vc1_ifc->clk (clk);
        vc1_ifc->rst (rst);
        vc1_ifc->val (vc1_in_val);
        vc1_ifc->ret (vc1_in_ret);
        vc1_ifc->rok (vc1_rcv_rok_wire);
        vc1_ifc->rd  (vc1_rcv_rd_wire);
        vc1_ifc->wok (vc1_rcv_wok_wire);
        vc1_ifc->wr  (vc1_rcv_wr_wire);
        vc1_ifc->data(vc1_rcv_data_wire);

        SC_METHOD(p_sel_channel);
        sensitive << vc0_out_val
                  << vc0_out_data_wire
                  << vc1_out_data_wire
                  << vc1_snd_rok_wire2;
    }
};
#endif // __TG_2VC_H__
