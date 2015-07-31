////////////////////////////////////////////////////////////////////////////////
// 
// name         tm_2vc.cpp
//
////////////////////////////////////////////////////////////////////////////////
// Packet format

#include "tm_2vc.h"

#define REQUIRED_BW_POSITION    22
#define TRAFFIC_CLASS_POSITION  18
#define FLOW_ID_POSITION        DATA_WIDTH-2
#define MIN_PAYLOAD_LENGTH      3

//#define CALC_PERFORMANCE_PARAMETERS

////////////////////////////////////////////////////////////////////////////////
void tm_2vc::f_write_received_flit_vc0(FILE *fp_out)
////////////////////////////////////////////////////////////////////////////////
{
    sc_uint<FLIT_WIDTH>	 data_tmp;  // Used to extract fields from din
    bool                 bop;       // packet framing bit: begin of packet
    bool                 eop;       // packet framing bit: end of packet
//    unsigned int         nb_of_cycles_per_flit;

//    switch (FC_TYPE) {
//        case 0  : nb_of_cycles_per_flit = 4; break;
//        case 1  : nb_of_cycles_per_flit = 1; break;
//        case 2  : nb_of_cycles_per_flit = 1; break;
//        default : break;
//    }

    // It extracts the the framing bits
    data_tmp = data.read();
    bop      = data_tmp[FLIT_WIDTH-2];
    eop      = data_tmp[FLIT_WIDTH-1];

    // It counts the number of flits received, including the header
    vc0_pck_size++;

    if (bop == 1) {
        // It gets the header and extracts the address fields
        vc0_header            = (unsigned long long) (data.read() & (unsigned long long)(pow(2,FLIT_WIDTH-2)-1));
        vc0_x_src             = (unsigned int) ((vc0_header & X_SRC_MASK ) >> RIB_WIDTH) >> RIB_WIDTH/2;
        vc0_y_src             = (unsigned int) ((vc0_header & Y_SRC_MASK ) >> RIB_WIDTH);
        vc0_x_dest            = (unsigned int) ((vc0_header & X_DEST_MASK) >> RIB_WIDTH/2);
        vc0_y_dest            = (unsigned int) ((vc0_header & Y_DEST_MASK));
        vc0_traffic_class     = (unsigned int) ((vc0_header >> TRAFFIC_CLASS_POSITION)) & 0x3;
        vc0_flow_id  		  = (unsigned int) (((vc0_header >> ((unsigned int)FLOW_ID_POSITION))) & 0x3);
        vc0_cycle_of_arriving = (unsigned long long) clock_cycles.read();
    }

    if (vc0_pck_size == 2) {
        vc0_required_bw = (unsigned int) (data.read() & (unsigned long long)(pow(2,FLIT_WIDTH-2)-1));
    }

    if (vc0_pck_size == 3) {
        vc0_deadline	= (unsigned long long) (data.read() & (unsigned long long)(pow(2,FLIT_WIDTH-2)-1));
    }

    if (vc0_pck_size == 4) {
        vc0_cycle_to_send	= (unsigned long long) (data.read() & (unsigned long long)(pow(2,FLIT_WIDTH-2)-1)) << DATA_WIDTH;
    }

    // Increments the packet counter after a trailer has been received
    if (vc0_pck_size == 5) {
        vc0_cycle_to_send = vc0_cycle_to_send | (unsigned long long) (data.read() & (unsigned long long)(pow(2,FLIT_WIDTH-2)-1));
    }

    if (eop==1) {
        vc0_pck_received++;
        pck_received++;
        fprintf(fp_out,"%10u\t"  , pck_received);
        //	scanf(fp_in,"%u",&pck_received)
//        fprintf(fp_out,"%2u\t"   , vc0_x_dest);
//        fprintf(fp_out,"%2u\t"   , vc0_y_dest);
        fprintf(fp_out,"%2u\t"   , vc0_x_src);
        fprintf(fp_out,"%2u\t"   , vc0_y_src);
        fprintf(fp_out,"%2u\t"   , vc0_flow_id);
        fprintf(fp_out,"%2u\t"   , vc0_traffic_class);
        fprintf(fp_out,"%10llu\t", vc0_deadline);
        fprintf(fp_out,"%10llu\t", vc0_cycle_to_send);
        fprintf(fp_out,"%10llu\t", vc0_cycle_of_arriving);
        fprintf(fp_out,"%10llu\t", (unsigned long long) clock_cycles.read());
        fprintf(fp_out,"%7u\t"   , vc0_pck_size-1);  // IT DOES NOT TAKE INTO ACCOUNT THE HEADER
        //    fprintf(fp_out,"  %.2f\t" ,(round(((float) required_bw) * 100.0)/100.0));
        //    fprintf(fp_out,"  %.2f\t" ,(round(((float) required_bw) * 100.0)/10000.0));
        fprintf(fp_out,"  %.0f\t" ,round(((float) vc0_required_bw)));
        fprintf(fp_out,"\n");
        vc0_pck_size = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
void tm_2vc::f_write_received_flit_vc1(FILE *fp_out)
////////////////////////////////////////////////////////////////////////////////
{
    sc_uint<FLIT_WIDTH>	 data_tmp;   // Used to extract fields from din
    bool                 bop;	     // packet framing bit: begin of packet
    bool                 eop;        // packet framing bit: end of packet
//    unsigned int         nb_of_cycles_per_flit;

//    switch (FC_TYPE) {
//        case 0  : nb_of_cycles_per_flit = 4; break;
//        case 1  : nb_of_cycles_per_flit = 1; break;
//        case 2  : nb_of_cycles_per_flit = 1; break;
//        default : break;
//    }

    // It extracts the the framing bits
    data_tmp = data.read();
    bop      = data_tmp[FLIT_WIDTH-2];
    eop      = data_tmp[FLIT_WIDTH-1];

    // It counts the number of flits received, including the header
    vc1_pck_size++;

    if (bop == 1) {
        // It gets the header and extracts the address fields
        vc1_header            = (unsigned long long) (data.read() & (unsigned long long)(pow(2,FLIT_WIDTH-2)-1));
        vc1_x_src             = (unsigned int) ((vc1_header & X_SRC_MASK ) >> RIB_WIDTH) >> RIB_WIDTH/2;
        vc1_y_src             = (unsigned int) ((vc1_header & Y_SRC_MASK ) >> RIB_WIDTH);
        vc1_x_dest            = (unsigned int) ((vc1_header & X_DEST_MASK) >> RIB_WIDTH/2);
        vc1_y_dest            = (unsigned int) ((vc1_header & Y_DEST_MASK));
        vc1_traffic_class     = (unsigned int) ((vc1_header >> TRAFFIC_CLASS_POSITION)) & 0x3;
        vc1_flow_id           = (unsigned int) (((vc1_header >> ((unsigned int)FLOW_ID_POSITION) )) & 0x3);
        vc1_cycle_of_arriving = (unsigned long long) clock_cycles.read();
    }

    if (vc1_pck_size == 2) {
        vc1_required_bw = (unsigned int) (data.read() & (unsigned long long)(pow(2,FLIT_WIDTH-2)-1));
    }

    if (vc1_pck_size == 3) {
        vc1_deadline	= (unsigned long long) (data.read() & (unsigned long long)(pow(2,FLIT_WIDTH-2)-1));
    }

    if (vc1_pck_size == 4) {
        vc1_cycle_to_send	= (unsigned long long) (data.read() & (unsigned long long)(pow(2,FLIT_WIDTH-2)-1)) << DATA_WIDTH;
    }

    // Increments the packet counter after a trailer has been received
    if (vc1_pck_size == 5) {
        vc1_cycle_to_send = vc1_cycle_to_send | (unsigned long long) (data.read() & (unsigned long long)(pow(2,FLIT_WIDTH-2)-1));
    }

    if (eop==1) {
        vc1_pck_received++;
        pck_received++;
        fprintf(fp_out,"%10u\t"  , pck_received);
        //	scanf(fp_in,"%u",&pck_received)
//        fprintf(fp_out,"%2u\t"   , vc1_x_dest);
//        fprintf(fp_out,"%2u\t"   , vc1_y_dest);
        fprintf(fp_out,"%2u\t"   , vc1_x_src);
        fprintf(fp_out,"%2u\t"   , vc1_y_src);
        fprintf(fp_out,"%2u\t"   , vc1_flow_id);
        fprintf(fp_out,"%2u\t"   , vc1_traffic_class);
        fprintf(fp_out,"%10llu\t", vc1_deadline);
        fprintf(fp_out,"%10llu\t", vc1_cycle_to_send);
        fprintf(fp_out,"%10llu\t", vc1_cycle_of_arriving);
        fprintf(fp_out,"%10llu\t", (unsigned long long) clock_cycles.read());
        fprintf(fp_out,"%7u\t"   , vc1_pck_size-1);  // IT DOES NOT TAKE INTO ACCOUNT THE HEADER
        //    fprintf(fp_out,"  %.2f\t" ,(round(((float) required_bw) * 100.0)/100.0));
        //    fprintf(fp_out,"  %.2f\t" ,(round(((float) required_bw) * 100.0)/10000.0));
        fprintf(fp_out,"  %.2f\t" ,round(((float) vc1_required_bw)));
        fprintf(fp_out,"\n");
        vc1_pck_size = 0;
    }
}


////////////////////////////////////////////////////////////////////////////////
void tm_2vc::p_vc0_probe()
////////////////////////////////////////////////////////////////////////////////
{
    unsigned int flit_written = 0;

    /////////////////////////////////
    if (FC_TYPE == 0) {  // Handshake
        /////////////////////////////////
        while (1) {
            if (file_is_open) {
                flit_written = 0;

                while (vc0_val.read()==0) wait();

                while (vc0_ret.read()==0) wait();

                if (flit_written == 0) {
                    f_write_received_flit_vc0(fp_out);
                    flit_written = 1;
                }

                while (vc0_val.read()==1) wait();

                while (vc0_ret.read()==1) wait();
            } else {
                wait();
            }
        }
        ////////////////////////
    } else { // Credit-based
        ////////////////////////
        while (1) {
            if (file_is_open) {
                if (vc0_val.read()) f_write_received_flit_vc0(fp_out);
                wait();
            } else {
                wait();
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void tm_2vc::p_vc1_probe()
////////////////////////////////////////////////////////////////////////////////
{
    unsigned int flit_written = 0;

    /////////////////////////////////
    if (FC_TYPE == 0) {  // Handshake
        /////////////////////////////////
        while (1) {
            if (file_is_open) {
                flit_written = 0;

                while (vc1_val.read()==0) wait();

                while (vc1_ret.read()==0) wait();

                if (flit_written == 0) {
                    f_write_received_flit_vc1(fp_out);
                    flit_written = 1;
                }

                while (vc1_val.read()==1) wait();

                while (vc1_ret.read()==1) wait();
            } else {
                wait();
            }
        }
        ////////////////////////
    } else { // Credit-based
        ////////////////////////
        while (1) {
            if (file_is_open) {
                if (vc1_val.read()) f_write_received_flit_vc1(fp_out);
                wait();
            } else {
                wait();
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void tm_2vc::p_start()
////////////////////////////////////////////////////////////////////////////////
{
    char path_filename[256];
    sprintf(path_filename,"%s/%s",PATH, FILENAME);

    file_is_open     = 0;
    pck_received     = 0;
    vc0_pck_received = 0;
    vc0_pck_size     = 0;
    vc1_pck_received = 0;
    vc1_pck_size     = 0;

    // It creates the output file
    if (file_is_open == 0) {
        if ((fp_out=fopen(path_filename,"wt")) == NULL) {
            printf("\n\t[tm.cpp] ERROR: Impossible to open file \"%s\". Exiting...", path_filename);
            exit(1);
        }
        file_is_open = 1;
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
    }

    while (1) {
        wait();
    }
}

////////////////////////////////////////////////////////////////////////////////
void tm_2vc::p_stop()
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
