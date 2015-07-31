/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : ifc_credit_vct (credit-based input flow controller vor VCT switching )
FILE	 : ifc_credit_vct.cpp
--------------------------------------------------------------------------------
*/
#include "ifc_credit_vct.h"

////////////////////////////////////////////////////////////////////////////////
void ifc_credit_vct::p_return()
////////////////////////////////////////////////////////////////////////////////
{
    sc_uint<FLIT_WIDTH>	 data_tmp;        // Used to extract fields from din
    bool                 eop;             // packet framing bit: end of packet

    // It extracts the the eop framing bit
    data_tmp = data.read();
    eop = data_tmp[FLIT_WIDTH-1];

    if (rst.read()==1)
        ret.write(0);
    else
        ret.write(rd.read() and rok.read() and eop);
}

////////////////////////////////////////////////////////////////////////////////
void ifc_credit_vct::p_valid()
////////////////////////////////////////////////////////////////////////////////
{
    wr.write(val.read());
}

////////////////////////////////////////////////////////////////////////////////
void ifc_credit_vct::p_debug()
////////////////////////////////////////////////////////////////////////////////
{
    static long int cycle = 0;
    char port[5] = {'L','N','E','S','W'};

    printf("\n[ifc_credit_vct(%d,%d)-%c] @ cycle %ld",XID, YID,port[PORT_ID],cycle++);
    cout  << "   val=" << val.read() \
          << "   ret=" << ret.read() \
          << "   wr =" << wr.read()  \
          << "   rd =" << rd.read()  \
          << "\t";
}
