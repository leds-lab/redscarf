/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* GSoC.cpp
* Copyright (C) 2014 LEDS - Univali <zeferino@univali.br>
* Laboratory of Embedded and Distributed Systems
* University of Vale do Itajaí
*
* Authors: Laboratory of Embedded and Distributed Systems (LEDS)
*
* Contact: Profº Dr. Cesar Albenes Zeferino {zeferino@univali.br}
*
* RedScarf is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* RedScarf is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* ------------------------------ Reviews -------------------------------------
* Date       - Version - Author                      | Description
* ----------------------------------------------------------------------------
* 10/12/2014 - 1.0     - Eduardo Alves da Silva      | Initial release
*
*/

#include "include/Model/System/GSoC.h"
#include "include/Model/FolderOperation.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

GSoC::GSoC(SystemParameters* sp, Experiment *experiment){
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Model/SystemConfiguration/GSoC" << std::endl;
#endif

    this->systemParameters = sp;
    this->experimento = experiment;
}

bool GSoC::generateHeaderStopSim() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/GSoC::generateHeaderStopSim" << std::endl;
#endif

    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    //                                                                          //
    //                     CREATION OF THE FILE "stopsim.h"                     //
    //                                                                          //
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    sprintf(this->fileName,"system/stopsim.h");
    if ((this->out=fopen(this->fileName,"wt")) == NULL){
        printf("\n\tERROR: Cannot open the file \"%s\".\n\n",this->fileName);
        return false;
    }

    unsigned int x,y;
    unsigned int x_size = this->systemParameters->getXSize();
    unsigned int y_size = this->systemParameters->getYSize();

    fprintf(this->out,"\n////////////////////////////////////////////////////////////////////////////////");
    fprintf(this->out,"\n// ");
    fprintf(this->out,"\n// name         stopsim.h");
    fprintf(this->out,"\n//");
    fprintf(this->out,"\n////////////////////////////////////////////////////////////////////////////////");

    fprintf(this->out,"\n\n#ifndef __STOPSIM_H__");
    fprintf(this->out,"\n#define __STOPSIM_H__");

    fprintf(this->out,"\n\n#include <systemc.h>");
    fprintf(this->out,"\n#include \"parameters.h\"");

    fprintf(this->out,"\n\nSC_MODULE(stopsim)");
    fprintf(this->out,"\n{");
    fprintf(this->out,"\n    // INTERFACE");
    fprintf(this->out,"\n    // System signals");
    fprintf(this->out,"\n    sc_in_clk           clk;");
    fprintf(this->out,"\n    sc_in<bool>         rst;");
    fprintf(this->out,"\n    sc_out<bool>        eos;");
    fprintf(this->out,"\n    sc_in<sc_uint<64> > clock_cycles;");

    fprintf(this->out,"\n");
    for(x=0;x<x_size;x++)
        for(y=0;y<y_size;y++)
            fprintf(this->out,"\n    sc_in<bool>         TG_%u_%u_eot; ",x,y);

    fprintf(this->out,"\n");
    for(x=0;x<x_size;x++)
        for(y=0;y<y_size;y++)
            fprintf(this->out,"\n    sc_in<sc_uint<32> > TG_%u_%u_number_of_packets_sent; ",x,y);

    fprintf(this->out,"\n");
    for(x=0;x<x_size;x++)
        for(y=0;y<y_size;y++)
            fprintf(this->out,"\n    sc_in<sc_uint<32> > TG_%u_%u_number_of_packets_received; ",x,y);

    fprintf(this->out,"\n\n    // Signals");
    fprintf(this->out,"\n    sc_signal<sc_uint<32> > total_packets_received;");
    fprintf(this->out,"\n    sc_signal<sc_uint<32> > total_packets_sent;");
    fprintf(this->out,"\n    sc_signal<bool>         eot;");

    fprintf(this->out,"\n\n    // Internal data structures");
    fprintf(this->out,"\n    char *PATH;");
    fprintf(this->out,"\n    char *FILENAME;");

    fprintf(this->out,"\n\n    // Module's process");
    fprintf(this->out,"\n    void p_stop();");

    fprintf(this->out,"\n\n    SC_HAS_PROCESS(stopsim);");

    fprintf(this->out,"\n\n    //////////////////////////////////////////////////////////////////////////////");
    fprintf(this->out,"\n    stopsim(sc_module_name nm,");
    fprintf(this->out,"\n            char *PATH,");
    fprintf(this->out,"\n            char *FILENAME) :");
    fprintf(this->out,"\n            sc_module(nm),");
    fprintf(this->out,"\n            PATH(PATH),");
    fprintf(this->out,"\n            FILENAME(FILENAME)");
    fprintf(this->out,"\n    //////////////////////////////////////////////////////////////////////////////");

    fprintf(this->out,"\n    { ");
    fprintf(this->out,"\n        SC_CTHREAD(p_stop, clk.pos());");
    fprintf(this->out,"\n        sensitive << clk.pos() << rst;");
    fprintf(this->out,"\n    }");

    fprintf(this->out,"\n\n};");
    fprintf(this->out,"\n#endif // __STOPSIM_H__");
    fprintf(this->out,"\n");

    fclose(this->out);
    return true;
}

