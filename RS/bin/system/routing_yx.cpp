/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : routing_xy
FILE   : routing_xy.cpp
--------------------------------------------------------------------------------
*/
#include "routing_yx.h"

////////////////////////////////////////////////////////////////////////////////
void routing_yx::p_req()
////////////////////////////////////////////////////////////////////////////////
{
    sc_uint<FLIT_WIDTH>  data_tmp;      // Used to extract fields from data
    sc_uint<RIB_WIDTH/2> Xdest;         // x-coordinate
    sc_uint<RIB_WIDTH/2> Ydest;         // y-coordinate
    bool                 bop;	        // packet framing bit: begin of packet
    bool                 header_present;// A header is in the FIFO's output
    sc_uint<5>           request;       //encoded request
    int                  X, Y;          // Aux. variables used for routing

    // It extracts the RIB fields and the framing bits
    data_tmp = data.read();
    Xdest = data_tmp.range(RIB_WIDTH-1, RIB_WIDTH/2);
    Ydest = data_tmp.range(RIB_WIDTH/2-1,0);
    bop   = data_tmp[FLIT_WIDTH-2];

    // It determines if a header is present
    if ((bop==1) && (rok.read()==1))
        header_present = 1;
    else
        header_present = 0;


    // It runs the routing algorithm
    if (header_present)
    {
        X = (int) Xdest - (int) XID;
        Y = (int) Ydest - (int) YID;

        if (Y != 0)
        {
            if (Y > 0)
                request = REQ_N;
            else
                request = REQ_S;
        }
        else if (X != 0)
        {
            if (X > 0)
                request = REQ_E;
            else
                request = REQ_W;
        }
        else // X == Y == 0
        {
            request = REQ_L;
        }
    }
    else
        request = REQ_NONE;

    
    // Outputs
    reqL.write(request[4]);
    reqN.write(request[3]);
    reqE.write(request[2]);
    reqS.write(request[1]);
    reqW.write(request[0]);
}
