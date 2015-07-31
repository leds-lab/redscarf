/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : ofc_credit_vct (credit-based output flow controller for VCT switching )
FILE	 : ofc_credit_vct.cpp
--------------------------------------------------------------------------------
*/
#include "ofc_credit_vct.h"

////////////////////////////////////////////////////////////////////////////////
void ofc_credit_vct::p_counter()
////////////////////////////////////////////////////////////////////////////////
// Since it has a credit (that means the receiver's buffer is empty) and
// there is a data to send (input buffer is not empty), it increments the
// counter at each cycle while rok=1, taking into account the number of sent
// flits. If a ret (credit return) is received, saying that the destination
// buffer was "flushed" (i.e. the trailer of the sent packet was forwarded),
// it resets the counter to 0, if rok=0 (there is no data to send), or to 1, 
// if rok=1 (there is data to send). 

{
    if (rst.read()==1) {
        counter.write(0);
    } else {
        if (rok.read()==1) {
            if (ret.read()==1) {
                counter.write(1);
            } else {
                if (counter.read() < (FIFO_IN_DEPTH+1))
                    counter.write(counter.read()+1);
            }
        } else {
            if (ret.read()==1) {
                counter.write(0);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ofc_credit_vct::p_outputs()
////////////////////////////////////////////////////////////////////////////////
// It determines the outputs. If there is a flit to be sent (rok=1) and the 
// sender still has at least one credit, the data is sent (val=rd=1). If there 
// is no credit, but the receiver is returning a new credit (ret=1), then, it 
// can also send the data, because there is room in the receiver.
{
    bool move; // Auxiliary variable that is set under the conditions for which
    // a flit can be sent.
    if ((counter.read()==0) || (counter.read()==(FIFO_IN_DEPTH+1))){
        move = 0;
    } else {
        move = 1;
    }
    val.write(move);
    rd.write(move);

}

////////////////////////////////////////////////////////////////////////////////
void ofc_credit_vct::p_debug()
////////////////////////////////////////////////////////////////////////////////
{
    static long int cycle = 0;
    char port[5] = {'L','N','E','S','W'};

    printf("\n[ofc_credit(%d,%d)-%c] @ cycle %ld",XID, YID,port[PORT_ID],cycle++);
    cout  << "   counter=" << counter.read() \
          << "   rok=" << rok.read() \
          << "   ret=" << ret.read() \
          << "   val=" << val.read() \
          << "   rd="  << rd.read() \
          << "\t";
}
