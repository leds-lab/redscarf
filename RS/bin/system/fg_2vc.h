////////////////////////////////////////////////////////////////////////////////
// 
// name         fg_2vc.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __FG_2VC_H__
#define __FG_2VC_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(fg_2vc)
{
    // INTERFACE
    // System signals
    sc_in_clk                       clk;
    sc_in<bool>                     rst;
    sc_in<sc_uint<64> >             clock_cycles;

    // Interface to the VCs of the communication port (its is done by means of FIFOs)
    sc_out<sc_uint<FLIT_WIDTH> >    vc0_snd_data;   // I/F with the output FIFO
    sc_out<bool>                    vc0_snd_wr;
    sc_in<bool>                     vc0_snd_wok;
    sc_in<sc_uint<FLIT_WIDTH> >     vc0_rcv_data;   // I/F with the input FIFO
    sc_in<bool>                     vc0_rcv_rok;
    sc_out<bool>                    vc0_rcv_rd;

    sc_out<sc_uint<FLIT_WIDTH> >    vc1_snd_data;   // I/F with the output FIFO
    sc_out<bool>                    vc1_snd_wr;
    sc_in<bool>                     vc1_snd_wok;
    sc_in<sc_uint<FLIT_WIDTH> >     vc1_rcv_data;   // I/F with the input FIFO
    sc_in<bool>                     vc1_rcv_rok;
    sc_out<bool>                    vc1_rcv_rd;

    // Status signals
    sc_out<bool>                    eot;
    sc_out<sc_uint<32> >            number_of_packets_sent;
    sc_out<sc_uint<32> >            number_of_packets_received;

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    // Data structures with the basic fields of a flow
    struct FLOW_TYPE {
        unsigned int        type;                   //  0: 0 = gtr determines the traffic model, 1 = tg determines the traffic model by using PARETO
        unsigned int        x_dest;                 //  1: X address of the destination node
        unsigned int        y_dest;                 //  2: Y address of the destination node
        unsigned int        flow_id;                //  3: Flow identifier
        unsigned int        traffic_class;          //  4: Class of traffic (RT0, RT1, nRT0, nRT1)
        unsigned long long  pck_2send;              //  5: Number of packets to be sent by the flow
        unsigned long long  deadline;               //  6: Required deadline to deliver the message
        float               required_bw;            //  7: A percentual value of the channel bandwidth (e.g. 0.2 = 20%)
        unsigned int        payload_length;         //  8: Number of flits in the payload (including the trailer)
        unsigned int        idle;                   //  9: Number of idle cycles between two packets
        unsigned int        iat;                    // 10: Inter-arrival time
        unsigned int        burst_size;             // 11: Number of packets in a burst transmission
        unsigned int        last_payload_length;    // 12: Number of flits in the payload (including the trailer) of the last packet in a burst
        float               parameter1;             // 13: Additional parameter for TG-based trafic modelling (e.g.alfa_on in Pareto)
        float               parameter2;             // 14: Additional parameter for TG-based trafic modelling (e.g.alfa_off in Pareto)
        unsigned int        switching_type;         // 15: Switching type
        unsigned long long  pck_sent;               // 16: Status about the number of packets already sent
    };

    // Module process
    void p_send();
    void p_receive();

    // Member functions
    void f_send_flit(sc_uint<FLIT_WIDTH>, unsigned int);
    void f_send_packet(sc_uint<RIB_WIDTH>, unsigned long long, FLOW_TYPE,unsigned long long, unsigned int);
    void f_send_burst_of_packets(sc_uint<FLIT_WIDTH>, unsigned long long, FLOW_TYPE);

    SC_HAS_PROCESS(fg_2vc);

    //////////////////////////////////////////////////////////////////////////////
    fg_2vc(sc_module_name nm,
           unsigned short int XID,
           unsigned short int YID) :
        sc_module(nm),
        XID(XID),
        YID(YID)
    //////////////////////////////////////////////////////////////////////////////
    {
        SC_CTHREAD(p_send, clk.pos());
        sensitive << clk.pos() << rst.pos();

        SC_CTHREAD(p_receive, clk.pos());
        sensitive << clk.pos() << rst.pos();
    }

};
#endif // __FG_2VC_H__
