/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : fifo_out
FILE   : fifo_out.cpp
--------------------------------------------------------------------------------
*/
#include "fifo_out.h"

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// CONTROLLER /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void fifo_out::p_next_state()
////////////////////////////////////////////////////////////////////////////////
// This process determines the next state of the FIFO taking into account
// the current state and the write and read commands, as follows:
{
    if (state_reg.read() == 0) { // FIFO EMPTY
        if (wr.read()) 	   next_state.write(state_reg.read()+1);
        else					 		 next_state.write(state_reg.read());
    } else {
        if (state_reg.read() == FIFO_OUT_DEPTH) {	  //FIFO FULL
            if (rd.read())   next_state.write(state_reg.read()-1);
            else						 next_state.write(state_reg.read());
        } else {  // FIFO NEITHER EMPTY, NEITHER FULL
            if (wr.read()==1) {
                if (rd.read()) next_state.write(state_reg.read());				//   rd &  wr
                else					 next_state.write(state_reg.read()+1);			//  /rd &  wr
            } else {
                if (rd.read()) next_state.write(state_reg.read()-1);			//   rd & /wr
                else					 next_state.write(state_reg.read()); 	 		  //  /rd & /wr
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void fifo_out::p_current_state()
////////////////////////////////////////////////////////////////////////////////
// This process implements the register that stores the state of the control fsm
{
    if (rst.read()==1){
        state_reg.write(0);
    } else {
        state_reg.write(next_state.read());;
    }
}

////////////////////////////////////////////////////////////////////////////////
void fifo_out::p_debug_controller()
////////////////////////////////////////////////////////////////////////////////
// This proces is available only for debugging purpose
{
    static long int cycle = 0;
    char port[5] = {'L','N','E','S','W'};

    printf("\n[fifo_out_c(%d,%d)-%c] @ cycle %ld",XID, YID,port[PORT_ID],cycle++);
    cout << "  next_state=" << next_state.read() \
         << "  state_reg=" << state_reg.read() \
         << "\t";
}


////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// DATAPATH ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void fifo_out::p_next_wr_ptr()
////////////////////////////////////////////////////////////////////////////////
// It determines the next write pointer, by incrementing the current one when 
// the FIFO is not full and there is a writing into the FIFO.
{
    if ((state_reg.read()!=FIFO_OUT_DEPTH) && (wr.read()==1)) {
        if (wr_ptr_reg.read() == (unsigned int)(FIFO_OUT_DEPTH - 1))
            next_wr_ptr.write(0);
        else
            next_wr_ptr.write(wr_ptr_reg.read() + 1);
    }
    else
        next_wr_ptr.write(wr_ptr_reg.read());
}

////////////////////////////////////////////////////////////////////////////////
void fifo_out::p_next_rd_ptr()
////////////////////////////////////////////////////////////////////////////////
// It determines the next read pointer, by decrementing the current one when the 
// FIFO is not empty and there is a reading from the FIFO.
{
    if ((state_reg.read()!=0) && (rd.read()==1)) {
        if (rd_ptr_reg.read() == (unsigned int) (FIFO_OUT_DEPTH-1))
            next_rd_ptr.write(0);
        else
            next_rd_ptr.write(rd_ptr_reg.read() + 1);
    }
    else
        next_rd_ptr.write(rd_ptr_reg.read());
}

////////////////////////////////////////////////////////////////////////////////
void fifo_out::p_reg()
////////////////////////////////////////////////////////////////////////////////
// It implements the pointer registers
{
    if (rst.read()) {
        wr_ptr_reg.write(0);
        rd_ptr_reg.write(0);
    }
    else {
        wr_ptr_reg.write(next_wr_ptr.read());
        rd_ptr_reg.write(next_rd_ptr.read());
    }
}

////////////////////////////////////////////////////////////////////////////////
void fifo_out::p_wr_fifo()
////////////////////////////////////////////////////////////////////////////////
// It implements the FIFO memory
{
    if ((wr.read()) && (state_reg.read()!=FIFO_OUT_DEPTH)) {
        for (unsigned int index=0; index<FIFO_OUT_DEPTH; index++) {
            if (index == wr_ptr_reg.read())
                fifo[index].write(din.read());
            else
                fifo[index].write(fifo[index].read());
        }
    }

    // The code above is a VHDL2SystemC mapping from the original VHDL code, but
    // it could be implemented just like below:

    /* if ((wr.read()) && (state_reg.read()< FIFO_OUT_DEPTH)) {
           fifo[wr_ptr_reg.read()].write(din.read());
    } */
}

////////////////////////////////////////////////////////////////////////////////
void fifo_out::p_outputs()
////////////////////////////////////////////////////////////////////////////////
// It updates the outputs
{
    dout.write(fifo[rd_ptr_reg.read()].read());

    if (state_reg.read() == 0) {
        rok.write(0);
        wok.write(1);
    }
    else {
        if (state_reg.read() == FIFO_OUT_DEPTH) {
            rok.write(1);
            wok.write(0);
        }
        else {
            rok.write(1);
            wok.write(1);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void fifo_out::p_debug_datapath()
////////////////////////////////////////////////////////////////////////////////
// This process is available only for debugging purpose
{
    static long int cycle = 0;
    char port[5] = {'L','N','E','S','W'};
    int temp;

    printf("\n[fifo_out_d(%d,%d)-%c] @ cycle %ld",XID, YID,port[PORT_ID],cycle++);
    cout << "  state=" << state_reg.read();

    cout << "  FIFO[" << FIFO_OUT_DEPTH-1 << "..0]=";
    for (int j = 0; j < FIFO_OUT_DEPTH; j++) {
        temp = fifo[j].read();
        printf("  0x%05x", temp);
    }

    temp = fifo[rd_ptr_reg.read()].read();
    printf("  FIFO_OUT_output=0x%05x", temp);

    //  cout << "  wr=" << wr.read() << "  ";
    //  cout << "  rd=" << rd.read() << "  ";
    //  cout << "  rd_ptr=" << rd_ptr_reg.read() << "  ";
    //  cout << "  wr_ptr=" << wr_ptr_reg.read() << " ";
    //  cout << "  next_rd_ptr=" << next_rd_ptr.read() << "  ";
    //  cout << "  next_wr_ptr=" << next_wr_ptr.read() << "  ";
    cout << "\n";
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// NO FIFO /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void fifo_out::p_fifo_null()
////////////////////////////////////////////////////////////////////////////////
{
    rok.write(wr.read());
    wok.write(rd.read());
    dout.write(din.read());
}

