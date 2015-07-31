/*******************************************************************************

 Company: 				Intelbras / UNIVALI
 Engineer:				Cesar Albenes Zeferino
                                    Marcelo Daniel Berejuck
                                    Thiago Felski Pereira

 Create Date:			13/05/2009
 Design Name:			Stampler
 Module Name:			testbench.h
 Project Name:
 Target Devices:
 Tool versions:
 Description:

 Dependencies:

 Revision:
 Revision 0.01 - File Created
 Additional Comments:

*******************************************************************************/
#ifndef __STAMPLER_H__
#define __STAMPLER_H__

#include <systemc.h>
#include "parameters.h"
#include "stampler_control.h"
#include "stampler_counter.h"


SC_MODULE(stampler)
{
    // System signals
    sc_in_clk     i_CLK;  // clock
    sc_in<bool>   i_RST;  // reset

    // Stampler control interface
    sc_in<bool>                     i_ROK;
    sc_in<sc_uint<FLIT_WIDTH> >     i_DIN;
    sc_out<sc_uint<FLIT_WIDTH> >    o_DOUT;

    // Signals
    sc_signal<bool>                 w_BOP;
    sc_signal<bool>                 w_HEADER_PRESENT;
    sc_signal<bool>                 w_CLR_CNT;
    sc_signal<bool>                 w_ENA_CNT;
    sc_signal<sc_uint<AGE_WIDTH> >  w_NEW_AGE;
    sc_signal<sc_uint<FLIT_WIDTH> >	w_DIN;


    // Module's processes
    void p_stampler();
    void p_framing();
    void p_debug();

    // Internal data structures
    unsigned short int XID, YID, PORT_ID, CHANNEL_ID;

    SC_HAS_PROCESS(stampler);
    /*
  //////////////////////////////////////////////////////////////////////////////
  stampler(sc_module_name nm) :	sc_module(nm)
  //////////////////////////////////////////////////////////////////////////////
*/
    //////////////////////////////////////////////////////////////////////////////
    stampler(sc_module_name nm, unsigned short int XID, unsigned short int YID,
             unsigned short int PORT_ID, unsigned short int CHANNEL_ID) :
        sc_module(nm), XID(XID), YID(YID), PORT_ID(PORT_ID), CHANNEL_ID(CHANNEL_ID)
    //////////////////////////////////////////////////////////////////////////////

    {
        ///////////////////////////////////////////////////////////////////////
        stampler_control *scontrol0 = new stampler_control("stampler_control",
                                                           XID,YID,PORT_ID,CHANNEL_ID);
        ///////////////////////////////////////////////////////////////////////
        scontrol0->i_CLK(i_CLK);
        scontrol0->i_RST(i_RST);
        scontrol0->i_HP(w_HEADER_PRESENT);
        scontrol0->o_CLEAR(w_CLR_CNT);
        scontrol0->o_ENA(w_ENA_CNT);

        ///////////////////////////////////////////////////////////////////////
        stampler_counter *scounter0 = new stampler_counter("stampler_counter",
                                                           XID,YID,PORT_ID,CHANNEL_ID);
        ///////////////////////////////////////////////////////////////////////
        scounter0->i_CLK(i_CLK);
        scounter0->i_RST(i_RST);
        scounter0->i_CLEAR(w_CLR_CNT);
        scounter0->i_ENA(w_ENA_CNT);
        scounter0->o_DOUT(w_NEW_AGE);

        ///////////////////////////////////////////////////////////////////////
        SC_METHOD(p_framing);
        ///////////////////////////////////////////////////////////////////////
        sensitive << i_DIN
                  << i_ROK;


        ///////////////////////////////////////////////////////////////////////
        SC_METHOD(p_stampler);
        ///////////////////////////////////////////////////////////////////////
        sensitive << i_DIN
                  << w_HEADER_PRESENT
                  << w_NEW_AGE
                  << i_ROK;

#ifdef DEBUG_STAMPLER
        SC_METHOD(p_debug);
        sensitive << i_CLK.pos();
#endif
    }
};
#endif // __STAMPLER__
