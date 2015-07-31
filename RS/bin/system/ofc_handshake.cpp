/*
PROJECT: ParIS
MODULE : ofc_handshake (handshale-based output flow controller )
FILE	 : ofc_handshake.h
*/
#include "ofc_handshake.h"

//////////////////////////////////////////////////////////////////////////////// 
void ofc_handshake::p_state_reg()
////////////////////////////////////////////////////////////////////////////////
// This process implements the state register of the FSM
{
    if (rst.read())
        state_reg.write(S0);
    else
        state_reg.write(next_state.read());
}


////////////////////////////////////////////////////////////////////////////////
void ofc_handshake::p_next_state()
////////////////////////////////////////////////////////////////////////////////
// This process implements the next state function of the FSM
{
    switch (state_reg) {
        // If there is a data to be sent in the selected input channel (rok=1)
        // and the receiver is not busy (ret=0), goes to the S1 state in order
        // to send the data.
        case S0:	if ((!ret.read()) && (rok.read()))
                next_state.write(S1);
            else
                next_state.write(S0);
            break;

            // It sends the data and, when the data is received (ret=1), goes to the
            // state S2 in order to notify the sender that the data was delivered.
        case S1:	if (ret.read())
                next_state.write(S2);
            else
                next_state.write(S1);
            break;

            // It notifies the sender that the data was delivered and returns to state
            // S0 or S1 (under the same conditions used in S0).
        case S2:	if ((!ret.read()) && (rok.read()))
                next_state.write(S1);
            else
                next_state.write(S0);
            break;

        default:	next_state.write(S0);
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ofc_handshake::p_outputs()
////////////////////////////////////////////////////////////////////////////////
// This process updates the outputs
{
    switch (state_reg.read()) {
        // Do nothing
        case S0:	val.write(0);
            rd.write(0);
            break;

            //Validates the outgoing data.
        case S1:	val.write(1);
            rd.write(0);
            break;

            // Notifies the sender that the data was sent.
        case S2:	val.write(0);
            rd.write(1);
            break;

        default:	val.write(0);
            rd.write(0);
            break;
    }
}


////////////////////////////////////////////////////////////////////////////////
void ofc_handshake::p_debug()
////////////////////////////////////////////////////////////////////////////////
{
    static long int cycle = 0;
    char port[5] = {'L','N','E','S','W'};

    printf("\n[ofc_hndshk(%d,%d)-%c] @ cycle %ld",XID, YID,port[PORT_ID],cycle++);
    cout << "  state=" << state_reg.read();
    cout << "  rok=" << rok.read();
    cout << "  ret=" << ret.read();
    cout << "  val=" << val.read();
    cout << "  rd="  << rd.read();
    cout << "\t";
}