bool GSoC::generateStopSim() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/GSoC::generateStopSim" << std::endl;
#endif

    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    //                                                                          //
    //                     CREATION OF THE FILE "stopsim.cpp"                   //
    //                                                                          //
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    sprintf(this->fileName,"system/stopsim.cpp");

    if ((this->out=fopen(this->fileName,"wt")) == NULL){
        printf("\n\tERROR: Cannot open the file \"%s\".\n\n",this->fileName);
        return false;
    }

    unsigned int x,y;
    unsigned int x_size = this->systemParameters->getXSize();
    unsigned int y_size = this->systemParameters->getYSize();


    fprintf(this->out,"\n#include \"stopsim.h\"");

    fprintf(this->out,"\n\n////////////////////////////////////////////////////////////////////////////////");
    fprintf(this->out,"\nvoid stopsim::p_stop()");
    fprintf(this->out,"\n////////////////////////////////////////////////////////////////////////////////");
    fprintf(this->out,"\n{");

    fprintf(this->out,"\n    unsigned long int stop_time_ns;");
    fprintf(this->out,"\n    unsigned long int stop_time_cycles;");
    fprintf(this->out,"\n    char str[2048];");
    fprintf(this->out,"\n    FILE *fp_in;");
    fprintf(this->out,"\n    FILE *fp_out;");

    fprintf(this->out,"\n\n    // It opens the input file and reads the stop time");
    fprintf(this->out,"\n    sprintf(str,\"../\%%s.par\",FILENAME); ");
    fprintf(this->out,"\n    if ((fp_in=fopen(str,\"r\")) == NULL) {");
    fprintf(this->out,"\n        printf(\"\\n\t[stopsim.cpp] ERROR: Impossible to open file \\\"%%s\\\". Exiting...\", str);");
    fprintf(this->out,"\n        exit(1);");
    fprintf(this->out,"\n    }");
    fprintf(this->out,"\n    fscanf(fp_in,\"%%s\",str);");
    fprintf(this->out,"\n    stop_time_cycles = atol(str);");
    fprintf(this->out,"\n    fscanf(fp_in,\"%%s\",str);");
    fprintf(this->out,"\n    stop_time_ns     = atol(str);");
    fprintf(this->out,"\n    fclose(fp_in);");
    fprintf(this->out,"\n");

    fprintf(this->out,"\n\n    // It opens the output file");
    fprintf(this->out,"\n    sprintf(str,\"\%%s.out\",FILENAME); ");
    fprintf(this->out,"\n    if ((fp_out=fopen(str,\"wt\")) == NULL) {");
    fprintf(this->out,"\n        printf(\"\\n\t[stopsim.cpp] ERROR: Impossible to open file \\\"%%s\\\". Exiting...\", str);");
    fprintf(this->out,"\n        exit(1);");
    fprintf(this->out,"\n    }");


    fprintf(this->out,"\n\n    // It resets the counters");
    fprintf(this->out,"\n    total_packets_sent.write(0);");
    fprintf(this->out,"\n    total_packets_received.write(0);");
    fprintf(this->out,"\n    eos.write(0);");
    fprintf(this->out,"\n    wait();");
    fprintf(this->out,"\n\n    while (1) {");
    fprintf(this->out,"\n        total_packets_sent.write(");
    for(x=0;x<x_size;x++)
        for(y=0;y<y_size;y++)
            if ((x==0) && (y==0))
                fprintf(this->out," TG_0_0_number_of_packets_sent.read()");
            else
                fprintf(this->out,"\n                                + TG_%u_%u_number_of_packets_sent.read()",x,y);

    fprintf(this->out," );");

    fprintf(this->out,"\n\n        total_packets_received.write(");
    for(x=0;x<x_size;x++)
        for(y=0;y<y_size;y++)
            if ((x==0) && (y==0))
                fprintf(this->out," TG_0_0_number_of_packets_received.read()");
            else
                fprintf(this->out,"\n                                    + TG_%u_%u_number_of_packets_received.read()",x,y);

    fprintf(this->out," );");

    fprintf(this->out,"\n\n        eot.write(");
    for(x=0;x<x_size;x++)
        for(y=0;y<y_size;y++)
            if ((x==0) && (y==0))
              fprintf(this->out," TG_0_0_eot.read()");
            else
              fprintf(this->out,"\n                 & TG_%u_%u_eot.read()",x,y);

    fprintf(this->out," );");


    fprintf(this->out,"\n\n#ifdef DEBUG_STOPSIM");
    fprintf(this->out,"\n        cout << \"\\n\";");

    fprintf(this->out,"\n        cout << \"[stopsim] cycle = \" << clock_cycles.read();");
//    fprintf(this->out,"\n        printf( \"[stopsim] cycle = \%%08lu\", (unsigned long int) clock_cycles.read());");
    fprintf(this->out,"\n        cout << \"\t\tpackets sent = \" << total_packets_sent.read();");

    for(x=0;x<x_size;x++)
        for(y=0;y<y_size;y++)
            if ((x==0) && (y==0))
                fprintf(this->out,"\n        cout << \"  (\" << TG_0_0_number_of_packets_sent.read();");
            else
                fprintf(this->out,"\n        cout << \" + \" << TG_%u_%u_number_of_packets_sent.read();",x,y);

    fprintf(this->out,"\n        cout << \")\";");

    fprintf(this->out,"\n\n        cout << \"\t\tpackets received = \" << total_packets_received.read();");
    for(x=0;x<x_size;x++)
        for(y=0;y<y_size;y++)
            if ((x==0) && (y==0))
                fprintf(this->out,"\n        cout << \"  (\" << TG_0_0_number_of_packets_received.read();");
            else
                fprintf(this->out,"\n        cout << \" + \" << TG_%u_%u_number_of_packets_received.read();",x,y);

    fprintf(this->out,"\n        cout << \")\";");

    fprintf(this->out,"\n#endif // DEBUG_STOPSIM");

    fprintf(this->out,"\n\n        wait();");


    fprintf(this->out,"\n        if (stop_time_cycles == 0) {");
    fprintf(this->out,"\n            if (eot.read() == 1) {");
    fprintf(this->out,"\n                if (total_packets_received.read()==total_packets_sent.read()) {");
    fprintf(this->out,"\n                    fprintf(fp_out,\"%%llu\", (unsigned long long) clock_cycles.read());");
    fprintf(this->out,"\n                    fclose(fp_out);");
    fprintf(this->out,"\n                    eos.write(1);");
    fprintf(this->out,"\n                    wait();");
    fprintf(this->out,"\n                    sc_stop();");
    fprintf(this->out,"\n                }");
    fprintf(this->out,"\n            }");
    fprintf(this->out,"\n        } else {");
    fprintf(this->out,"\n            if (clock_cycles.read() == stop_time_cycles) {");
    fprintf(this->out,"\n                fprintf(fp_out,\"%%llu\", (unsigned long long) clock_cycles.read());");
    fprintf(this->out,"\n                fclose(fp_out);");
    fprintf(this->out,"\n                eos.write(1);");
    fprintf(this->out,"\n                wait();");// for(int i=0; i<10000000;i++)
    fprintf(this->out,"\n                sc_stop();");
    fprintf(this->out,"\n            }");
    fprintf(this->out,"\n        }");

