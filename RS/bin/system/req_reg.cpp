/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : req_req (request register)
FILE	 : req_reg.cpp
--------------------------------------------------------------------------------
*/
#include "req_reg.h"

////////////////////////////////////////////////////////////////////////////////
void req_reg::p_reg()
////////////////////////////////////////////////////////////////////////////////
// This process implements a register which determines the state of the requests 
// to the output channels. 
{
    sc_uint<FLIT_WIDTH> data_tmp; // Used to extract fields from din
    bool                bop;      // packet framing bit: begin of packet
    bool                eop;      // packet framing bit: end of packet

    // It extracts the the framing bits
    data_tmp = data.read();
    bop = data_tmp[FLIT_WIDTH-2];
    eop = data_tmp[FLIT_WIDTH-1];

    if (rst.read()==1) {
        reqL.write(0);
        reqN.write(0);
        reqS.write(0);
        reqE.write(0);
        reqW.write(0);
    } else {
        // If there is no registered request and a header is present,
        // it registers the new request determined by the routing function
        if ((rok.read()==1) && (bop==1) && (requesting.read()==0)) {

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
            if ((rok.read()==1) && (eop==1) && (rd.read()==1)) {
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
void req_reg::p_outputs()
////////////////////////////////////////////////////////////////////////////////
// Updates the outputs
{
    out_reqL.write(reqL.read());
    out_reqN.write(reqN.read());
    out_reqE.write(reqE.read());
    out_reqS.write(reqS.read());
    out_reqW.write(reqW.read());
    requesting.write(reqL.read()|reqN.read()|reqE.read()|reqS.read()|reqW.read());
}


///////////////////////////////////// SoCIN LP: function for acivity measurement
void req_reg::p_probe_switching_activity()
////////////////////////////////////////////////////////////////////////////////
// This process implements a function that counts the number of transictions in
// registers
{
    static FILE *fp;
    static unsigned int  file_status = 0;
    static unsigned long int transictions_counter = 0;
    char 								 port[5] = {'L','N','E','S','W'};

    if (file_status==0) {
        if ((fp = fopen("work/req_reg.saf","wt")) == NULL ) {
            printf("\nRedScarf: Cannot open work/req_reg.saf\n");
            exit(0);
        } else {
            //			printf("\nRedScarf: File req_reg.saf was opened");
            file_status = 1;
        }
    }

    reqL_delayed.write(reqL.read());
    reqN_delayed.write(reqN.read());
    reqE_delayed.write(reqE.read());
    reqS_delayed.write(reqS.read());
    reqW_delayed.write(reqW.read());


    if (reqL.read() != reqL_delayed.read()) {
        transictions_counter++;
        fprintf(fp,"(%d,%d,%c):%ld\n",XID, YID,port[PORT_ID],transictions_counter);
        //printf("\n(%d,%d,%c) %d:%d - %u", XID, YID,port[PORT_ID], reqL.read(),reqL_delayed.read(),transictions_counter);
    }

    if (reqN.read() != reqN_delayed.read()) {
        transictions_counter++;
        fprintf(fp,"(%d,%d,%c):%ld\n",XID, YID,port[PORT_ID],transictions_counter);
        //printf("\n(%d,%d,%c) %d:%d - %u", XID, YID,port[PORT_ID], reqN.read(),reqN_delayed.read(),transictions_counter);
    }

    if (reqE.read() != reqE_delayed.read()) {
        transictions_counter++;
        fprintf(fp,"(%d,%d,%c):%ld\n",XID, YID,port[PORT_ID],transictions_counter);
        //printf("\n(%d,%d,%c) %d:%d - %u", XID, YID,port[PORT_ID], reqE.read(),reqE_delayed.read(),transictions_counter);
    }

    if (reqS.read() != reqS_delayed.read()) {
        transictions_counter++;
        fprintf(fp,"(%d,%d,%c):%ld\n",XID, YID,port[PORT_ID],transictions_counter);
        //printf("\n(%d,%d,%c) %d:%d - %u", XID, YID,port[PORT_ID], reqS.read(),reqS_delayed.read(),transictions_counter);
    }

    if (reqW.read() != reqW_delayed.read()) {
        transictions_counter++;
        fprintf(fp,"(%d,%d,%c):%ld\n",XID, YID,port[PORT_ID],transictions_counter);
        //printf("\n(%d,%d,%c) %d:%d - %u", XID, YID,port[PORT_ID], reqW.read(),reqW_delayed.read(),transictions_counter);
    }
}


////////////////////////////////////////////////////////////////////////////////
void req_reg::p_debug()
////////////////////////////////////////////////////////////////////////////////
{
    static long int      cycle = 0;
    char                 port[5] = {'L','N','E','S','W'};
    sc_uint<FLIT_WIDTH>	 data_tmp; // Used to extract fields from din
//    bool                 bop;      // packet framing bit: begin of packet
//    bool                 eop;      // packet framing bit: end of packet
    short int            xdest;
    short int            ydest;

    data_tmp = data.read();
    xdest = data_tmp.range(RIB_WIDTH-1, RIB_WIDTH/2);
    ydest = data_tmp.range(RIB_WIDTH/2-1,0);

    printf("\n[req_reg   (%d,%d)-%c] @ cycle %ld",XID, YID,port[PORT_ID],cycle++);
    cout << "  rok=" 	<< rok.read();
    printf("  Xdest=0x%x", xdest);
    printf("  Ydest=0x%x", ydest);
    cout << "  bop=" 	<< data_tmp[FLIT_WIDTH-2];
    cout << "  eop=" 	<< data_tmp[FLIT_WIDTH-1];
    cout << "  rd="  	<< rd.read();
    cout << "  in_reqLNESW="<< in_reqL.read() << in_reqN.read() \
         << in_reqE.read() << in_reqS.read() << in_reqW.read();
    cout << "  reqLNESW=" 	<< reqL.read() << reqN.read() << reqE.read() \
         << reqS.read() << reqW.read();
    cout << "  requesting=" << requesting.read();
}
