/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : ofc_credit (credit-based output flow controller )
FILE	 : ofc_credit.cpp
--------------------------------------------------------------------------------
*/
#include "ofc_credit.h"

////////////////////////////////////////////////////////////////////////////////
void ofc_credit::p_counter()
////////////////////////////////////////////////////////////////////////////////
// It implements the credit counter. If there is no data to be sent (rok=0) and 
// a credit is received (ret=1), it increments the number of credits.On the 
// other hand, if it is sending a data and no credit is being received, it 
// decrements the number of credits. Otherwise, the number of credits doesn't 
// change.  
{
    if (rst.read()==1) {
        counter.write(CREDIT);
    }
    else {
        if (rok.read()==0 ) {
            if ((ret.read()) && (counter.read()!=(CREDIT)))
                counter.write(counter.read()+1);
        } else {
            if ((ret.read()==0) && (counter.read()!=0))
                counter.write(counter.read()-1);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ofc_credit::p_outputs()
////////////////////////////////////////////////////////////////////////////////
// It determines the outputs. If there is a flit to be sent (rok=1) and the 
// sender still has at least one credit, the data is sent (val=rd=1). If there 
// is no credit, but the receiver is returning a new credit (ret=1), then, it 
// can also send the data, because there is room in the receiver.
{
    bool move; // Auxiliary variable that is set under the conditions for which
    // a flit can be sent.

    if (rok.read()==1) {
        if (counter.read()==0) {
            if (ret.read()==1)
                move = 1;
            else
                move = 0;
        } else {
            move = 1;
        }
    } else {
        move = 0;
    }

    val.write(move);
    rd.write(move);

}

////////////////////////////////////////////////////////////////////////////////
void ofc_credit::p_debug()
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