//    fprintf(this->out,"\n      printf(\"stop_time_cycles: \%%lu \",stop_time_cycles );");

    fprintf(this->out,"\n//    wait();");
    fprintf(this->out,"\n    }");
    fprintf(this->out,"\n}");
    fprintf(this->out,"\n");

    fclose(this->out);

    return true;
}

bool GSoC::generateMain() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/GSoC::generateMain" << std::endl;
#endif

    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    //                                                                          //
    //                     CREATION OF THE FILE "main.cpp"                      //
    //                                                                          //
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    sprintf(this->fileName,"system/main.cpp");

    if ((this->out=fopen(this->fileName,"wt")) == NULL){
        printf("\n\tERROR: Cannot open the file \"%s\".\n\n",this->fileName);
        return false;
    }

    unsigned int x_size = this->systemParameters->getXSize();
    unsigned int y_size = this->systemParameters->getYSize();
    unsigned int router_type = this->experimento->getRouterArchitecture();
    unsigned int vcd_option = this->systemParameters->getVcdOption();
    unsigned int x,y;

    //////////////////////////////////////////////////////////////////////////////
    // It prints a header
    //////////////////////////////////////////////////////////////////////////////
    fprintf(this->out,"\n/*");
    fprintf(this->out,"\n--------------------------------------------------------------------------------");
    fprintf(this->out,"\nPROJECT: SoCIN");
    fprintf(this->out,"\nFILE   : main.cpp");
    fprintf(this->out,"\n--------------------------------------------------------------------------------");
    fprintf(this->out,"\nDESCRIPTION: A SoC with %u traffic generators attached to a %ux%u SoCIN NoC",x_size*y_size, x_size, y_size);
    fprintf(this->out,"\n--------------------------------------------------------------------------------");
    fprintf(this->out,"\nAUTHORS: Laboratory of Embedded and Distribution System (LSED - UNIVALI)");
    fprintf(this->out,"\nCONTACT: Prof. Cesar Zeferino (zeferino@univali.br)");
    fprintf(this->out,"\n--------------------------------------------------------------------------------");
    fprintf(this->out,"\n*/");

    //////////////////////////////////////////////////////////////////////////////
    // It prints the defines and includes to the file
    //////////////////////////////////////////////////////////////////////////////
    fprintf(this->out,"\n\n#include <systemc.h>");
    fprintf(this->out,"\n#include \"parameters.h\"");
    fprintf(this->out,"\n#include \"reset.h\"");
    fprintf(this->out,"\n#include \"global_clock.h\"");
    fprintf(this->out,"\n#include \"stopsim.h\"");

    switch (router_type) {
        case 0 : // PARIS
            fprintf(this->out,"\n#include \"tg.h\"");
            fprintf(this->out,"\n#include \"tm_single.h\"");
            fprintf(this->out,"\n#include \"socin.h\"");
            break;

        case 1: // PARIS2
        case 2: //PARIS2_AG:
        case 3: // PARIS2_AG_AG:
            fprintf(this->out,"\n#include \"tg_2vc.h\"");
            fprintf(this->out,"\n#include \"tm_2vc.h\"");
            fprintf(this->out,"\n#include \"socin_2vc.h\"");
            break;

        default : break;
    }

    fprintf(this->out,"\n\n#define USE_LOCAL  1");
    fprintf(this->out,"\n#define USE_NORTH  1");
    fprintf(this->out,"\n#define USE_EAST   1");
    fprintf(this->out,"\n#define USE_SOUTH  1");
    fprintf(this->out,"\n#define USE_WEST   1");
    fprintf(this->out,"\n#define XID        1");
    fprintf(this->out,"\n#define YID        1");

    fprintf(this->out,"\n\n\nint sc_main(int argc, char* argv[])");
    fprintf(this->out,"\n{");

    // Adição realizada 21/05/2014 - correção Zeferino
    fprintf(this->out,"\n\n    // Obtaining the operation clock frequency");
    fprintf(this->out,"\n    float tclk;");
    fprintf(this->out,"\n    if (argc >= 2)");
    fprintf(this->out,"\n        tclk = atof(argv[2]);");
    fprintf(this->out,"\n    else");
    fprintf(this->out,"\n        tclk = 1;");
    // Fim adição

    fprintf(this->out,"\n    // System signals");
    fprintf(this->out,"\n    sc_clock                clk_wire(\"clk\", tclk, SC_NS);  // System clock - Tclk=1 ns");
    fprintf(this->out,"\n    sc_signal<bool>         rst_wire;                      // Reset");
    fprintf(this->out,"\n    sc_signal<sc_uint<64> > clock_cycles_wire;             // Number of cycles");


    switch (router_type) {
        case 0 :// PARIS
            for(x=0;x<x_size;x++) {
                for(y=0;y<y_size;y++) {
                    fprintf(this->out,"\n\n    // NoC terminal (%u,%u)",x,y);
                    fprintf(this->out,"\n    sc_signal<sc_uint<FLIT_WIDTH> > L_%u_%u_in_data_wire; ",x,y);
                    fprintf(this->out,"\n    sc_signal<bool>                 L_%u_%u_in_val_wire;  ",x,y);
                    fprintf(this->out,"\n    sc_signal<bool>                 L_%u_%u_in_ret_wire;  ",x,y);
                    fprintf(this->out,"\n    sc_signal<sc_uint<FLIT_WIDTH> > L_%u_%u_out_data_wire;",x,y);
                    fprintf(this->out,"\n    sc_signal<bool>                 L_%u_%u_out_val_wire; ",x,y);
                    fprintf(this->out,"\n    sc_signal<bool>                 L_%u_%u_out_ret_wire; ",x,y);
                }
            }
            break;

        case 1: // PARIS2       :
        case 2: // PARIS2_AG    :
        case 3: // PARIS2_AG_AG :
            for(x=0;x<x_size;x++) {
                for(y=0;y<y_size;y++) {
                    fprintf(this->out,"\n\n    // NoC terminal (%u,%u)",x,y);
                    fprintf(this->out,"\n    sc_signal<sc_uint<FLIT_WIDTH> > L_%u_%u_in_data_wire; "    ,x,y);
                    fprintf(this->out,"\n    sc_signal<bool>                 L_%u_%u_in_vc0_val_wire;  ",x,y);
                    fprintf(this->out,"\n    sc_signal<bool>                 L_%u_%u_in_vc0_ret_wire;  ",x,y);
                    fprintf(this->out,"\n    sc_signal<bool>                 L_%u_%u_in_vc1_val_wire;  ",x,y);
                    fprintf(this->out,"\n    sc_signal<bool>                 L_%u_%u_in_vc1_ret_wire;  ",x,y);

                    fprintf(this->out,"\n    sc_signal<sc_uint<FLIT_WIDTH> > L_%u_%u_out_data_wire;"    ,x,y);
                    fprintf(this->out,"\n    sc_signal<bool>                 L_%u_%u_out_vc0_val_wire; ",x,y);
                    fprintf(this->out,"\n    sc_signal<bool>                 L_%u_%u_out_vc0_ret_wire; ",x,y);
                    fprintf(this->out,"\n    sc_signal<bool>                 L_%u_%u_out_vc1_val_wire; ",x,y);
                    fprintf(this->out,"\n    sc_signal<bool>                 L_%u_%u_out_vc1_ret_wire; ",x,y);
                }
            }
            break;

        default	: break;
    }

    fprintf(this->out,"\n\n    // Status signals of the traffic generators attached to the terminals of the NoC");
    for(x=0;x<x_size;x++)
        for(y=0;y<y_size;y++)
            fprintf(this->out,"\n    sc_signal<bool>                 TG_%u_%u_eot_wire; ",x,y);

    for(x=0;x<x_size;x++)
        for(y=0;y<y_size;y++)
            fprintf(this->out,"\n    sc_signal<sc_uint<32> >         TG_%u_%u_number_of_packets_sent_wire; ",x,y);

    for(x=0;x<x_size;x++)
        for(y=0;y<y_size;y++)
            fprintf(this->out,"\n    sc_signal<sc_uint<32> >         TG_%u_%u_number_of_packets_received_wire; ",x,y);

    fprintf(this->out,"\n\n    // Status signal saying that stopsim is ready to stop simulation");
    fprintf(this->out,"\n    sc_signal<bool>                 eos_wire; ");

    fprintf(this->out,"\n\n    /////////////////////////////////////////////////////////////");
    fprintf(this->out,"\n    reset *rst = new reset(\"rst\");");
    fprintf(this->out,"\n    /////////////////////////////////////////////////////////////");
    fprintf(this->out,"\n    rst->clk(clk_wire);");
    fprintf(this->out,"\n    rst->rst(rst_wire);");

    fprintf(this->out,"\n\n    //////////////////////////////////////////////////////////////////////////////");
    fprintf(this->out,"\n    global_clock *clock0 = new global_clock(\"global_clock\");");
    fprintf(this->out,"\n    //////////////////////////////////////////////////////////////////////////////");
    fprintf(this->out,"\n    clock0->clk(clk_wire);");
    fprintf(this->out,"\n    clock0->rst(rst_wire);");
    fprintf(this->out,"\n    clock0->clock_cycles(clock_cycles_wire);");


    fprintf(this->out,"\n\n    //////////////////////////////////////////////////////////////////////////////");
    fprintf(this->out,"\n    stopsim *stop0 = new stopsim(\"stopsim\", (char *) argv[1],(char*)\"stopsim\");");
    fprintf(this->out,"\n    //////////////////////////////////////////////////////////////////////////////");
    fprintf(this->out,"\n    stop0->clk(clk_wire);");
    fprintf(this->out,"\n    stop0->rst(rst_wire);");
    fprintf(this->out,"\n    stop0->eos(eos_wire);");
    fprintf(this->out,"\n    stop0->clock_cycles(clock_cycles_wire);\n");


    for(x=0;x<x_size;x++)
        for(y=0;y<y_size;y++)
            fprintf(this->out,"\n    stop0->TG_%u_%u_number_of_packets_sent(TG_%u_%u_number_of_packets_sent_wire);",x,y,x,y);

    for(x=0;x<x_size;x++)
        for(y=0;y<y_size;y++)
            fprintf(this->out,"\n    stop0->TG_%u_%u_number_of_packets_received(TG_%u_%u_number_of_packets_received_wire);",x,y,x,y);

    for(x=0;x<x_size;x++)
        for(y=0;y<y_size;y++)
            fprintf(this->out,"\n    stop0->TG_%u_%u_eot(TG_%u_%u_eot_wire);",x,y,x,y);

    for(x=0;x<x_size;x++) {
        for(y=0;y<y_size;y++) {
            switch (router_type) {
                case 0: // PARIS :
                    fprintf(this->out,"\n\n    //////////////////////////////////////////////////////////////////////////////");
                    fprintf(this->out,"\n    tg *tg_%u_%u = new tg(\"tg_%u_%u\", %u, %u);",x,y,x,y,x,y);
                    fprintf(this->out,"\n    //////////////////////////////////////////////////////////////////////////////");
                    fprintf(this->out,"\n    tg_%u_%u->clk(clk_wire);",x,y);
                    fprintf(this->out,"\n    tg_%u_%u->rst(rst_wire);",x,y);
                    fprintf(this->out,"\n    tg_%u_%u->clock_cycles(clock_cycles_wire);",x,y);

                    fprintf(this->out,"\n    tg_%u_%u->out_data  (L_%u_%u_in_data_wire );",x,y,x,y);
                    fprintf(this->out,"\n    tg_%u_%u->out_val   (L_%u_%u_in_val_wire  );",x,y,x,y);
                    fprintf(this->out,"\n    tg_%u_%u->out_ret   (L_%u_%u_in_ret_wire  );",x,y,x,y);

                    fprintf(this->out,"\n    tg_%u_%u->in_data   (L_%u_%u_out_data_wire);",x,y,x,y);
                    fprintf(this->out,"\n    tg_%u_%u->in_val    (L_%u_%u_out_val_wire );",x,y,x,y);
                    fprintf(this->out,"\n    tg_%u_%u->in_ret    (L_%u_%u_out_ret_wire );",x,y,x,y);

                    fprintf(this->out,"\n    tg_%u_%u->eot(TG_%u_%u_eot_wire);",x,y,x,y);
                    fprintf(this->out,"\n    tg_%u_%u->number_of_packets_sent(TG_%u_%u_number_of_packets_sent_wire);",x,y,x,y);
                    fprintf(this->out,"\n    tg_%u_%u->number_of_packets_received(TG_%u_%u_number_of_packets_received_wire);",x,y,x,y);
                    break;

                case 1: // PARIS2        :
                case 2: // PARIS2_AG     :
                case 3: // PARIS2_AG_AG  :
                    fprintf(this->out,"\n\n    //////////////////////////////////////////////////////////////////////////////");
                    fprintf(this->out,"\n    tg_2vc *tg_%u_%u = new tg_2vc(\"tg_%u_%u\", %u, %u);",x,y,x,y,x,y);
                    fprintf(this->out,"\n    //////////////////////////////////////////////////////////////////////////////");
                    fprintf(this->out,"\n    tg_%u_%u->clk(clk_wire);",x,y);
                    fprintf(this->out,"\n    tg_%u_%u->rst(rst_wire);",x,y);
                    fprintf(this->out,"\n    tg_%u_%u->clock_cycles(clock_cycles_wire);",x,y);

                    fprintf(this->out,"\n    tg_%u_%u->out_data    (L_%u_%u_in_data_wire    );",x,y,x,y);
                    fprintf(this->out,"\n    tg_%u_%u->vc0_out_val (L_%u_%u_in_vc0_val_wire );",x,y,x,y);
                    fprintf(this->out,"\n    tg_%u_%u->vc0_out_ret (L_%u_%u_in_vc0_ret_wire );",x,y,x,y);
                    fprintf(this->out,"\n    tg_%u_%u->vc1_out_val (L_%u_%u_in_vc1_val_wire );",x,y,x,y);
                    fprintf(this->out,"\n    tg_%u_%u->vc1_out_ret (L_%u_%u_in_vc1_ret_wire );",x,y,x,y);

                    fprintf(this->out,"\n    tg_%u_%u->in_data     (L_%u_%u_out_data_wire   );",x,y,x,y);
                    fprintf(this->out,"\n    tg_%u_%u->vc0_in_val  (L_%u_%u_out_vc0_val_wire);",x,y,x,y);
                    fprintf(this->out,"\n    tg_%u_%u->vc0_in_ret  (L_%u_%u_out_vc0_ret_wire);",x,y,x,y);
                    fprintf(this->out,"\n    tg_%u_%u->vc1_in_val  (L_%u_%u_out_vc1_val_wire);",x,y,x,y);
                    fprintf(this->out,"\n    tg_%u_%u->vc1_in_ret  (L_%u_%u_out_vc1_ret_wire);",x,y,x,y);

                    fprintf(this->out,"\n    tg_%u_%u->eot(TG_%u_%u_eot_wire);",x,y,x,y);
                    fprintf(this->out,"\n    tg_%u_%u->number_of_packets_sent(TG_%u_%u_number_of_packets_sent_wire);",x,y,x,y);
                    fprintf(this->out,"\n    tg_%u_%u->number_of_packets_received(TG_%u_%u_number_of_packets_received_wire);",x,y,x,y);
                    break;

                default	: break;
            }
        }
    }


    for(x=0;x<x_size;x++) {
        for(y=0;y<y_size;y++) {
            switch (router_type) {
                case 0: // PARIS :
                    fprintf(this->out,"\n\n    //////////////////////////////////////////////////////////////////////////////");
                    fprintf(this->out,"\n    tm_single *tm_%u_%u_out = new tm_single(\"tm_%u_%u_out\", %u, %u,(char *) argv[1], (char*)\"ext_%u_%u_out\");",x,y,x,y,x,y,x,y);
                    fprintf(this->out,"\n    //////////////////////////////////////////////////////////////////////////////");
                    fprintf(this->out,"\n    tm_%u_%u_out->clk(clk_wire);",x,y);
                    fprintf(this->out,"\n    tm_%u_%u_out->rst(rst_wire);",x,y);
                    fprintf(this->out,"\n    tm_%u_%u_out->eos(eos_wire);",x,y);
                    fprintf(this->out,"\n    tm_%u_%u_out->clock_cycles(clock_cycles_wire);",x,y);
                    fprintf(this->out,"\n    tm_%u_%u_out->data(L_%u_%u_out_data_wire );",x,y,x,y);
                    fprintf(this->out,"\n    tm_%u_%u_out->val (L_%u_%u_out_val_wire  );",x,y,x,y);
                    fprintf(this->out,"\n    tm_%u_%u_out->ret (L_%u_%u_out_ret_wire  );",x,y,x,y);
                    break;

                case 1: // PARIS2        :
                case 2: // PARIS2_AG     :
                case 3: // PARIS2_AG_AG  :
                    fprintf(this->out,"\n\n    //////////////////////////////////////////////////////////////////////////////");
                    fprintf(this->out,"\n    tm_2vc *tm_%u_%u_out = new tm_2vc(\"tm_%u_%u_out\", %u, %u,(char *) argv[1], (char*)\"ext_%u_%u_out\");",x,y,x,y,x,y,x,y);
                    fprintf(this->out,"\n    //////////////////////////////////////////////////////////////////////////////");
                    fprintf(this->out,"\n    tm_%u_%u_out->clk(clk_wire);",x,y);
                    fprintf(this->out,"\n    tm_%u_%u_out->rst(rst_wire);",x,y);
                    fprintf(this->out,"\n    tm_%u_%u_out->eos(eos_wire);",x,y);
                    fprintf(this->out,"\n    tm_%u_%u_out->clock_cycles(clock_cycles_wire);",x,y);
                    fprintf(this->out,"\n    tm_%u_%u_out->data(L_%u_%u_out_data_wire );",x,y,x,y);
                    fprintf(this->out,"\n    tm_%u_%u_out->vc0_val (L_%u_%u_out_vc0_val_wire  );",x,y,x,y);
                    fprintf(this->out,"\n    tm_%u_%u_out->vc0_ret (L_%u_%u_out_vc0_ret_wire  );",x,y,x,y);
                    fprintf(this->out,"\n    tm_%u_%u_out->vc1_val (L_%u_%u_out_vc1_val_wire  );",x,y,x,y);
                    fprintf(this->out,"\n    tm_%u_%u_out->vc1_ret (L_%u_%u_out_vc1_ret_wire  );",x,y,x,y);
                    break;

                default : break;
            }
        }
    }

    switch (router_type) {
        case 0: // PARIS :
            fprintf(this->out,"\n\n    //////////////////////////////////////////////////////////////////////////////");
            fprintf(this->out,"\n    socin *socin0 = new socin(\"socin\");");
            fprintf(this->out,"\n    //////////////////////////////////////////////////////////////////////////////");
            fprintf(this->out,"\n    // System signals");
            fprintf(this->out,"\n    socin0->clk(clk_wire);");
            fprintf(this->out,"\n    socin0->rst(rst_wire);");

            for(x=0;x<x_size;x++) {
                for(y=0;y<y_size;y++) {
                    fprintf(this->out,"\n\n    // NoC terminal %u %u",x,y);
                    fprintf(this->out,"\n    socin0->L_%u_%u_in_data (L_%u_%u_in_data_wire );",x,y,x,y);
                    fprintf(this->out,"\n    socin0->L_%u_%u_in_val  (L_%u_%u_in_val_wire  );",x,y,x,y);
                    fprintf(this->out,"\n    socin0->L_%u_%u_in_ret  (L_%u_%u_in_ret_wire  );",x,y,x,y);
                    fprintf(this->out,"\n    socin0->L_%u_%u_out_data(L_%u_%u_out_data_wire);",x,y,x,y);
                    fprintf(this->out,"\n    socin0->L_%u_%u_out_val (L_%u_%u_out_val_wire );",x,y,x,y);
                    fprintf(this->out,"\n    socin0->L_%u_%u_out_ret (L_%u_%u_out_ret_wire );",x,y,x,y);
                }
            }
            break;

        case 1: // PARIS2        :
        case 2: // PARIS2_AG     :
        case 3: // PARIS2_AG_AG  :
            fprintf(this->out,"\n\n    //////////////////////////////////////////////////////////////////////////////");
            fprintf(this->out,"\n    socin_2vc *socin0 = new socin_2vc(\"socin\");");
            fprintf(this->out,"\n    //////////////////////////////////////////////////////////////////////////////");
            fprintf(this->out,"\n    // System signals");
            fprintf(this->out,"\n    socin0->clk(clk_wire);");
            fprintf(this->out,"\n    socin0->rst(rst_wire);");

            for(x=0;x<x_size;x++) {
                for(y=0;y<y_size;y++) {
                    fprintf(this->out,"\n\n    // NoC terminal %u %u",x,y);
                    fprintf(this->out,"\n    socin0->L_%u_%u_in_data     (L_%u_%u_in_data_wire     );",x,y,x,y);
                    fprintf(this->out,"\n    socin0->L_%u_%u_vc0_in_val  (L_%u_%u_in_vc0_val_wire  );",x,y,x,y);
                    fprintf(this->out,"\n    socin0->L_%u_%u_vc0_in_ret  (L_%u_%u_in_vc0_ret_wire  );",x,y,x,y);
                    fprintf(this->out,"\n    socin0->L_%u_%u_vc1_in_val  (L_%u_%u_in_vc1_val_wire  );",x,y,x,y);
                    fprintf(this->out,"\n    socin0->L_%u_%u_vc1_in_ret  (L_%u_%u_in_vc1_ret_wire  );",x,y,x,y);
                    fprintf(this->out,"\n    socin0->L_%u_%u_out_data    (L_%u_%u_out_data_wire    );",x,y,x,y);
                    fprintf(this->out,"\n    socin0->L_%u_%u_vc0_out_val (L_%u_%u_out_vc0_val_wire );",x,y,x,y);
                    fprintf(this->out,"\n    socin0->L_%u_%u_vc0_out_ret (L_%u_%u_out_vc0_ret_wire );",x,y,x,y);
                    fprintf(this->out,"\n    socin0->L_%u_%u_vc1_out_val (L_%u_%u_out_vc1_val_wire );",x,y,x,y);
                    fprintf(this->out,"\n    socin0->L_%u_%u_vc1_out_ret (L_%u_%u_out_vc1_ret_wire );",x,y,x,y);
                }
            }
            break;

        default	: break;
    }


    if (vcd_option > 0) {
        fprintf(this->out,"\n\n\n    // Signal tracing");
        fprintf(this->out,"\n    sc_trace_file *tf=sc_create_vcd_trace_file(\"waves\");");
        fprintf(this->out,"\n    sc_trace(tf, clk_wire           , \"clk           \");");
        fprintf(this->out,"\n    sc_trace(tf, eos_wire           , \"eos           \");");

        if (vcd_option > 1) {
            fprintf(this->out,"\n    sc_trace(tf, rst_wire           , \"rst           \");");
            fprintf(this->out,"\n    sc_trace(tf, clock_cycles_wire  , \"global_clock  \");");

            if (vcd_option > 2) {
                for(x=0;x<x_size;x++) {
                    for(y=0;y<y_size;y++) {
                        fprintf(this->out,"\n\n    sc_trace(tf, L_%u_%u_in_data_wire , \"L_%u_%u_in_data \");",x,y,x,y);

                        switch (router_type) {
                            case 0: // PARIS :
                                if (vcd_option > 3) {
                                    fprintf(this->out,"\n    sc_trace(tf, L_%u_%u_in_val_wire  , \"L_%u_%u_in_val  \");",x,y,x,y);
                                    fprintf(this->out,"\n    sc_trace(tf, L_%u_%u_in_ret_wire  , \"L_%u_%u_in_ret  \");",x,y,x,y);
                                }
                                break;

                            case 1: // PARIS2        :
                            case 2: // PARIS2_AG     :
                            case 3: // PARIS2_AG_AG  :
                                if (vcd_option > 3) {
                                    fprintf(this->out,"\n    sc_trace(tf, L_%u_%u_in_vc0_val_wire  , \"L_%u_%u_in_vc0_val  \");",x,y,x,y);
                                    fprintf(this->out,"\n    sc_trace(tf, L_%u_%u_in_vc0_ret_wire  , \"L_%u_%u_in_vc0_ret  \");",x,y,x,y);
                                    fprintf(this->out,"\n    sc_trace(tf, L_%u_%u_in_vc1_val_wire  , \"L_%u_%u_in_vc1_val  \");",x,y,x,y);
                                    fprintf(this->out,"\n    sc_trace(tf, L_%u_%u_in_vc1_ret_wire  , \"L_%u_%u_in_vc1_ret  \");",x,y,x,y);
                                }
                                break;

                            default	: break;
                        } // switch (router_type)

                        fprintf(this->out,"\n    sc_trace(tf, L_%u_%u_out_data_wire, \"L_%u_%u_out_data\");",x,y,x,y);

                        switch (router_type) {
                            case 0: // PARIS :
                                if (vcd_option > 3) {
                                    fprintf(this->out,"\n    sc_trace(tf, L_%u_%u_out_val_wire , \"L_%u_%u_out_val \");",x,y,x,y);
                                    fprintf(this->out,"\n    sc_trace(tf, L_%u_%u_out_ret_wire , \"L_%u_%u_out_ret \");",x,y,x,y);
                                }
                                break;

                            case 1: // PARIS2        :
                            case 2: // PARIS2_AG     :
                            case 3: // PARIS2_AG_AG  :
                                if (vcd_option > 3) {
                                    fprintf(this->out,"\n    sc_trace(tf, L_%u_%u_out_vc0_val_wire , \"L_%u_%u_vc0_out_val \");",x,y,x,y);
                                    fprintf(this->out,"\n    sc_trace(tf, L_%u_%u_out_vc0_ret_wire , \"L_%u_%u_vc0_out_ret \");",x,y,x,y);
                                    fprintf(this->out,"\n    sc_trace(tf, L_%u_%u_out_vc1_val_wire , \"L_%u_%u_vc1_out_val \");",x,y,x,y);
                                    fprintf(this->out,"\n    sc_trace(tf, L_%u_%u_out_vc1_ret_wire , \"L_%u_%u_vc1_out_ret \");",x,y,x,y);

                                }
                                break;

                            default	: break;
                        } // switch (router_type)
                    }//for(y=0;y<y_size;y++)
                }//for(x=0;x<x_size;x++)
            }//if (vcd_option > 2)
        }//if (vcd_option > 1)
    }//if (vcd_option > 0)

    if (vcd_option > 4) {
        fprintf(this->out,"\n");
        for(x=0;x<x_size;x++)
            for(y=0;y<y_size;y++)
                fprintf(this->out,"\n    sc_trace(tf, TG_%u_%u_eot_wire    , \"TG_%u_%u_eot    \");",x,y,x,y);
    }

    fprintf(this->out,"\n\n    // Start the simulation (the testbench will stop it)");
    fprintf(this->out,"\n    sc_start();");

    if (vcd_option > 0){
        fprintf(this->out,"\n\n    sc_close_vcd_trace_file(tf);");
    }
    fprintf(this->out,"\n    return 0;");
    fprintf(this->out,"\n}");
    fprintf(this->out,"\n");

    fclose(this->out);

    return true;
}

