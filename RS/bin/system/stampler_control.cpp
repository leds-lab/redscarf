/*******************************************************************************

 Company: 				Intelbras / UNIVALI
 Engineer:				Cesar Albenes Zeferino
                                    Marcelo Daniel Berejuck
                                    Thiago Felski Pereira

 Create Date:			12/05/2009
 Design Name:			Stampler Control
 Module Name:			stampler_control.cpp
 Project Name:
 Target Devices:
 Tool versions:
 Description:

 Dependencies:

 Revision:
 Revision 0.01 - File Created
 Additional Comments:

*******************************************************************************/
#include "stampler_control.h"

////////////////////////////////////////////////////////////////////////////////
void stampler_control::p_state_reg()
////////////////////////////////////////////////////////////////////////////////
{
    if (i_RST.read())
        w_STATE_REG.write(STAMPLER_S0);
    else
        w_STATE_REG.write(w_NEXT_STATE.read());
}

////////////////////////////////////////////////////////////////////////////////
void stampler_control::p_next_state()
////////////////////////////////////////////////////////////////////////////////
{
    switch (w_STATE_REG.read()) {
        case STAMPLER_S0:
            if (i_HP.read())
                w_NEXT_STATE.write(STAMPLER_S1);
            else
                w_NEXT_STATE.write(STAMPLER_S0);
            break;

        case STAMPLER_S1:
            if (i_HP.read())
                w_NEXT_STATE.write(STAMPLER_S1);
            else
                w_NEXT_STATE.write(STAMPLER_S0);
            break;

        default:	w_NEXT_STATE.write(STAMPLER_S0);
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
void stampler_control::p_outputs()
////////////////////////////////////////////////////////////////////////////////
{
    switch (w_STATE_REG.read()) {
        case STAMPLER_S0:
            o_CLEAR.write(1);
            o_ENA.write(0);
            break;

        case STAMPLER_S1:
            o_CLEAR.write(0);
            o_ENA.write(1);
            break;

        default:	o_CLEAR.write(0);
            o_ENA.write(0);
            break;
    }

}


////////////////////////////////////////////////////////////////////////////////
void stampler_control::p_debug()
////////////////////////////////////////////////////////////////////////////////
{
    static long int cycle = 0;
    char port[5] = {'L','N','E','S','W'};

    printf("\n[stampler_control(%d,%d)-%c:%d] @ cycle %ld",XID, YID,port[PORT_ID],CHANNEL_ID, cycle++);
    printf("  w_STATE_REG = %d",w_STATE_REG.read());
}

