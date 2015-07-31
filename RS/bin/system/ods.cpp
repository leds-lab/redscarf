/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : ods (output data switch)
FILE   : ods.h
--------------------------------------------------------------------------------
*/

#include "ods.h"

////////////////////////////////////////////////////////////////////////////////
void ods::p_output()
////////////////////////////////////////////////////////////////////////////////
{
    sc_uint<4> grant;

    if (SWITCH_TYPE == 0) { // ARRAY AND-OR
        // It selects the data bus from the granted input channel. If there is no
        // grant, dout must be 0.

        grant[0] = gnt0.read();
        grant[1] = gnt1.read();
        grant[2] = gnt2.read();
        grant[3] = gnt3.read();

        switch (grant)	{
            case 0x8: dout.write(din3);
                break;
            case 0x4: dout.write(din2);
                break;
            case 0x2: dout.write(din1);
                break;
            case 0x1: dout.write(din0);
                break;
            default: 	dout.write(0);
                break;
        }
    }
    else {} // FOR OTHER TYPE OF IMPLEMENTATIONS (INTENDED FOR SYNTHESIS)
}