bool GSoC::generateListNodes() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/GSoC::generateListNodes" << std::endl;
#endif

    unsigned int vcd_option = this->systemParameters->getVcdOption();

    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    //                                                                          //
    //                     CREATION OF THE FILE "list_nodes.sav"                //
    //                                                                          //
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    if (vcd_option > 0) {
        sprintf(this->fileName,"system/list_nodes.sav");
        if ((this->out=fopen(this->fileName,"wt")) == NULL){
            printf("\n\tERROR: Cannot open the file \"%s\".\n\n",this->fileName);
            return false;
        }
        unsigned int x_size = this->systemParameters->getXSize();
        unsigned int y_size = this->systemParameters->getYSize();
        unsigned int router_type = this->experimento->getRouterArchitecture();
        unsigned int flit_width = this->systemParameters->getPhitWidth();
        unsigned int x,y;   // Loop counters

        if (vcd_option > 1) {
            fprintf(this->out,"@28");
            fprintf(this->out,"\nSystemC.clk");
            fprintf(this->out,"\nSystemC.eos");
            fprintf(this->out,"\nSystemC.rst");

            if (vcd_option > 2) {
                fprintf(this->out,"\n@200");

                switch (router_type) {
                    case 0: // PARIS :
                        for(x=0;x<x_size;x++) {
                            for(y=0;y<y_size;y++) {
                                fprintf(this->out,"\n-Core_%u_%u",x,y);
                                fprintf(this->out,"\n@22");
                                fprintf(this->out,"\nSystemC.L_%u_%u_in_data[%u:0]",x,y,flit_width-1);

                                if (vcd_option > 3) {
                                    fprintf(this->out,"\n@28");
                                    fprintf(this->out,"\nSystemC.L_%u_%u_in_val",x,y);
                                    fprintf(this->out,"\nSystemC.L_%u_%u_in_ret",x,y);
                                }

                                fprintf(this->out,"\n@22");
                                fprintf(this->out,"\nSystemC.L_%u_%u_out_data[%u:0]",x,y,flit_width-1);

                                if (vcd_option > 3) {
                                    fprintf(this->out,"\n@28");
                                    fprintf(this->out,"\nSystemC.L_%u_%u_out_val",x,y);
                                    fprintf(this->out,"\nSystemC.L_%u_%u_out_ret",x,y);
                                    fprintf(this->out,"\n@200");
                                }
                            }
                        }
                        break;

                    case 1: // PARIS2       :
                    case 2: // PARIS2_AG    :
                    case 3: // PARIS2_AG_AG :
                        for(y=0;y<y_size;y++) {
                            for(x=0;x<x_size;x++) {
                                fprintf(this->out,"\n-Core_%u_%u",x,y);
                                fprintf(this->out,"\n@22");
                                fprintf(this->out,"\nSystemC.L_%u_%u_in_data[%u:0]",x,y,flit_width-1);

                                if (vcd_option > 3) {
                                    fprintf(this->out,"\n@28");
                                    fprintf(this->out,"\nSystemC.L_%u_%u_in_vc0_val",x,y);
                                    fprintf(this->out,"\nSystemC.L_%u_%u_in_vc0_ret",x,y);
                                    fprintf(this->out,"\nSystemC.L_%u_%u_in_vc1_val",x,y);
                                    fprintf(this->out,"\nSystemC.L_%u_%u_in_vc1_ret",x,y);
                                }

                                fprintf(this->out,"\n@22");
                                fprintf(this->out,"\nSystemC.L_%u_%u_out_data[%u:0]",x,y,flit_width-1);

                                if (vcd_option > 3) {
                                    fprintf(this->out,"\n@28");
                                    fprintf(this->out,"\nSystemC.L_%u_%u_vc0_out_val",x,y);
                                    fprintf(this->out,"\nSystemC.L_%u_%u_vc0_out_ret",x,y);
                                    fprintf(this->out,"\nSystemC.L_%u_%u_vc1_out_val",x,y);
                                    fprintf(this->out,"\nSystemC.L_%u_%u_vc1_out_ret",x,y);
                                    fprintf(this->out,"\n@200");
                                }
                            }
                        }
                        break;

                    default : break;
                }
            }

            if (vcd_option > 4) {
                fprintf(this->out,"\n-");
                fprintf(this->out,"\n@28");
                for(x=0;x<x_size;x++)
                    for(y=0;y<y_size;y++)
                        fprintf(this->out,"\nSystemC.TG_%u_%u_eot",x,y);

                fprintf(this->out,"\n");
            }
        }

        fclose(this->out);
        return true;
    } else {
        return true;
    }
    return false;

}

////////////////////////////////////////////////////////////////////////////////
void GSoC::generateSoC() throw (const char *){
////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Model/SystemConfiguration/GSoC::generateSoC" << std::endl;
#endif

    try {
        FolderOperation::createFolder("system");
    } catch(const char* problem) {
        throw problem;
    }

    if( !generateHeaderStopSim() ) {
        throw "Problem occurred in generation header stopsim(.h).\nNone generated file";
    }

    if( !generateStopSim() ) {
        throw "Problem occurred in generation source stopsim(.cpp).\nHeader stopsim generated";
    }

    if( !generateMain() ) {
        throw "Problem occurred in generation source main(.cpp).\nHeader and source stopsim generated";
    }

    if( !generateListNodes() ) {
        throw "Problem occurred in generation list_nodes.sav.\nHeader and source of stopsim, and main file generated";
    }

}
