/*******************************************************************************

 Company: 				UNIVALI
 Engineer:				Thiago Felski Pereira 
 
 Create Date:			12/05/2009 
 Design Name:			Stampler Counter 
 Module Name:    	stampler_counter.cpp
 Project Name: 			
 Target Devices: 
 Tool versions: 
 Description: 

 Dependencies: 

 Revision: 
 Revision 0.01 - File Created
 Additional Comments: 

*******************************************************************************/
#include "stampler_counter.h"

////////////////////////////////////////////////////////////////////////////////
void stampler_counter::p_clock_counter()
////////////////////////////////////////////////////////////////////////////////
{
	if (i_RST.read())
		w_CLOCK_COUNTER.write(0);
	else 
		if (i_CLEAR.read())
			w_CLOCK_COUNTER.write(0);
		else
			if (i_ENA.read())
				w_CLOCK_COUNTER.write(w_CLOCK_COUNTER.read() + 1);
}

////////////////////////////////////////////////////////////////////////////////
void stampler_counter::p_overflow_counter()
////////////////////////////////////////////////////////////////////////////////
{
    sc_uint<AGE_WIDTH>          v_REF_MAX_AGE;          // VHDL original name: v_REF
    sc_uint<AGE_CLOCK_WIDTH>    v_REF_CLOCK_COUNTER;    // VHDL original name: w_REF

    v_REF_MAX_AGE = ~0;         // This operation is equivalent to (others => '1')
    v_REF_CLOCK_COUNTER = ~0;   // This operation is equivalent to (others => '1')

	if (i_RST.read())
		w_AGE_COUNTER.write(0);
	else 
		if (i_CLEAR.read()) {		
			w_AGE_COUNTER.write(0);
		} else {
			if ((i_ENA.read()) and (w_CLOCK_COUNTER.read() == v_REF_CLOCK_COUNTER) and (w_AGE_COUNTER.read() < v_REF_MAX_AGE))
				w_AGE_COUNTER.write(w_AGE_COUNTER.read() + 1);
		}
	o_DOUT.write(w_AGE_COUNTER.read());
}


////////////////////////////////////////////////////////////////////////////////
void stampler_counter::p_debug()
////////////////////////////////////////////////////////////////////////////////
{
  static long int cycle = 0;
  char port[5] = {'L','N','E','S','W'};

  printf("\n[stampler_counter(%d,%d)-%c:%d] @ cycle %ld",XID, YID,port[PORT_ID],CHANNEL_ID,cycle++);
  cout  << "   i_CLEAR = " << i_CLEAR.read() \
        << "   i_ENA = "   << i_ENA.read() \
        << "   w_CLOCK_COUNTER = " << w_CLOCK_COUNTER.read() \
        << "   o_DOUT = " << o_DOUT.read() \
        << "\t";
}
