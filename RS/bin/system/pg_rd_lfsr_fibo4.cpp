/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : pg_rd_lfsr_fibo4 (priority_generator_random with Fibonacci 4-bit LFSR generator)
FILE	 : pg_rd_lfsr_fibo4.cpp
--------------------------------------------------------------------------------
*/
#include "pg_rd_lfsr_fibo4.h"

void pg_rd_lfsr_fibo4::init() {

    seed = XID ^ YID ^ PORT_ID;
    if(seed == 0) {
        seed = 1; // Because value 0 it is no accepted
    }
}

////////////////////////////////////////////////////////////////////////////////
void pg_rd_lfsr_fibo4::p_update_registers() {
////////////////////////////////////////////////////////////////////////////////

    sc_uint<4> v_REGS_TEMP;

    // If th system is reseted so reinitialize with the seed
    if( rst.read() == 1 ) {
        v_REGS_TEMP = seed;
    } else {
        v_REGS_TEMP = s_REGS.read();
        // The polynomial: X^2 + X^1 + 1
        bool newMSB;
        newMSB = (v_REGS_TEMP[1]^v_REGS_TEMP[0]);
        v_REGS_TEMP.range(2,0) = v_REGS_TEMP.range(3,1);
        v_REGS_TEMP[3] = newMSB;
    }

    s_REGS.write(v_REGS_TEMP);

}

////////////////////////////////////////////////////////////////////////////////
void pg_rd_lfsr_fibo4::p_outputs() {
////////////////////////////////////////////////////////////////////////////////

    sc_uint<2> v_TWO_LSB;   // Get the 2 least significant bits from LFSR
    sc_uint<N> v_P_tmp;     // Generates the priority output

    v_TWO_LSB = s_REGS.read().range(1,0);
    /* Truth table
     * X Y | [3] [2] [1] [0]
     * 0 0 |  0   0   0   1
     * 0 1 |  0   0   1   0
     * 1 0 |  0   1   0   0
     * 1 1 |  1   0   0   0
     *
     */
    v_P_tmp[0] = (not v_TWO_LSB[1]) and (not v_TWO_LSB[0]) ;
    v_P_tmp[1] = (not v_TWO_LSB[1]) and (v_TWO_LSB[0]);
    v_P_tmp[2] = (v_TWO_LSB[1]) and (not v_TWO_LSB[0]);
    v_P_tmp[3] = (v_TWO_LSB[1]) and (v_TWO_LSB[0]);

    P.write( v_P_tmp );

}

////////////////////////////////////////////////////////////////////////////////
void pg_rd_lfsr_fibo4::p_debug()
////////////////////////////////////////////////////////////////////////////////
// 
{
    sc_uint<N> tmp;
    short int i;
    static long int cycle = 0;
    char port[5] = {'L','N','E','S','W'};

    printf("\n[pg        (%d,%d)-%c] @ cycle %ld",XID, YID,port[PORT_ID],cycle++);

    cout << "  G=";
    tmp = G.read();
    for(i=N-1; i>=0; i--) cout << tmp[i];

    cout << "  P=";
    tmp = P.read();
    for(i=N-1; i>=0; i--) cout << tmp[i];

    cout << "\t";
}
