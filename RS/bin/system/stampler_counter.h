/*******************************************************************************

 Company: 				UNIVALI
 Engineer:				Thiago Felski Pereira
 
 Create Date:			12/05/2009
 Design Name:			Stampler Counter
 Module Name:    	stampler_counter.h
 Project Name:
 Target Devices:
 Tool versions:
 Description:

 Dependencies:

 Revision:
 Revision 0.01 - File Created
 Additional Comments:

*******************************************************************************/
#ifndef __STAMPLER_COUNTER_H__
#define __STAMPLER_COUNTER_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(stampler_counter)
{
    // System signals
    sc_in_clk   i_CLK;  // clock
    sc_in<bool> i_RST;  // reset

    // Stampler counter interface
    sc_in<bool>                 i_CLEAR;
    sc_in<bool>                 i_ENA;
    sc_out<sc_uint<AGE_WIDTH> >	o_DOUT;

    // Signals
    sc_signal<sc_uint<AGE_CLOCK_WIDTH> > w_CLOCK_COUNTER; //VHDL Original name: w_COUNTER_CLOCK
    sc_signal<sc_uint<AGE_WIDTH> >       w_AGE_COUNTER;   // VHDL original name: v_COUNT

    // Module's processes
    void p_clock_counter();
    void p_overflow_counter();
    void p_debug();

    // Internal data structures
    unsigned short int XID, YID, PORT_ID, CHANNEL_ID;

    SC_HAS_PROCESS(stampler_counter);

    //////////////////////////////////////////////////////////////////////////////
    stampler_counter(sc_module_name nm, unsigned short int XID, unsigned short int YID,
                     unsigned short int PORT_ID, unsigned short int CHANNEL_ID) :
        sc_module(nm), XID(XID), YID(YID), PORT_ID(PORT_ID), CHANNEL_ID(CHANNEL_ID)
      //////////////////////////////////////////////////////////////////////////////
    {
        SC_METHOD(p_clock_counter);
        sensitive << i_CLK.pos()
                  << i_RST;

        SC_METHOD(p_overflow_counter);
        sensitive << i_CLK.pos();


#ifdef DEBUG_STAMPLER_COUNTER
        SC_METHOD(p_debug);
        sensitive << i_CLK.pos();
#endif
    }
};
#endif // __STAMPLER_COUNTER__
