/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : irs (input read switch)
FILE   : irs.cpp
--------------------------------------------------------------------------------
*/
#include "irs.h"

////////////////////////////////////////////////////////////////////////////////
void irs::p_output()
////////////////////////////////////////////////////////////////////////////////
{
    sc_uint<4> grant;

    if (SWITCH_TYPE==0) {
        // It selects the read command from the granting output channel. If there is
        // no grant, rd_out must be 0.

        grant[0] = gnt0.read();
        grant[1] = gnt1.read();
        grant[2] = gnt2.read();
        grant[3] = gnt3.read();

        switch (grant)	{
            case 0x8: rd_out.write(rd_in3);
                break;
            case 0x4: rd_out.write(rd_in2);
                break;
            case 0x2: rd_out.write(rd_in1);
                break;
            case 0x1: rd_out.write(rd_in0);
                break;
            default:  rd_out.write(0);
                break;
        }
    }
    else {} // FOR OTHER TYPE OF IMPLEMENTATIONS (INTENDED FOR SYNTHESIS)
}
