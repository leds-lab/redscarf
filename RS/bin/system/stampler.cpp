/*******************************************************************************

 Company: 				Intelbras / UNIVALI
 Engineer:				Cesar Albenes Zeferino
                                    Marcelo Daniel Berejuck
                                    Thiago Felski Pereira

 Create Date:			13/05/2009
 Design Name:			stampler.cpp
 Module Name:			Stampler
 Project Name:
 Target Devices:
 Tool versions:
 Description:

 Dependencies:

 Revision:
 Revision 0.01 - File Created
 Additional Comments:

*******************************************************************************/
#include "stampler.h"
#include "stampler_control.cpp"
#include "stampler_counter.cpp"

////////////////////////////////////////////////////////////////////////////////
void stampler::p_framing()
////////////////////////////////////////////////////////////////////////////////
{
    sc_uint<FLIT_WIDTH>   v_DIN;
    v_DIN =	i_DIN.read();

    // It extracts the bop
    w_BOP.write(v_DIN[FLIT_WIDTH-2]);

    // It determines if a header is present (=bop & rok) - it is used by stampler_control
    w_HEADER_PRESENT.write(v_DIN[FLIT_WIDTH-2] and i_ROK.read());
}



////////////////////////////////////////////////////////////////////////////////
void stampler::p_stampler()
////////////////////////////////////////////////////////////////////////////////
{
    sc_uint<FLIT_WIDTH> v_DIN;
    sc_uint<FLIT_WIDTH> v_DIN_AGE;      // VHDL Original name: v_DIN_AGN
    sc_uint<AGE_WIDTH>  v_AGE;          // VHDL Original name: v_AGN
    sc_uint<AGE_WIDTH>  v_RES;
    sc_uint<AGE_WIDTH>  v_REF_MAX_AGE;  // VHDL original name: v_REF
    bool                v_HEADER_PRESENT;

    v_REF_MAX_AGE = ~0;					// This operation is equivalent to (others => '1')

    // It copies the flit to a variable in order to allow bit level evaluation
    v_DIN =	i_DIN.read();

    // It determines if the flit is a header
    v_HEADER_PRESENT = (v_DIN[FLIT_WIDTH-2] and i_ROK.read());

    // It selects the flit to be sent (the updated header or a payload/trailer field)
    if (v_HEADER_PRESENT) {
        // It extracts the current age field from the header
        v_AGE = v_DIN.range(DATA_WIDTH - 10, DATA_WIDTH - (10+AGE_WIDTH-1));
        v_RES = v_AGE + w_NEW_AGE.read();

        // It builds the new header  (NOTE: It must be updated in the VHDL code)
        v_DIN_AGE.range(19, 0) = v_DIN.range(19, 0);
        v_DIN_AGE.range(20+AGE_WIDTH-1, 20) = v_RES;
        v_DIN_AGE.range(FLIT_WIDTH-1, 20+AGE_WIDTH) = v_DIN.range(FLIT_WIDTH-1, 20+AGE_WIDTH);

        o_DOUT.write(v_DIN_AGE);
    } else {
        o_DOUT.write(i_DIN.read());
    }
}


////////////////////////////////////////////////////////////////////////////////
void stampler::p_debug()
////////////////////////////////////////////////////////////////////////////////
{
    static long int cycle = 0;
    char port[5] = {'L','N','E','S','W'};

    printf("\n[stampler(%d,%d)-%c:%d] @ cycle %ld",XID, YID,port[PORT_ID],CHANNEL_ID,cycle++);
    cout  << " w_NEW_AGE = " << w_NEW_AGE.read() \
          << " w_HEADER_PRESENT = " << w_HEADER_PRESENT.read() \
          << "\t";
    printf("  o_DOUT = 0x%08X\n", (int) o_DOUT.read());
}
