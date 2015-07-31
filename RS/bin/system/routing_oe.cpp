/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : routing_oe
FILE   : routing_oe.cpp
--------------------------------------------------------------------------------
*/
#include "routing_oe.h"

////////////////////////////////////////////////////////////////////////////////
void routing_oe::p_req()
////////////////////////////////////////////////////////////////////////////////
{
    sc_uint<FLIT_WIDTH>  data_tmp;        // Used to extract fields from data
    sc_uint<RIB_WIDTH/2> Xdest;           // x-coordinate
    sc_uint<RIB_WIDTH/2> Ydest;           // y-coordinate
    bool                 bop;	          // packet framing bit: begin of packet
//    bool                 eop;             // packet framing bit: end of packet
    bool                 header_present;  // A header is in the FIFO's output
    sc_uint<5>           request;         // encoded request
    int                  X, Y;            // Aux. variables used for routing
    // New includes.
//    int                  var_odd_even;    // Aux. variables used for odd and even check.
    sc_uint<RIB_WIDTH/2> Xsour;           // x-coordinate
    sc_uint<RIB_WIDTH/2> Ysour;           // y-coordinate

    // It extracts the RIB fields and the framing bits
    data_tmp = data.read();
    Xdest = data_tmp.range(RIB_WIDTH-1, RIB_WIDTH/2);
    Ydest = data_tmp.range(RIB_WIDTH/2-1,0);
    bop   = data_tmp[FLIT_WIDTH-2];
//    eop   = data_tmp[FLIT_WIDTH-1];

    Xsour = data_tmp.range(2*RIB_WIDTH-1, 2*RIB_WIDTH-RIB_WIDTH/2);
    Ysour = data_tmp.range(2*RIB_WIDTH-RIB_WIDTH/2-1, RIB_WIDTH);

    // It determines if a header is present
    if (bop && rok)
        header_present = true;
    else
        header_present = false;

//    var_odd_even = (int)XID & 0x1;		   // Check odd-even.
    
    // It runs the routing algorithm
    if (header_present)
    {
        X = (int)Xdest - (int)XID;
        Y = (int)Ydest - (int)YID;

        if ((X == 0) and (Y == 0)) {
            request = REQ_L;			// Destination is at the Local port.
        } else {
            if (X == 0)         		// Currently in the same column as destination.
                if (Y > 0)
                    request = REQ_N;    // Destination is at North.
                else
                    request = REQ_S;    // Destination is at South.

            //////////////////////////////////////////// Eastbound messages.
            else
                if (X > 0) {                                        // Eastbound messages.
                    if (Y == 0)
                        request = REQ_E;                            // Destination is at East.

                    else {
                        if ((XID & 1) || (XID == Xsour)) {
                            if (Y > 0) {
                                if (Nidle.read()) {
                                    request = REQ_N;                // Destination is at Northeast.
                                } else {
                                    if ((Xdest & 1) || (X != 1)) {  //	Odd dest. column or >= 2 columns to dest.
                                        if (Eidle.read()) {
                                            request = REQ_E;		// Destination is at East.
                                        } else {
                                            request = REQ_NONE;
                                        }
                                    }
                                }
                            } else {
                                if (Sidle.read()) {
                                    request = REQ_S;                // Destination is at Southeast.
                                } else {
                                    if ((Xdest & 1) || (X != 1)) {  //	Odd dest. column or >= 2 columns to dest.
                                        if (Eidle.read()) {
                                            request = REQ_E;        // Destination is at East.
                                        } else {
                                            request = REQ_NONE;
                                        }
                                    }
                                }
                            }
                        } else {
                            if ((Xdest & 1) || (X != 1))            // Odd dest. column or >= 2 columns to dest.
                                request = REQ_E;                    // Destination is at East.
                        }
                    }


                    ////////////////////////////////////////// Westbound messages.
                } else {
                    if (!(Xdest & 1)) {                     // Current router is at an even column
                        if (Y > 0) {                        // Destination is at Northwest.
                            if (Nidle.read())
                                request = REQ_N;
                            else
                                if (Widle.read())
                                    request = REQ_W;
                                else
                                    request = REQ_NONE;
                        } else {
                            if (Y < 0) {					// Destination is at Southwest.
                                if (Sidle.read())
                                    request = REQ_S;
                                else
                                    if (Widle.read())
                                        request = REQ_W;
                                    else
                                        request = REQ_NONE;
                            } else {                        // Destination is at West
                                request = REQ_W;
                            }
                        }
                    } else {                                // Current router is at an even column
                        request = REQ_W;                    // Destination is at West
                    }
                }
        }
    }

    // Outputs
    reqL.write(request[4]);
    reqN.write(request[3]);
    reqE.write(request[2]);
    reqS.write(request[1]);
    reqW.write(request[0]);
}
