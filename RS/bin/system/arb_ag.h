/*******************************************************************************

 Company: 				UNIVALI
 Engineer:				Thiago Felski Pereira
 
 Create Date:			19/05/2009
 Design Name:			arbiter_ag
 Module Name:    		arb_ag.h
 Project Name:
 Target Devices:
 Tool versions:
 Description:

 Dependencies:

 Revision:
 Revision 0.01 - File Created
 Additional Comments:

*******************************************************************************/
#ifndef __ARB_AG_H__
#define __ARB_AG_H__

#include <systemc.h>
#include "class_checker.h"
#include "stamp_checker.h"
#include "ppe.h"
#include "parameters.h"

SC_MODULE(arb_ag)
{
    // System signals
    sc_in_clk    i_CLK; // clock
    sc_in<bool>  i_RST; // reset

    // Arbiter Age interface
    sc_in<sc_uint<N> >          i_REQ;

    sc_in<sc_uint<FLIT_WIDTH> > i_DIN_0;
    sc_in<sc_uint<FLIT_WIDTH> > i_DIN_1;
    sc_in<sc_uint<FLIT_WIDTH> > i_DIN_2;
    sc_in<sc_uint<FLIT_WIDTH> > i_DIN_3;

    sc_in<bool>         i_ROK_0;
    sc_in<bool>         i_ROK_1;
    sc_in<bool>         i_ROK_2;
    sc_in<bool>         i_ROK_3;

    sc_out<sc_uint<N> >	o_GNT;
    sc_out<bool>        o_IDLE;

    //Internal Signals
    sc_signal<bool>     w_EN_AGE_0;
    sc_signal<bool>     w_EN_AGE_1;
    sc_signal<bool>     w_EN_AGE_2;
    sc_signal<bool>     w_EN_AGE_3;

    sc_signal<bool>     w_EN_CLS_0;
    sc_signal<bool>     w_EN_CLS_1;
    sc_signal<bool>     w_EN_CLS_2;
    sc_signal<bool>     w_EN_CLS_3;

    sc_signal<bool>     w_HP_0;
    sc_signal<bool>     w_HP_1;
    sc_signal<bool>     w_HP_2;
    sc_signal<bool>     w_HP_3;

    sc_signal<sc_uint<N> >  w_GO;

    sc_signal<sc_uint<AGE_WIDTH> >  w_AGE_0;
    sc_signal<sc_uint<AGE_WIDTH> >  w_AGE_1;
    sc_signal<sc_uint<AGE_WIDTH> >  w_AGE_2;
    sc_signal<sc_uint<AGE_WIDTH> >  w_AGE_3;

    sc_signal<sc_uint<2> >  w_CLS_0;
    sc_signal<sc_uint<2> >  w_CLS_1;
    sc_signal<sc_uint<2> >  w_CLS_2;
    sc_signal<sc_uint<2> >  w_CLS_3;

    // Internal data structures
    unsigned short int XID, YID, PORT_ID, CHANNEL_ID;

    // Module's processes
    void p_extract_age_cls_from_header();
    void p_detect_header_presence();
    void p_priority_generator();
    void p_debug();

    SC_HAS_PROCESS(arb_ag);

    //////////////////////////////////////////////////////////////////////////////
    arb_ag(sc_module_name nm,	unsigned short int XID, unsigned short int YID,
           unsigned short int PORT_ID, unsigned short int CHANNEL_ID) : sc_module(nm),
        XID(XID), YID(YID), PORT_ID(PORT_ID), CHANNEL_ID(CHANNEL_ID)
    //////////////////////////////////////////////////////////////////////////////
    {
        /////////////////////////////////////////
        SC_METHOD(p_extract_age_cls_from_header);
        /////////////////////////////////////////
        sensitive << i_DIN_0
                  << i_DIN_1
                  << i_DIN_2
                  << i_DIN_3
                  << w_EN_CLS_0
                  << w_EN_CLS_1
                  << w_EN_CLS_2
                  << w_EN_CLS_3
                  << w_HP_0
                  << w_HP_1
                  << w_HP_2
                  << w_HP_3;

        /////////////////////////////////////////
        SC_METHOD(p_detect_header_presence);
        /////////////////////////////////////////
        sensitive << i_DIN_0
                  << i_DIN_1
                  << i_DIN_2
                  << i_DIN_3
                  << i_ROK_0
                  << i_ROK_1
                  << i_ROK_2
                  << i_ROK_3;


        /////////////////////////////////////////
        SC_METHOD(p_priority_generator);
        /////////////////////////////////////////
        sensitive	<< w_EN_CLS_0
                    << w_EN_CLS_1
                    << w_EN_CLS_2
                    << w_EN_CLS_3
                    << w_EN_AGE_0
                    << w_EN_AGE_1
                    << w_EN_AGE_2
                    << w_EN_AGE_3;


        //////////////////////////////////////////////////////////////////////////////////////
        class_checker *class_checker0 = new class_checker("class_checker", XID, YID, PORT_ID);
        //////////////////////////////////////////////////////////////////////////////////////
        class_checker0->i_CLS_0(w_CLS_0);
        class_checker0->i_CLS_1(w_CLS_1);
        class_checker0->i_CLS_2(w_CLS_2);
        class_checker0->i_CLS_3(w_CLS_3);
        class_checker0->o_ENA_0(w_EN_CLS_0);
        class_checker0->o_ENA_1(w_EN_CLS_1);
        class_checker0->o_ENA_2(w_EN_CLS_2);
        class_checker0->o_ENA_3(w_EN_CLS_3);

        //////////////////////////////////////////////////////////////////////////////////////
        stamp_checker *stamp_checker0 = new stamp_checker("stamp_checker", XID, YID, PORT_ID);
        //////////////////////////////////////////////////////////////////////////////////////
        stamp_checker0->i_CLS_ENA_0(w_EN_CLS_0);
        stamp_checker0->i_CLS_ENA_1(w_EN_CLS_1);
        stamp_checker0->i_CLS_ENA_2(w_EN_CLS_2);
        stamp_checker0->i_CLS_ENA_3(w_EN_CLS_3);
        stamp_checker0->i_AGE_0(w_AGE_0);
        stamp_checker0->i_AGE_1(w_AGE_1);
        stamp_checker0->i_AGE_2(w_AGE_2);
        stamp_checker0->i_AGE_3(w_AGE_3);
        stamp_checker0->o_ENA_0(w_EN_AGE_0);
        stamp_checker0->o_ENA_1(w_EN_AGE_1);
        stamp_checker0->o_ENA_2(w_EN_AGE_2);
        stamp_checker0->o_ENA_3(w_EN_AGE_3);

        //////////////////////////////////////////////
        ppe *ppe0 = new ppe("ppe", XID, YID, PORT_ID);
        //////////////////////////////////////////////
        ppe0->clk(i_CLK);
        ppe0->rst(i_RST);
        ppe0->R(i_REQ);
        ppe0->P(w_GO);
        ppe0->G(o_GNT);
        ppe0->idle(o_IDLE);

#ifdef DEBUG_ARB_AG
        SC_METHOD(p_debug);
        sensitive << i_CLK.pos();
#endif
    }
};
#endif // __ARB_AG__
