/*******************************************************************************

 Company: 	UNIVALI
 Engineer:	Thiago Felski Pereira
 
 Create Date:  18/05/2009
 Design Name:  stamp_checker
 Module Name:  stamp_checker.h
 Project Name:
 Target Devices:
 Tool versions:
 Description:

 Dependencies:

 Revision:
 Revision 0.01 - File Created
 Additional Comments:

*******************************************************************************/
#ifndef __STAMP_CHECKER_H__
#define __STAMP_CHECKER_H__

#include <systemc.h>
#include "parameters.h"

SC_MODULE(stamp_checker)
{
    // System signals
    //  sc_in_clk	i_CLK;  // clock
    //  sc_in<bool> i_RST;  // reset

    // Class checker interface
    sc_in<bool>                 i_CLS_ENA_0;
    sc_in<bool>                 i_CLS_ENA_1;
    sc_in<bool>                 i_CLS_ENA_2;
    sc_in<bool>                 i_CLS_ENA_3;
    sc_in<sc_uint<AGE_WIDTH> >  i_AGE_0;
    sc_in<sc_uint<AGE_WIDTH> >  i_AGE_1;
    sc_in<sc_uint<AGE_WIDTH> >  i_AGE_2;
    sc_in<sc_uint<AGE_WIDTH> >  i_AGE_3;
    sc_out<bool>                o_ENA_0;
    sc_out<bool>                o_ENA_1;
    sc_out<bool>                o_ENA_2;
    sc_out<bool>                o_ENA_3;

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    // Module's processes
    void p_stamp_checker();
    void p_debug();

    SC_HAS_PROCESS(stamp_checker);

    /////////////////////////////////////////////////
    stamp_checker(sc_module_name nm,	unsigned short int XID, unsigned short int YID,
                  unsigned short int PORT_ID) : sc_module(nm), XID(XID), YID(YID),
        PORT_ID(PORT_ID)
      /////////////////////////////////////////////////
    {
        SC_METHOD(p_stamp_checker);
        sensitive << i_AGE_0
                  << i_AGE_1
                  << i_AGE_2
                  << i_AGE_3
                  << i_CLS_ENA_0
                  << i_CLS_ENA_1
                  << i_CLS_ENA_2
                  << i_CLS_ENA_3;


#ifdef DEBUG_STAMP_CHECKER
        SC_METHOD(p_debug);
        sensitive << i_AGE_0
                  << i_AGE_1
                  << i_AGE_2
                  << i_AGE_3
                  << o_ENA_0
                  << o_ENA_1
                  << o_ENA_2
                  << o_ENA_3;
#endif

    }
};
#endif // __STAMP_CHECKER__
