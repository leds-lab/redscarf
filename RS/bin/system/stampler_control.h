/*******************************************************************************

 Company: 				Intelbras / UNIVALI
 Engineer:				Cesar Albenes Zeferino
                                    Marcelo Daniel Berejuck
                                    Thiago Felski Pereira

 Create Date:			12/05/2009
 Design Name:			Stampler control
 Module Name:			stampler_control.h
 Project Name:
 Target Devices:
 Tool versions:
 Description:

 Dependencies:

 Revision:
 Revision 0.01 - File Created
 Additional Comments:

*******************************************************************************/
#ifndef __STAMPLER_CONTROL_H__
#define __STAMPLER_CONTROL_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(stampler_control)
{
    // System signals
    sc_in_clk   i_CLK;  // clock
    sc_in<bool> i_RST;  // reset

    // Stampler control interface
    sc_in<bool>     i_HP;
    sc_out<bool>    o_CLEAR;
    sc_out<bool>    o_ENA;

    // Signals
    sc_signal<stampler_fsm_state> w_STATE_REG;  // Current state
    sc_signal<stampler_fsm_state> w_NEXT_STATE;	// Next state

    // Module's processes
    void p_state_reg();
    void p_next_state();
    void p_outputs();
    void p_debug();

    // Internal data structures
    unsigned short int XID, YID, PORT_ID, CHANNEL_ID;

    SC_HAS_PROCESS(stampler_control);

    //////////////////////////////////////////////////////////////////////////////
    stampler_control(sc_module_name nm, unsigned short int XID, unsigned short int YID,
                     unsigned short int PORT_ID, unsigned short int CHANNEL_ID) :
        sc_module(nm), XID(XID), YID(YID), PORT_ID(PORT_ID), CHANNEL_ID(CHANNEL_ID)
    //////////////////////////////////////////////////////////////////////////////
    {
        SC_METHOD(p_state_reg);
        sensitive << i_CLK.pos() << i_RST;

        SC_METHOD(p_next_state);
        sensitive << w_STATE_REG << i_HP;

        SC_METHOD(p_outputs);
        sensitive << w_STATE_REG;


#ifdef DEBUG_STAMPLER_CONTROL
        SC_METHOD(p_debug);
        sensitive << i_CLK.pos();
#endif
    }
};
#endif // __STAMPLER_CONTROL__
