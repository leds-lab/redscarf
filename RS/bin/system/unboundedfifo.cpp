#include "unboundedfifo.h"

void UnboundedFifo::fifoProcess() {

    if( i_RD.read() == 1 ) {    // Read - only remove data from the FIFO if it isn't empty
        if(!m_FIFO.empty()) {
            m_FIFO.pop();
        }

        if( m_FIFO.empty() ) {  // if the fifo is empty after remove the data, then reset the outputs
            o_DATA_OUT.write(0);
            o_RD_OK.write(0);
        } else {                // if fifo isn't empty after remove the data, then update the outputs
            o_DATA_OUT.write( m_FIFO.front() );
            o_RD_OK.write(1);
        }
    }

    if( i_WR.read() == 1 ) {    // It writes in fifo
        m_FIFO.push( i_DATA_IN.read() );  // Puts on the queue
        o_DATA_OUT.write(m_FIFO.front()); // Update the output data if necessary
        o_RD_OK.write(1);                 // Enable FIFO-!EMPTY because of the write
    }


}

