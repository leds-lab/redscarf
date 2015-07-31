////////////////////////////////////////////////////////////////////////////////
// 
// name         tg.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __TG_H__
#define __TG_H__

#include <systemc.h>
#include "parameters.h"
#include "fg.h"
#include "ifc.h"
#include "ofc.h"

//#define USE_UNBOUNDED_FIFO

#ifdef USE_UNBOUNDED_FIFO
    #include "unboundedfifo.h"
#else
    #include "fifo_in.h"
    #include "fifo_out.h"
#endif


SC_MODULE(tg) {

    // INTERFACE
    // System signals
    sc_in_clk       	clk;
    sc_in<bool>       	rst;
    sc_in<sc_uint<64> > clock_cycles;

    // Link signals
    sc_out<sc_uint<FLIT_WIDTH> >    out_data;   // Output data channel
    sc_out<bool>                    out_val;    // Data validation
    sc_in<bool>                     out_ret;    // Return (ack or credit)
    sc_in<sc_uint<FLIT_WIDTH> >     in_data;    // Input data channel
    sc_in<bool>                     in_val;     // Data validation
    sc_out<bool>                    in_ret;     // Return (ack or credit)

    // Status signals
    sc_out<bool>            eot;
    sc_out<sc_uint<32> >    number_of_packets_sent;
    sc_out<sc_uint<32> >    number_of_packets_received;


    // Internal signals
    sc_signal<sc_uint<FLIT_WIDTH> > snd_data_wire;
    sc_signal<bool>                 snd_wok_wire;
    sc_signal<bool>                 snd_wr_wire;
    sc_signal<bool>                 snd_rok_wire;
    sc_signal<bool>                 snd_rd_wire;

    sc_signal<sc_uint<FLIT_WIDTH> > rcv_data_wire;
    sc_signal<bool>                 rcv_wok_wire;
    sc_signal<bool>                 rcv_wr_wire;
    sc_signal<bool>                 rcv_rok_wire;
    sc_signal<bool>                 rcv_rd_wire;

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    void p_debug(void) {
        if ((XID+YID)==0) {
            if (snd_wr_wire.read()) {
                printf("\n");
                printf("snd_wok = %d",(unsigned int) snd_wok_wire.read());
                printf("\tsnd_wr = %d",(unsigned int) snd_wr_wire.read());
                printf("\tsnd_data = 0x%8X",(unsigned int) snd_data_wire.read());
                printf("\tsnd_rok = %d",(unsigned int) snd_rok_wire.read());
                printf("\tsnd_rd = %d",(unsigned int) snd_rd_wire.read());
                printf("\tout_data = 0x%8X",(unsigned int) out_data.read());

            }
        }
    }

    SC_HAS_PROCESS(tg);

    //////////////////////////////////////////////////////////////////////////////
    tg(sc_module_name nm,
        unsigned short int XID,
        unsigned short int YID) :
        sc_module(nm),
        XID(XID),
        YID(YID)
    //////////////////////////////////////////////////////////////////////////////
    {
//        SC_METHOD(p_debug);
//        sensitive << clk.pos();

        //////////////////////////////////
        fg *fg0 = new fg("fg0", XID, YID);
        //////////////////////////////////
        fg0->clk(clk);
        fg0->rst(rst);
        fg0->clock_cycles(clock_cycles);
        fg0->snd_data(snd_data_wire);
        fg0->snd_wr(snd_wr_wire);
        fg0->snd_wok(snd_wok_wire);
        fg0->rcv_data(rcv_data_wire);
        fg0->rcv_rok(rcv_rok_wire);
        fg0->rcv_rd(rcv_rd_wire);
        fg0->eot(eot);
        fg0->number_of_packets_sent(number_of_packets_sent);
        fg0->number_of_packets_received(number_of_packets_received);

#ifndef USE_UNBOUNDED_FIFO
        ///////////////////////////////////////////////////////////
        fifo_out *fifo0 = new fifo_out("fifo0", XID, YID, PORT_ID);
        ///////////////////////////////////////////////////////////
        fifo0->clk(clk);
        fifo0->rst(rst);
        fifo0->rok(snd_rok_wire);
        fifo0->wok(snd_wok_wire);
        fifo0->rd(snd_rd_wire);
        fifo0->wr(snd_wr_wire);
        fifo0->din(snd_data_wire);
        fifo0->dout(out_data);
#else
        UnboundedFifo* fifoOut = new UnboundedFifo("FifoOutTG");
        fifoOut->i_CLK(clk);
        fifoOut->i_DATA_IN(snd_data_wire);
        fifoOut->i_WR(snd_wr_wire);
        fifoOut->i_RD(snd_rd_wire);
        fifoOut->o_WR_OK(snd_wok_wire);
        fifoOut->o_RD_OK(snd_rok_wire);
        fifoOut->o_DATA_OUT(out_data);
#endif

        //////////////////////////////////////////////
        ofc *ofc0 = new ofc("ofc0", XID, YID, PORT_ID);
        //////////////////////////////////////////////
        ofc0->clk(clk);
        ofc0->rst(rst);
        ofc0->val(out_val);
        ofc0->ret(out_ret);
        ofc0->rok(snd_rok_wire);
        ofc0->rd(snd_rd_wire);

#ifndef USE_UNBOUNDED_FIFO
        /////////////////////////////////////////////////////////
        fifo_in *fifo1 = new fifo_in("fifo1", XID, YID, PORT_ID);
        /////////////////////////////////////////////////////////
        fifo1->clk(clk);
        fifo1->rst(rst);
        fifo1->rok(rcv_rok_wire);
        fifo1->wok(rcv_wok_wire);
        fifo1->rd(rcv_rd_wire);
        fifo1->wr(rcv_wr_wire);
        fifo1->din(in_data);
        fifo1->dout(rcv_data_wire);
#else
        UnboundedFifo* fifoIn = new UnboundedFifo("FifoInTG");
        fifoIn->i_CLK(clk);
        fifoIn->i_DATA_IN(in_data);
        fifoIn->i_WR(rcv_wr_wire);
        fifoIn->i_RD(rcv_rd_wire);
        fifoIn->o_WR_OK(rcv_wok_wire);
        fifoIn->o_RD_OK(rcv_rok_wire);
        fifoIn->o_DATA_OUT(rcv_data_wire);
#endif

        //////////////////////////////////////////////
        ifc *ifc0 = new ifc("ifc0", XID, YID, PORT_ID);
        //////////////////////////////////////////////
        ifc0->clk(clk);
        ifc0->rst(rst);
        ifc0->val(in_val);
        ifc0->ret(in_ret);
        ifc0->rok(rcv_rok_wire);
        ifc0->rd(rcv_rd_wire);
        ifc0->wok(rcv_wok_wire);
        ifc0->wr(rcv_wr_wire);
        ifc0->data(rcv_data_wire);
    }

};
#endif // __TG_H__
