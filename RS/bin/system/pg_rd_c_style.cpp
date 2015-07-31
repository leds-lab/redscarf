/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : pg_rd_c_style (priority_generator_random)
FILE	 : pg_rd_c_style.cpp
--------------------------------------------------------------------------------
*/
#include "pg_rd_c_style.h"

void pg_rd_c_style::init() {

    seed = XID ^ YID ^ PORT_ID;
    srand(seed);
}

////////////////////////////////////////////////////////////////////////////////
void pg_rd_c_style::p_output()
////////////////////////////////////////////////////////////////////////////////
{
    sc_uint<N> v_P_tmp;     // Generates the priority output
    sc_uint<2> v_RND_NUMBER;
    v_RND_NUMBER = rand() % 4;


    /* Truth table
     * X Y | [3] [2] [1] [0]
     * 0 0 |  0   0   0   1
     * 0 1 |  0   0   1   0
     * 1 0 |  0   1   0   0
     * 1 1 |  1   0   0   0
     *
     */
    v_P_tmp[0] = (not v_RND_NUMBER[1]) and (not v_RND_NUMBER[0]) ;
    v_P_tmp[1] = (not v_RND_NUMBER[1]) and (v_RND_NUMBER[0]);
    v_P_tmp[2] = (v_RND_NUMBER[1]) and (not v_RND_NUMBER[0]);
    v_P_tmp[3] = (v_RND_NUMBER[1]) and (v_RND_NUMBER[0]);

    P.write( v_P_tmp );

}

////////////////////////////////////////////////////////////////////////////////
void pg_rd_c_style::p_debug()
////////////////////////////////////////////////////////////////////////////////
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
