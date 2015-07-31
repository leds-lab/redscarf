#include "global_clock.h"

////////////////////////////////////////////////////////////////////////////////
void global_clock::p_clock()
////////////////////////////////////////////////////////////////////////////////
{ 
    if (rst.read()==1) {
        clock_counter.write(0);
        clock_cycles.write(0);
        wait();
    } else {
        while(1) {
            clock_counter.write(clock_counter.read() + 1);
            clock_cycles.write(clock_counter.read());
            wait();
            //      printf("\n\nCYCLE 0x%08X ",(int) clock_counter.read());
        }
    }
}
