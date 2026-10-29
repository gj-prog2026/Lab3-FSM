#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "../../vbuddy.cpp"

int main(int argc, char **argv, char **env){
    int symcyc = 0;

    Verilated::commandArgs(argc, argv);
    Verilated::traceEverOn(true);
    Vtop* top = new Vtop;
    VerilatedVcdC* tfp= new VerilatedVcdC;

    top->trace(tfp, 99);
    tfp->open("top.vcd");

    if (vbdOpen() != 1) return -1;
    vbdHeader("L3T3: Auto F1");
    top->clk = 1;
    top->en = 0;
    top->rst = 1;
    top->N = 40;

    for(; symcyc < 100000; symcyc++){
        top->rst = (symcyc < 2);
        for(int tick = 0; tick < 2; tick ++){
            tfp->dump(symcyc*2 + tick);
            top->clk = !top->clk;
            top->eval();
        }

        vbdBar(top->data_out & 0xFF);
        top->en = 1;
        vbdCycle(symcyc);

        if(Verilated::gotFinish() || vbdGetkey() == 'q'){
            printf("\nExit\n");
            exit(0);
        }

    }
    vbdClose();
    tfp->close();
    return 0;
}