/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : ows (output write switch)
FILE   : ows.cpp
--------------------------------------------------------------------------------
*/
#include "ows.h"

////////////////////////////////////////////////////////////////////////////////
void ows::p_output()
////////////////////////////////////////////////////////////////////////////////
{
    sc_uint<4> grant;

    if (SWITCH_TYPE == 0) {
        // It selects the write command from the granted input channel. If there is
        // no grant, wr_out must be 0.

        grant[0] = gnt0.read();
        grant[1] = gnt1.read();
        grant[2] = gnt2.read();
        grant[3] = gnt3.read();

        switch (grant)	{
            case 0x8: wr_out.write(wr_in3);
                break;
            case 0x4: wr_out.write(wr_in2);
                break;
            case 0x2: wr_out.write(wr_in1);
                break;
            case 0x1: wr_out.write(wr_in0);
                break;
            default: 	wr_out.write(0);
                break;
        }
    }
    else {} // FOR OTHER TYPE OF IMPLEMENTATIONS (INTENDED FOR SYNTHESIS)
}
