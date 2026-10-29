#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vf1_fsm.h"
#include "../vbuddy.cpp"

int main(int argc, char **argv, char **env){
    Verilated::commandArgs(argc, argv);
    Vf1_fsm* top = new Vf1_fsm;
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;

    top-> trace(tfp, 99);
    tfp->open("f1_fsm.vcd");

    top->rst = 1;
    top->clk = 1;
    top->en = 0;
    if(vbdOpen() != 1) return (-1);
    vbdHeader("L3T2: F1 FSM");
    vbdSetMode(1);
    
    for(int i = 0; i < 10000; i++){
        top->rst = (i < 2);
        for(int tick = 0; tick < 2; tick++){
            tfp->dump(2*i + tick);
            top->clk = !top->clk;
            top->eval();
        }
        vbdBar(top->data_out & 0xFF);
        top->en = vbdFlag();
        vbdCycle(i);

        if(Verilated::gotFinish() || vbdGetkey()=='q'){
            exit(0);
        }
    }
    vbdClose();
    tfp->close();
    printf("Exiting\n");
    exit(0);
}