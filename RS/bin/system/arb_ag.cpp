/*******************************************************************************

 Company: 				UNIVALI
 Engineer:				Thiago Felski Pereira
 
 Create Date:			20/05/2009
 Design Name:			arbiter_ag
 Module Name:    		arb_ag.cpp
 Project Name:
 Target Devices:
 Tool versions:
 Description:

 Dependencies:

 Revision:
 Revision 0.01 - File Created
 Additional Comments:

*******************************************************************************/
#include "arb_ag.h"

////////////////////////////////////////////////////////////////////////////////
void arb_ag::p_extract_age_cls_from_header()
////////////////////////////////////////////////////////////////////////////////
{
    // Variables
    sc_uint<FLIT_WIDTH>	v_DIN_0;
    sc_uint<FLIT_WIDTH>	v_DIN_1;
    sc_uint<FLIT_WIDTH>	v_DIN_2;
    sc_uint<FLIT_WIDTH>	v_DIN_3;
    sc_uint<N>					v_GO;

    v_DIN_0 = i_DIN_0.read();
    v_DIN_1 = i_DIN_1.read();
    v_DIN_2 = i_DIN_2.read();
    v_DIN_3 = i_DIN_3.read();

    /*
    // Enable time stamp checker based on status of class checker.
    w_AGE_0 <= i_AGE_0 when ((w_EN_CLS(0) = '1') and w_HP(0) = '1') else "000";
    w_AGE_1 <= i_AGE_1 when ((w_EN_CLS(1) = '1') and w_HP(1) = '1') else "000";
    w_AGE_2 <= i_AGE_2 when ((w_EN_CLS(2) = '1') and w_HP(2) = '1') else "000";
    w_AGE_3 <= i_AGE_3 when ((w_EN_CLS(3) = '1') and w_HP(3) = '1') else "000";
    */

    // Enable time stamp checker based on status of class checker
    if (w_EN_CLS_0.read() and w_HP_0.read())
        w_AGE_0.write(v_DIN_0.range(DATA_WIDTH - 10, DATA_WIDTH - 9 - AGE_WIDTH));
    else
        w_AGE_0.write(0);

    if (w_EN_CLS_1.read() and w_HP_1.read())
        w_AGE_1.write(v_DIN_1.range(DATA_WIDTH - 10, DATA_WIDTH - 9 - AGE_WIDTH));
    else
        w_AGE_0.write(0);

    if (w_EN_CLS_2.read() and w_HP_2.read())
        w_AGE_2.write(v_DIN_2.range(DATA_WIDTH - 10, DATA_WIDTH - 9 - AGE_WIDTH));
    else
        w_AGE_2.write(0);

    if (w_EN_CLS_3.read() and w_HP_3.read())
        w_AGE_3.write(v_DIN_3.range(DATA_WIDTH - 10, DATA_WIDTH - 9 - AGE_WIDTH));
    else
        w_AGE_3.write(0);

    /*
    // Check for priority class type.
    w_CLS_0 <= i_CLS_0 when (w_HP(0) = '1') else "01";
    w_CLS_1 <= i_CLS_1 when (w_HP(1) = '1') else "01";
    w_CLS_2 <= i_CLS_2 when (w_HP(2) = '1') else "01";
    w_CLS_3 <= i_CLS_3 when (w_HP(3) = '1') else "01";
    */
    // Check for priority class type.
    if (w_HP_0.read())
        w_CLS_0.write(v_DIN_0.range(DATA_WIDTH - AGE_WIDTH - 10, DATA_WIDTH - AGE_WIDTH - 11));
    else
        w_CLS_0.write(01);

    if (w_HP_1.read())
        w_CLS_1.write(v_DIN_1.range(DATA_WIDTH - AGE_WIDTH - 10, DATA_WIDTH - AGE_WIDTH - 11));
    else
        w_CLS_1.write(01);

    if (w_HP_2.read())
        w_CLS_2.write(v_DIN_2.range(DATA_WIDTH - AGE_WIDTH - 10, DATA_WIDTH - AGE_WIDTH - 11));
    else
        w_CLS_2.write(01);

    if (w_HP_3.read())
        w_CLS_3.write(v_DIN_3.range(DATA_WIDTH - AGE_WIDTH - 10, DATA_WIDTH - AGE_WIDTH - 11));
    else
        w_CLS_3.write(01);
}



////////////////////////////////////////////////////////////////////////////////
void arb_ag::p_detect_header_presence()
////////////////////////////////////////////////////////////////////////////////
{
    // Variables
    sc_uint<FLIT_WIDTH>	v_DIN_0;
    sc_uint<FLIT_WIDTH>	v_DIN_1;
    sc_uint<FLIT_WIDTH>	v_DIN_2;
    sc_uint<FLIT_WIDTH>	v_DIN_3;
    sc_uint<N>					v_GO;

    v_DIN_0 = i_DIN_0.read();
    v_DIN_1 = i_DIN_1.read();
    v_DIN_2 = i_DIN_2.read();
    v_DIN_3 = i_DIN_3.read();

    /*
    // Generate w_HP
    for i in 0 to p_N-1 generate
        begin
            w_HP(i) 	<= i_BOP(i) and i_ROK(i);
    end generate;
*/


    // Generate w_HP
    w_HP_0.write(v_DIN_0[DATA_WIDTH] and i_ROK_0.read());
    w_HP_1.write(v_DIN_1[DATA_WIDTH] and i_ROK_1.read());
    w_HP_2.write(v_DIN_2[DATA_WIDTH] and i_ROK_2.read());
    w_HP_3.write(v_DIN_3[DATA_WIDTH] and i_ROK_3.read());
}


////////////////////////////////////////////////////////////////////////////////
void arb_ag::p_priority_generator()
////////////////////////////////////////////////////////////////////////////////
{
    // Variables
    sc_uint<N>					v_GO;

    /*
    // Generate w_GO
    for i in 0 to p_N-1 generate
        begin
            w_GO_2(i)	<= w_EN_CLS(i) and w_EN_AGE(i);
    end generate;
*/


    // Generate w_GO
    v_GO[0] = (w_EN_CLS_0.read() and w_EN_AGE_0.read());
    v_GO[1] = (w_EN_CLS_1.read() and w_EN_AGE_1.read());
    v_GO[2] = (w_EN_CLS_2.read() and w_EN_AGE_2.read());
    v_GO[3] = (w_EN_CLS_3.read() and w_EN_AGE_3.read());
    w_GO.write(v_GO);
}


////////////////////////////////////////////////////////////////////////////////
void arb_ag::p_debug()
////////////////////////////////////////////////////////////////////////////////
{
    static long int cycle = 0;
    char port[5] = {'L','N','E','S','W'};

    printf("\n[arb_ag(%d,%d)-%c] @ cycle %4ld",XID, YID,port[PORT_ID],cycle++);
    printf(" i_DIN_0=0x%08x", (unsigned int) i_DIN_0.read());
    printf(" i_DIN_1=0x%08x", (unsigned int) i_DIN_1.read());
    printf(" i_DIN_2=0x%08x", (unsigned int) i_DIN_2.read());
    printf(" i_DIN_3=0x%08x", (unsigned int) i_DIN_3.read());
    printf(" w_GO=0x%x"     , (unsigned int) w_GO.read());
    printf(" o_GNT=0x%x"    , (unsigned int) o_GNT.read());

}

