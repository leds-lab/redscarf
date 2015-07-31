/*******************************************************************************

 Company: 				UNIVALI
 Engineer:				Thiago Felski Pereira
 
 Create Date:			18/05/2009
 Design Name:			stamp_checker
 Module Name:    	stamp_checker.cpp
 Project Name:
 Target Devices:
 Tool versions:
 Description:

 Dependencies:

 Revision:
 Revision 0.01 - File Created
 Additional Comments:

*******************************************************************************/
#include "stamp_checker.h"

////////////////////////////////////////////////////////////////////////////////
void stamp_checker::p_stamp_checker()
////////////////////////////////////////////////////////////////////////////////
{
    bool v_COMP_01;
    bool v_COMP_02;
    bool v_COMP_03;
    bool v_COMP_12;
    bool v_COMP_13;
    bool v_COMP_23;

//    bool temp;

    // Check i_AGE from 0.
    if ((i_AGE_0.read() >= i_AGE_1.read()) and (i_CLS_ENA_0.read()))
        v_COMP_01 = 1;
    else
        v_COMP_01 = 0;

    if ((i_AGE_0.read() >= i_AGE_2.read()) and (i_CLS_ENA_0.read()))
        v_COMP_02 = 1;
    else
        v_COMP_02 = 0;

    if ((i_AGE_0.read() >= i_AGE_3.read()) and (i_CLS_ENA_0.read()))
        v_COMP_03 = 1;
    else
        v_COMP_03 = 0;

    o_ENA_0.write(v_COMP_01 and v_COMP_02 and v_COMP_03);



    // Check i_AGE from 1.
    if ((i_AGE_1.read() >= i_AGE_2.read()) and (i_CLS_ENA_1.read()))
        v_COMP_12 = 1;
    else
        v_COMP_12 = 0;

    if ((i_AGE_1.read() >= i_AGE_3.read()) and (i_CLS_ENA_1.read()))
        v_COMP_13 = 1;
    else
        v_COMP_13 = 0;

    o_ENA_1.write( (not v_COMP_01) and v_COMP_12 and v_COMP_13);


    // Check i_AGE from 2.
    if ((i_AGE_2.read() >= i_AGE_3.read())  and (i_CLS_ENA_2.read()))
        v_COMP_23 = 1;
    else
        v_COMP_23 = 0;

    o_ENA_2.write((not v_COMP_02) and (not v_COMP_12) and v_COMP_23);

    // Check i_AGE from 3.
    o_ENA_3.write((not v_COMP_03) and (not v_COMP_13) and (not v_COMP_23));
}


////////////////////////////////////////////////////////////////////////////////
void stamp_checker::p_debug()
////////////////////////////////////////////////////////////////////////////////
{
    static long int cycle = 0;
    char port[5] = {'L','N','E','S','W'};

    printf("\n[stamp_checker(%d,%d)-%c] @ cycle %4ld",XID, YID,port[PORT_ID],cycle++);
    cout  << "   i_AGE_0=" << i_AGE_0.read() \
          << "   i_AGE_1=" << i_AGE_1.read() \
          << "   i_AGE_2=" << i_AGE_2.read() \
          << "   i_AGE_3=" << i_AGE_3.read() \
          << " ||  o_ENA_0=" << o_ENA_0.read() \
          << "   o_ENA_1=" << o_ENA_1.read() \
          << "   o_ENA_2=" << o_ENA_2.read() \
          << "   o_ENA_3=" << o_ENA_3.read() \
          << "\t";
}

