/*******************************************************************************

 Company: 				UNIVALI
 Engineer:				Thiago Felski Pereira
 
 Create Date:			19/05/2009
 Design Name:			class_checker
 Module Name:    	class_checker.h
 Project Name:
 Target Devices:
 Tool versions:
 Description:

 Dependencies:

 Revision:
 Revision 0.01 - File Created
 Additional Comments:

*******************************************************************************/
#ifndef __CLASS_CHECKER_H__
#define __CLASS_CHECKER_H__

#include <systemc.h>

SC_MODULE(class_checker)
{
    // System signals
    //  sc_in_clk     		i_CLK;			// clock
    //  sc_in<bool>   		i_RST;			// reset

    // Class checker interface
    sc_in<sc_uint<2> >  i_CLS_0;
    sc_in<sc_uint<2> >  i_CLS_1;
    sc_in<sc_uint<2> >  i_CLS_2;
    sc_in<sc_uint<2> >  i_CLS_3;
    sc_out<bool>        o_ENA_0;
    sc_out<bool>        o_ENA_1;
    sc_out<bool>        o_ENA_2;
    sc_out<bool>        o_ENA_3;

    // Internal data structures
    unsigned short int XID, YID, PORT_ID;

    // Module's processes
    void p_class_checker();
    void p_debug();

    SC_HAS_PROCESS(class_checker);

    /////////////////////////////////////////////////
    class_checker(sc_module_name nm,	unsigned short int XID,
                  unsigned short int YID, unsigned short int PORT_ID) :
        sc_module(nm), XID(XID), YID(YID), PORT_ID(PORT_ID)
    /////////////////////////////////////////////////
    {
        SC_METHOD(p_class_checker);
        sensitive << i_CLS_0
                  << i_CLS_1
                  << i_CLS_2
                  << i_CLS_3;


#ifdef DEBUG_CLASS_CHECKER
        SC_METHOD(p_debug);
        sensitive << i_CLS_0
                  << i_CLS_1
                  << i_CLS_2
                  << i_CLS_3
                  << o_ENA_0
                  << o_ENA_1
                  << o_ENA_2
                  << o_ENA_3;
#endif
    }
};
#endif // __CLASS_CHECKER__
