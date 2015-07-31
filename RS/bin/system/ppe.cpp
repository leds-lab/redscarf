/*
--------------------------------------------------------------------------------
PROJECT: ParIS
MODULE : ppe (priority_generator)
FILE	 : ppe.h
--------------------------------------------------------------------------------
*/
#include "ppe.h"

////////////////////////////////////////////////////////////////////////////////
void ppe::p_imed_in()
////////////////////////////////////////////////////////////////////////////////
// Status from the previous arbitration cell 
{
    sc_uint<N> Imed_out_tmp;// A copy of Imed_out for bit-level operations
    sc_uint<N> Imed_in_tmp; // A temp. var. used to calculate Imed_in

    Imed_out_tmp = Imed_out.read();

    Imed_in_tmp[0] = Imed_out_tmp[N-1];
    for(unsigned short int i=1; i<N;i++)
        Imed_in_tmp[i] = Imed_out_tmp[i-1];

    Imed_in.write(Imed_in_tmp);
}


////////////////////////////////////////////////////////////////////////////////
void ppe::p_grant()
////////////////////////////////////////////////////////////////////////////////
// Grant signal sent to the requesting block 
{
    sc_uint<N> Imed_in_tmp;	// A copy of Imed_in for bit-level operations
    sc_uint<N> R_tmp;       // A copy of R for bit-level operations
    sc_uint<N> P_tmp;       // A copy of P for bit-level operations
    sc_uint<N> Grant_tmp;   // A temp. var. used to calculate Grant

    Imed_in_tmp = Imed_in.read();
    R_tmp       = R.read();
    P_tmp       = P.read();

    for(unsigned short int i=0; i<N;i++)
        Grant_tmp[i] = R_tmp[i] and (not (Imed_in_tmp[i] and (not P_tmp[i]) ) );

    Grant.write(Grant_tmp);
}


////////////////////////////////////////////////////////////////////////////////
void ppe::p_imed_out()
////////////////////////////////////////////////////////////////////////////////
// Status to the next arbitration cell  
{
    sc_uint<N> Imed_in_tmp;	// A copy of Imed_in for bit-level operations
    sc_uint<N> R_tmp;       // A copy of R for bit-level operations
    sc_uint<N> P_tmp;       // A copy of P for bit-level operations
    sc_uint<N> Imed_out_tmp;// A temp. var. used to calculate Imed_out

    Imed_in_tmp = Imed_in.read();
    R_tmp       = R.read();
    P_tmp       = P.read();

    for(unsigned short int i=0; i<N;i++)
        Imed_out_tmp[i] = R_tmp[i] or (Imed_in_tmp[i] and (not P_tmp[i]) );

    Imed_out.write(Imed_out_tmp);
}

////////////////////////////////////////////////////////////////////////////////
void ppe::p_grant_reg()
////////////////////////////////////////////////////////////////////////////////
// Implements the register that stores the grant for a selected request
{
    sc_uint<N> Grant_tmp;       // A copy of Grant_tmp for bit-level operations
    sc_uint<N> R_tmp;           // A copy of R for bit-level operations
    sc_uint<N> Grant_reg_tmp;   // A temp. var. used to calculate Grant_reg

    Grant_tmp     = Grant.read();
    Grant_reg_tmp = Grant_reg.read();
    R_tmp         = R.read();

    for (unsigned short int i=0; i<N; i++) {
        if (rst) {
            Grant_reg_tmp[i] = 0;
        } else { // A register bit can be updated when the arbiter is idle
            if (s_idle.read()) {
                Grant_reg_tmp[i] = Grant_tmp[i];
            } else { // or when a req. is low, specially when a granted request is reset
                if (!R_tmp[i]) {
                    Grant_reg_tmp[i] = 0;
                }
            }
        }
    }

    Grant_reg.write(Grant_reg_tmp);
}


////////////////////////////////////////////////////////////////////////////////
void ppe::p_idle()
////////////////////////////////////////////////////////////////////////////////
// It signals if the channel scheduled by the arb. is idle (there is no grant)
{
    sc_uint<N> Grant_reg_tmp;   // A copy of Grant_reg for bit-level operations
    bool  tmp;                  // A temp. var. used to calculate s_idle

    Grant_reg_tmp = Grant_reg.read();

    tmp = 0;
    for (unsigned short int i=0; i<N; i++)
        tmp = tmp or Grant_reg_tmp[i];

    s_idle.write(not tmp);
}


////////////////////////////////////////////////////////////////////////////////
void ppe::p_outputs()	
////////////////////////////////////////////////////////////////////////////////
// It updates the output
{
    idle.write(s_idle.read());
    G.write(Grant_reg.read());
}

////////////////////////////////////////////////////////////////////////////////
void ppe::p_debug()
////////////////////////////////////////////////////////////////////////////////
// 
{
    sc_uint<N> tmp;
    short int i;
    static long int cycle = 0;
    char port[5] = {'L','N','E','S','W'};

    printf("\n[ppe       (%d,%d)-%c] @ cycle %ld",XID, YID,port[PORT_ID],cycle++);

    cout << "  P=";
    tmp = P.read();
    for(i=N-1; i>=0; i--) cout << tmp[i];

    cout << "  R=";
    tmp = R.read();
    for(i=N-1; i>=0; i--) cout << tmp[i];

    cout << "  G=";
    tmp = G.read();
    for(i=N-1; i>=0; i--) cout << tmp[i];

    cout << "  Imed_in=";
    tmp = Imed_in.read();
    for(i=N-1; i>=0; i--) cout << tmp[i];

    cout << "  Imed_out=";
    tmp = Imed_out.read();
    for(i=N-1; i>=0; i--) cout << tmp[i];

    cout << "  Grant=";
    tmp = Grant.read();
    for(i=N-1; i>=0; i--) cout << tmp[i];

    cout << "  Grant_reg=";
    tmp = Grant_reg.read();
    for(i=N-1; i>=0; i--) cout << tmp[i];

    cout << "  idle=" << idle.read();

    cout << "\t";
}
