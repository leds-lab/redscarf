
#include "stopsim.h"

////////////////////////////////////////////////////////////////////////////////
void stopsim::p_stop()
////////////////////////////////////////////////////////////////////////////////
{
    unsigned long int stop_time_ns;
    unsigned long int stop_time_cycles;
    char str[2048];
    FILE *fp_in;
    FILE *fp_out;

    // It opens the input file and reads the stop time
    sprintf(str,"../%s.par",FILENAME); 
    if ((fp_in=fopen(str,"r")) == NULL) {
        printf("\n	[stopsim.cpp] ERROR: Impossible to open file \"%s\". Exiting...", str);
        exit(1);
    }
    fscanf(fp_in,"%s",str);
    stop_time_cycles = atol(str);
    fscanf(fp_in,"%s",str);
    stop_time_ns     = atol(str);
    fclose(fp_in);


    // It opens the output file
    sprintf(str,"%s.out",FILENAME); 
    if ((fp_out=fopen(str,"wt")) == NULL) {
        printf("\n	[stopsim.cpp] ERROR: Impossible to open file \"%s\". Exiting...", str);
        exit(1);
    }

    // It resets the counters
    total_packets_sent.write(0);
    total_packets_received.write(0);
    eos.write(0);
    wait();

    while (1) {
        total_packets_sent.write( TG_0_0_number_of_packets_sent.read()
                                + TG_0_1_number_of_packets_sent.read()
                                + TG_1_0_number_of_packets_sent.read()
                                + TG_1_1_number_of_packets_sent.read() );

        total_packets_received.write( TG_0_0_number_of_packets_received.read()
                                    + TG_0_1_number_of_packets_received.read()
                                    + TG_1_0_number_of_packets_received.read()
                                    + TG_1_1_number_of_packets_received.read() );

        eot.write( TG_0_0_eot.read()
                 & TG_0_1_eot.read()
                 & TG_1_0_eot.read()
                 & TG_1_1_eot.read() );

#ifdef DEBUG_STOPSIM
        cout << "\n";
        cout << "[stopsim] cycle = " << clock_cycles.read();
        cout << "		packets sent = " << total_packets_sent.read();
        cout << "  (" << TG_0_0_number_of_packets_sent.read();
        cout << " + " << TG_0_1_number_of_packets_sent.read();
        cout << " + " << TG_1_0_number_of_packets_sent.read();
        cout << " + " << TG_1_1_number_of_packets_sent.read();
        cout << ")";

        cout << "		packets received = " << total_packets_received.read();
        cout << "  (" << TG_0_0_number_of_packets_received.read();
        cout << " + " << TG_0_1_number_of_packets_received.read();
        cout << " + " << TG_1_0_number_of_packets_received.read();
        cout << " + " << TG_1_1_number_of_packets_received.read();
        cout << ")";
#endif // DEBUG_STOPSIM

        wait();
        if (stop_time_cycles == 0) {
            if (eot.read() == 1) {
                if (total_packets_received.read()==total_packets_sent.read()) {
                    fprintf(fp_out,"%llu", (unsigned long long) clock_cycles.read());
                    fclose(fp_out);
                    eos.write(1);
                    wait();
                    sc_stop();
                }
            }
        } else {
            if (clock_cycles.read() == stop_time_cycles) {
                fprintf(fp_out,"%llu", (unsigned long long) clock_cycles.read());
                fclose(fp_out);
                eos.write(1);
                wait();
                sc_stop();
            }
        }
//    wait();
    }
}
