/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : pg_rt (priority_generator_rotative)
FILE	 : pg_rt.cpp
--------------------------------------------------------------------------------
*/
#include "pg_rt.h"

////////////////////////////////////////////////////////////////////////////////
void pg_rt::p_gdelayed()
////////////////////////////////////////////////////////////////////////////////
// It's just a d-type register that holds the state of G for one clock cycle
{
    if (rst.read())
        Gdelayed.write(0);
    else
        Gdelayed.write(G.read());
}

////////////////////////////////////////////////////////////////////////////////
void pg_rt::p_update_register()
////////////////////////////////////////////////////////////////////////////////
// It just implements a parameterizable OR which detect if any request was
// granted in the last cycle. In this case, it enables the priority register 
// to update its state.
{
    sc_uint<N> granting;       // A request was granted
    sc_uint<N> G_tmp;		   // A copy of G for bit-level operations
    sc_uint<N> Gdelayed_tmp;   // A copy of Gdelayed for bit-level ops
    bool update_register_tmp;  // A temp. var. used to calculate update_register
    short int i;               // A variable for loops

    G_tmp = G.read();
    Gdelayed_tmp = Gdelayed.read();

    // It first determines if there exists any request that was granted in the
    // last cycle.
    for (i = 0; i < N; i++)
        granting[i] = G_tmp[i] and (not Gdelayed_tmp[i]);

    // Then, it makes an OR operation among all the granting(i) bits
    update_register_tmp = 0;
    for (i = 0; i < N; i++)
        update_register_tmp = update_register_tmp | granting[i];
    
    update_register.write(update_register_tmp);
}


////////////////////////////////////////////////////////////////////////////////
void pg_rt::p_nextp()
////////////////////////////////////////////////////////////////////////////////
// It determines the next priority order by rotating 1x left the current 
// priority status. Ex. If Preg="0001", then, nextP="0010". Such rotation will
// ensure that the current priority (e.g. R(0)) will have the lowest
// priority level at the next arbitration cycle (e.g. P(1)>P(2)> P(3)>P(0)).
{
    sc_uint<N> Preg_tmp;    // A copy of Preg for bit-level operations
    sc_uint<N> nextP_tmp;   // A temp. var. used to calculate nextP

    Preg_tmp = Preg.read();

    nextP_tmp[0] = Preg_tmp[N-1];
    for (unsigned short int i = 1; i < N; i++)
        nextP_tmp[i] = Preg_tmp[i-1];

    nextP.write(nextP_tmp);
}


////////////////////////////////////////////////////////////////////////////////
void pg_rt::p_preg()
////////////////////////////////////////////////////////////////////////////////
// It is reset with bit 0 in 1 and the others in 0, and is updated at each 
// arbitration cycle (after a request is grant) with the value determined
// for nextP.
{
    sc_uint<N> nextP_tmp;   // A copy of nextP_tmp for bit-level operations
    sc_uint<N> Preg_tmp;    // A temp. var. used to calculate Preg

    Preg_tmp 	= Preg.read();
    nextP_tmp	= nextP.read();

    if (rst.read()) {
        Preg_tmp[0] = 1;
    } else {
        if (update_register.read())
            Preg_tmp[0] = nextP_tmp[0];
    }

    for (unsigned short int i = 1; i < N; i++) {
        if (rst.read()) {
            Preg_tmp[i] = 0;
        } else {
            if (update_register.read())
                Preg_tmp[i] = nextP_tmp[i];
        }
    }

    Preg.write(Preg_tmp);
}

////////////////////////////////////////////////////////////////////////////////
void pg_rt::p_outputs()
////////////////////////////////////////////////////////////////////////////////
// It updates the output
{
    P.write(Preg.read());
}

////////////////////////////////////////////////////////////////////////////////
void pg_rt::p_debug()
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

    cout << "  Gdelayed=";
    tmp = Gdelayed.read();
    for(i=N-1; i>=0; i--) cout << tmp[i];

    cout << "  update_register=" << update_register.read();

    cout << "  nextP=";
    tmp = nextP.read();
    for(i=N-1; i>=0; i--) cout << tmp[i];

    cout << "  Preg=";
    tmp = Preg.read();
    for(i=N-1; i>=0; i--) cout << tmp[i];

    cout << "  P=";
    tmp = P.read();
    for(i=N-1; i>=0; i--) cout << tmp[i];

    cout << "\t";
}
