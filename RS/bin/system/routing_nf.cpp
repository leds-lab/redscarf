/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : routing_nf
FILE   : routing_nf.cpp
--------------------------------------------------------------------------------
*/
#include "routing_nf.h"

////////////////////////////////////////////////////////////////////////////////
void routing_nf::p_req()
////////////////////////////////////////////////////////////////////////////////
{
    sc_uint<FLIT_WIDTH>	 data_tmp;        // Used to extract fields from data
    sc_uint<RIB_WIDTH/2> Xdest;           // x-coordinate
    sc_uint<RIB_WIDTH/2> Ydest;           // y-coordinate
    bool                 bop;	          // packet framing bit: begin of packet
//    bool                 eop;             // packet framing bit: end of packet
    bool                 header_present;  // A header is in the FIFO's output
    sc_uint<5>           request;         //encoded request
    int                  X, Y;            // Aux. variables used for routing

    // It extracts the RIB fields and the framing bits
    data_tmp = data.read();
    Xdest = data_tmp.range(RIB_WIDTH-1, RIB_WIDTH/2);
    Ydest = data_tmp.range(RIB_WIDTH/2-1,0);
    bop   = data_tmp[FLIT_WIDTH-2];
//    eop   = data_tmp[FLIT_WIDTH-1];

    // It determines if a header is present
    if (bop && rok)
        header_present = true;
    else
        header_present = false;
    
    // It runs the routing algorithm
    if (header_present) {
        X = (int)Xdest - (int)XID;
        Y = (int)Ydest - (int)YID;

        if ((X < 0) || (Y < 0)) {
            if (X <0)
                request = REQ_W;                // Destination is at West region
            else
                request = REQ_S;                // Destination is at South region
        } else {
            if ((X > 0) || (Y > 0)) {
                if (X == 0)
                    request = REQ_N;            // Destination is at North
                else if (Y == 0)
                    request = REQ_E;            // Destination is at East
                else                            // Destination is at Northeast region
                    if (Nidle.read())
                        request = REQ_N;
                    else
                        if (Eidle.read())
                            request = REQ_E;
                        else
                            request = REQ_NONE;
            } else {
                request = REQ_L;                // Detination is the Local port
            }
        }
    } else {
        request = REQ_NONE;
    }
    
    // Outputs
    reqL.write(request[4]);
    reqN.write(request[3]);
    reqE.write(request[2]);
    reqS.write(request[1]);
    reqW.write(request[0]);
}
