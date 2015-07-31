/*******************************************************************************

 Company: 				UNIVALI
 Engineer:				Thiago Felski Pereira
 
 Create Date:			18/05/2009
 Design Name:			class_checker
 Module Name:    	class_checker.cpp
 Project Name:
 Target Devices:
 Tool versions:
 Description:

 Dependencies:

 Revision:
 Revision 0.01 - File Created
 Additional Comments:

*******************************************************************************/
#include "class_checker.h"

////////////////////////////////////////////////////////////////////////////////
void class_checker::p_class_checker()
////////////////////////////////////////////////////////////////////////////////
{
    // Internal signals
    bool v_IS_CLS_0;
    bool v_IS_CLS_1;
    bool v_IS_CLS_2;
    bool v_IS_CLS_3;
    bool v_LOW_CLS;

    // Variables
    sc_uint<2> v_CLS_0;
    sc_uint<2> v_CLS_1;
    sc_uint<2> v_CLS_2;
    sc_uint<2> v_CLS_3;
    v_CLS_0 = i_CLS_0.read();
    v_CLS_1 = i_CLS_1.read();
    v_CLS_2 = i_CLS_2.read();
    v_CLS_3 = i_CLS_3.read();

    // Spliting the classes - 0 or 1: 0 --> low; 1 --> high
    v_IS_CLS_0 = v_CLS_0[1] or v_CLS_0[0];
    v_IS_CLS_1 = v_CLS_1[1] or v_CLS_1[0];
    v_IS_CLS_2 = v_CLS_2[1] or v_CLS_2[0];
    v_IS_CLS_3 = v_CLS_3[1] or v_CLS_3[0];


    // Check all i_CLS at same time.
    v_LOW_CLS = v_IS_CLS_0 and v_IS_CLS_1 and v_IS_CLS_2 and v_IS_CLS_3;

    // Check i_CLS from 0.
    o_ENA_0.write( (not v_IS_CLS_0) or v_LOW_CLS );

    // Check i_CLS from 1.
    o_ENA_1.write( (not v_IS_CLS_1) or v_LOW_CLS );

    // Check i_CLS from 2.
    o_ENA_2.write( (not v_IS_CLS_2) or v_LOW_CLS );

    // Check i_CLS from 3.
    o_ENA_3.write( (not v_IS_CLS_3) or v_LOW_CLS );

}

////////////////////////////////////////////////////////////////////////////////
void class_checker::p_debug()
////////////////////////////////////////////////////////////////////////////////
{
    static long int cycle = 0;
    char port[5] = {'L','N','E','S','W'};

    printf("\n[class_checker(%d,%d)-%c] @ cycle %4ld",XID, YID,port[PORT_ID],cycle++);
    cout  << "   i_CLS_0=" << i_CLS_0.read() \
          << "   i_CLS_1=" << i_CLS_1.read() \
          << "   i_CLS_2=" << i_CLS_2.read() \
          << "   i_CLS_3=" << i_CLS_3.read() \
          << "     ||  o_ENA_0=" << o_ENA_0.read() \
          << "   o_ENA_1=" << o_ENA_1.read() \
          << "   o_ENA_2=" << o_ENA_2.read() \
          << "   o_ENA_3=" << o_ENA_3.read() \
          << "\t";
}

