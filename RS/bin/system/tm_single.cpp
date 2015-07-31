////////////////////////////////////////////////////////////////////////////////
// 
// name         tm_single.cpp
//
////////////////////////////////////////////////////////////////////////////////
// Packet format

#include "tm_single.h"

#define REQUIRED_BW_POSITION    22
#define TRAFFIC_CLASS_POSITION	18
#define FLOW_ID_POSITION        DATA_WIDTH-2
#define MIN_PAYLOAD_LENGTH      3

//#define CALC_PERFORMANCE_PARAMETERS

////////////////////////////////////////////////////////////////////////////////
void tm_single::f_write_received_flit(FILE *fp_out)
////////////////////////////////////////////////////////////////////////////////
{
//    float               offered_load;
//    float               accepted_traffic;
    sc_uint<FLIT_WIDTH>	data_tmp;   // Used to extract fields from din
    bool                bop;        // packet framing bit: begin of packet
    bool                eop;        // packet framing bit: end of packet


    switch (FC_TYPE) {
    case 0  : nb_of_cycles_per_flit = 4; break;
    case 1  : nb_of_cycles_per_flit = 1; break;
    case 2  : nb_of_cycles_per_flit = 1; break;
    default : break;
    }

    // It extracts the the framing bits
    data_tmp = data.read();
    bop      = data_tmp[FLIT_WIDTH-2];
    eop      = data_tmp[FLIT_WIDTH-1];

    // It counts the number of flits received, including the header
    pck_size++;

    if (bop == 1) {
        // It gets the header and extracts the address fields
        header 			  = (unsigned long long) (data.read() & (unsigned long long)(pow(2,FLIT_WIDTH-2)-1));
        x_src  			  = (unsigned int) ((header & X_SRC_MASK ) >> RIB_WIDTH) >> RIB_WIDTH/2;
        y_src             = (unsigned int) ((header & Y_SRC_MASK ) >> RIB_WIDTH);
        x_dest 			  = (unsigned int) ((header & X_DEST_MASK) >> RIB_WIDTH/2);
        y_dest            = (unsigned int) ((header & Y_DEST_MASK));
        traffic_class 	  = (unsigned int) ((header >> TRAFFIC_CLASS_POSITION)) & 0x3;
        flow_id           = (unsigned int) ((header >> ((unsigned int)FLOW_ID_POSITION) ) & 0x3);
        cycle_of_arriving = (unsigned long long) clock_cycles.read();
    }

    if (pck_size == 2) {
        required_bw = (unsigned int) (data.read() & (unsigned long long)(pow(2,FLIT_WIDTH-2)-1));
    }

    if (pck_size == 3) {
        deadline	= (unsigned long long) (data.read() & (unsigned long long)(pow(2,FLIT_WIDTH-2)-1));
    }

    if (pck_size == 4) {
        cycle_to_send	= (unsigned long long) (data.read() & (unsigned long long)(pow(2,FLIT_WIDTH-2)-1)) << DATA_WIDTH;
    }

    // Increments the packet counter after a trailer has been received
    if (pck_size == 5) {
        cycle_to_send = cycle_to_send | (unsigned long long) (data.read() & (unsigned long long)(pow(2,FLIT_WIDTH-2)-1));
    }

    if (eop==1) {
        pck_received++;
        fprintf(fp_out,"%10u\t"  , pck_received);
        //	scanf(fp_in,"%u",&pck_received)
//        fprintf(fp_out,"%2u\t"   , x_dest);
//        fprintf(fp_out,"%2u\t"   , y_dest);
        fprintf(fp_out,"%2u\t"   , x_src);
        fprintf(fp_out,"%2u\t"   , y_src);
        fprintf(fp_out,"%2u\t"   , flow_id);
        fprintf(fp_out,"%2u\t"   , traffic_class);
        fprintf(fp_out,"%10llu\t" , deadline);
        fprintf(fp_out,"%10llu\t" , cycle_to_send);
        fprintf(fp_out,"%10llu\t" , cycle_of_arriving);
        fprintf(fp_out,"%10llu\t" , (unsigned long long) clock_cycles.read());
        fprintf(fp_out,"%7u\t"   , pck_size-1);  // IT DOES NOT TAKE INTO ACCOUNT THE HEADER
        //    fprintf(fp_out,"  %.2f\t" ,(round(((float) required_bw) * 100.0)/100.0));
        //    fprintf(fp_out,"  %.2f\t" ,(round(((float) required_bw) * 100.0)/10000.0));
        fprintf(fp_out,"  %.0f\t" ,round(((float) required_bw)));
        fprintf(fp_out,"\n");
        pck_size = 0;
    }
}



////////////////////////////////////////////////////////////////////////////////
void tm_single::p_probe()
////////////////////////////////////////////////////////////////////////////////
{
    char path_filename[256];
    unsigned int flit_written = 0;
    sprintf(path_filename,"%s/%s",PATH, FILENAME);

    // It creates the output file
    if ((fp_out=fopen(path_filename,"wt")) == NULL) {
        printf("\n\t[tm.cpp] ERROR: Impossible to open file \"%s\". Exiting...", path_filename);
        exit(1);
    }

    // It prints the header of the table
    fprintf(fp_out,"FILE: %s",path_filename);
#ifdef CALC_PERFORMANCE_PARAMETERS   
    fprintf(fp_out,"  -  Note: Pck Latency = Trailer at cycle  -  Pck Creation + 1");
#endif
    fprintf(fp_out,"\n");

    fprintf(fp_out,"\n    Packet\tXs\tYs\tFID\tClass\tDeadline\t     Packet\t    Header\t   Trailer\t Packet\t    Req");
#ifdef CALC_PERFORMANCE_PARAMETERS            
    fprintf(fp_out,"\t    Packet\t  Delay of\t  Off.\tAccep.");
#endif

    fprintf(fp_out,"\n    Number\t  \t  \t  \t  \t  \t  \t  \t  \t   Creation\t  at cycle\t  at cycle\t Length\t     BW");
#ifdef CALC_PERFORMANCE_PARAMETERS            
    fprintf(fp_out,"\t   Latency\t Injection\t  Load\tTraff.");
#endif

    fprintf(fp_out,"\n#\n");

    pck_received = 0;
    pck_size = 0;
    wait();

    /////////////////////////////////
    if (FC_TYPE == 0) {  // Handshake
        /////////////////////////////////
        while (1) {
            flit_written = 0;

            while (val.read()==0) wait();

            while (ret.read()==0) wait();

            if (flit_written == 0) {
                f_write_received_flit(fp_out);
                flit_written = 1;
            }

            while (val.read()==1) wait();

            while (ret.read()==1) wait();
        }

        ////////////////////////
    } else { // Credit-based
        ////////////////////////
        while (1) {
            if (val.read())
                f_write_received_flit(fp_out);
            wait();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void tm_single::p_stop()
////////////////////////////////////////////////////////////////////////////////
{
    while (1) {
        if (eos.read()==1) {
            fprintf(fp_out,"\n#");
            fprintf(fp_out," %lu", (unsigned long int) clock_cycles.read());
            fclose(fp_out);
        }
        wait();
    }
}
