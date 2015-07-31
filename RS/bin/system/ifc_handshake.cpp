/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : ifc_handshake (handshale-based input flow controller)
FILE	 : ifc_handshake.cpp
--------------------------------------------------------------------------------
*/
#include "ifc.h"

//////////////////////////////////////////////////////////////////////////////// 
void ifc_handshake::p_state_reg()
////////////////////////////////////////////////////////////////////////////////
// This process implements the state register of the FSM
{
    if (rst.read()==1)
        state_reg = S0;
    else
        state_reg = next_state;
}


////////////////////////////////////////////////////////////////////////////////
void ifc_handshake::p_next_state()
////////////////////////////////////////////////////////////////////////////////
// This process implements the next state function of the FSM
{
    switch (state_reg) {
    // Waits a new incoming data (val=1) and, if the FIFO is not full
    // (wok=1), goes to the S1 state in order to receive the data.
        case S0:
            if ((val.read()) && (wok.read()))
                next_state = S1;
            else
                next_state = S0;
            break;

            // Writes the data into the FIFO and goes back to the S0 state
            // if val=0, or, if not, goes to S2 state.
        case S1:
            if (!val.read())
                next_state = S0;
            else
                next_state = S2;
            break;

            // Waits val goes to 0 to complete the data tranfer.
        case S2:
            if (!val.read())
                next_state = S0;
            else
                next_state = S2;
            break;

        default:
            next_state = S0;
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ifc_handshake::p_outputs()
////////////////////////////////////////////////////////////////////////////////
// This process updates the outputs
{
    switch (state_reg) {
    // Do nothing
        case S0:
            ret.write(0);
            wr.write(0);
            break;

            // Acknowledges the data and writes it into the FIFO
        case S1:
            ret.write(1);
            wr.write(1);
            break;

            // Holds the acknowledge high while valid is not low
        case S2:
            ret.write(1);
            wr.write(0);
            break;

        default:
            ret.write(0);
            wr.write(0);
            break;
    }
}


////////////////////////////////////////////////////////////////////////////////
void ifc_handshake::p_debug()
////////////////////////////////////////////////////////////////////////////////
{
    static long int cycle = 0;
    char port[5] = {'L','N','E','S','W'};

    printf("\n[ifc_hndshk(%d,%d)-%c] @ cycle %ld",XID, YID,port[PORT_ID],cycle++);
    cout << "  state="<< state_reg.read();
    cout << "  val="  << val.read();
    cout << "  ret="  << ret.read();
    cout << "  wr="   << wr.read();
    cout << "  wok="  << wok.read();
    cout << "\t";
}
