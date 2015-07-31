#ifndef UNBOUNDEDFIFO_H
#define UNBOUNDEDFIFO_H

#include <systemc.h>
#include <queue>

#include "parameters.h"

class UnboundedFifo : public sc_module {
public:
    // System signals
    sc_in_clk                        i_CLK;   // clock
    // FIFO interface
    sc_in<sc_uint<FLIT_WIDTH> >      i_DATA_IN; // Input data channel
    sc_in<bool>                      i_RD;      // Command to read a data from the FIFO
    sc_in<bool>                      i_WR;      // Command to write a data into FIFO
    sc_out<bool>                     o_RD_OK;   // FIFO has a data to be read (not empty)
    sc_out<bool>                     o_WR_OK;   // FIFO has room to be written (not full)
    sc_out<sc_uint<FLIT_WIDTH> >     o_DATA_OUT;// Output data channel

    std::queue<sc_uint<FLIT_WIDTH> > m_FIFO; // The FIFO

    // Module's processes
    void fifoProcess();

    SC_HAS_PROCESS(UnboundedFifo);
    //////////////////////////////////////////////////////////////////////////////
    UnboundedFifo(sc_module_name nm) : sc_module(nm) {
    //////////////////////////////////////////////////////////////////////////////

        o_WR_OK.initialize(1);

        SC_METHOD(fifoProcess);
        sensitive << i_CLK.pos();
    }

};


#endif // UNBOUNDEDFIFO_H
