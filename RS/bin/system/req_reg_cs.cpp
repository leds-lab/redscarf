/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : req_req_cs (request register circuit switching)
FILE	 : req_reg_cs.cpp
--------------------------------------------------------------------------------
*/
#include "req_reg_cs.h"

////////////////////////////////////////////////////////////////////////////////
void req_reg_cs::p_internal_signals()
////////////////////////////////////////////////////////////////////////////////
{
    sc_uint<FLIT_WIDTH> v_data; // Used to extract fields from din
    sc_uint<2>          cs_cmd; // Circuit-switching command
    bool v_bop;                 // Begin-of-packet marker
    bool v_eop;                 // End-of-packet marker
    bool v_allocate;            // Circuit-switching command is CS_ALLOCATE
    bool v_release;             // Circuit-switching command is CS_RELEASE
    bool v_cs_allocate;         // Allocate circuit
    bool v_cs_release;          // Release  circuit
    bool v_trailer_sent;        // Packet trailer was sent

    // It copies the data input to extract the framing and circuit switching commands/bits
    v_data = data.read();

    // It decodes the framing commands (bits)
    v_bop = v_data[FLIT_WIDTH-2];   //(not v_data[FLIT_TYPE_WIDTH-1]) and v_data[FLIT_TYPE_WIDTH-2];
    v_eop = v_data[FLIT_WIDTH-1];   //v_data[FLIT_TYPE_WIDTH-1]  and (not v_data[FLIT_TYPE_WIDTH-2]);

    // It decodes the CS command (allocate and release)
    cs_cmd[1]  = v_data[RIB_WIDTH*2+1];
    cs_cmd[0]  = v_data[RIB_WIDTH*2];
    v_allocate = (not cs_cmd[1]) and      cs_cmd[0];
    v_release  = (    cs_cmd[1]) and  not cs_cmd[0];

    // It checks if there is a header and if the header includes a circuit Allocate or Release command
    v_cs_allocate  = rok.read() and rd.read() and v_bop and v_allocate;
    v_cs_release   = rok.read() and rd.read() and v_bop and v_release;

    // It determines if a trailer of a packet was delivered (it is used to cancel the current connection)
    v_trailer_sent = rok.read() and rd.read() and v_eop and (not circuit_set.read());

    // It copies the variable to signals to send them to the other process
    bop.write(v_bop);
    cs_allocate.write(v_cs_allocate);
    cs_release.write(v_cs_release);
    trailer_sent.write(v_trailer_sent);
}


////////////////////////////////////////////////////////////////////////////////
void req_reg_cs::p_reg()
////////////////////////////////////////////////////////////////////////////////
// This process implements a register which determines the state of the requests 
// to the output channels. 
{
    if (rst.read()==1) {
        reqL.write(0);
        reqN.write(0);
        reqS.write(0);
        reqE.write(0);
        reqW.write(0);

    } else {

        // If there is no registered request and a header is present,
        // it registers the new request determined by the routing function
        if ((rok.read()==1) && (bop.read()==1) && (requesting.read()==0)) {

            // REQUEST LOCAL (reqL is not registered by module L)
            if (PORT_ID==LOCAL_ID)
                reqL.write(0);
            else
                reqL.write(in_reqL.read());

            // REQUEST NORTH (reqN is not registered by module N)
            if (PORT_ID==NORTH_ID)
                reqN.write(0);
            else
                reqN.write(in_reqN.read());

            // REQUEST EAST (reqE is not registered by module E
            if (PORT_ID==EAST_ID)
                reqE.write(0);
            else
                reqE.write(in_reqE.read());

            // REQUEST SOUTH (reqS is not registered by module S)
            if (PORT_ID==SOUTH_ID)
                reqS.write(0);
            else
                reqS.write(in_reqS.read());

            // REQUEST WEST (reqW is not registered by module W)
            if (PORT_ID==WEST_ID)
                reqW.write(0);
            else
                reqW.write(in_reqW.read());

        } else {
            if (trailer_sent.read()){
                reqL.write(0);
                reqN.write(0);
                reqS.write(0);
                reqE.write(0);
                reqW.write(0);
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void req_reg_cs::p_circuit_set()
////////////////////////////////////////////////////////////////////////////////
{
    if (rst.read() == 1)
        circuit_set.write(0);
    else
        if ((cs_allocate.read()==1) || (cs_release.read()==1)) {
            if (cs_allocate.read()==1) {
                // If there is a CS_ALLOCATE command, it sets   the circuit
                circuit_set.write(1);
            } else {
                // If there is a CS_RELEASE  command, it resets the circuit
                circuit_set.write(0);
            }
        } else {
            // In the other cases, it holds the current state
            circuit_set.write(circuit_set.read());
        }
}


////////////////////////////////////////////////////////////////////////////////
void req_reg_cs::p_outputs()
////////////////////////////////////////////////////////////////////////////////
// Updates the outputs
{
    out_reqL.write(reqL.read());
    out_reqN.write(reqN.read());
    out_reqE.write(reqE.read());
    out_reqS.write(reqS.read());
    out_reqW.write(reqW.read());
    requesting.write(reqL.read() | reqN.read() | reqE.read() | reqS.read() | reqW.read());
}

////////////////////////////////////////////////////////////////////////////////
void req_reg_cs::p_debug()
////////////////////////////////////////////////////////////////////////////////
{
    static long int     cycle = 0;
    char 				port[5] = {'L','N','E','S','W'};
    sc_uint<FLIT_WIDTH>	data_tmp; // Used to extract fields from din
//    bool                bop;	  // packet framing bit: begin of packet
//    bool                eop;      // packet framing bit: end of packet
    short int           xdest;
    short int           ydest;

    data_tmp = data.read();
    xdest = data_tmp.range(RIB_WIDTH-1, RIB_WIDTH/2);
    ydest = data_tmp.range(RIB_WIDTH/2-1,0);

    printf("\n[req_reg   (%d,%d)-%c] @ cycle %ld",XID, YID,port[PORT_ID],cycle++);
    cout << " rok=" 	<< rok.read();
    printf( " data=0x%08X", (int) data_tmp);
    printf( " Xdest=0x%x", xdest);
    printf( " Ydest=0x%x", ydest);
    cout << " bop=" 	<< data_tmp[FLIT_WIDTH-2];
    cout << " eop=" 	<< data_tmp[FLIT_WIDTH-1];
    cout << " rd="  	<< rd.read();
    cout << " cs_allocate ="<< cs_allocate.read();
    cout << " cs_release  ="<< cs_release.read();
    cout << " circuit_set ="<< circuit_set.read();
    //cout << "  in_reqLNESW="<< in_reqL.read() << in_reqN.read() \<< in_reqE.read() << in_reqS.read() << in_reqW.read();
    //cout << "  reqLNESW=" 	<< reqL.read() << reqN.read() << reqE.read() \ << reqS.read() << reqW.read();
    cout << "  requesting=" << requesting.read();

}
