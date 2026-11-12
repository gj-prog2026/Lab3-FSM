#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vtop.h"

#include "../vbuddy.cpp" // include vbuddy code
#define MAX_SIM_CYC 10000000

int main(int argc, char **argv, char **env)
{
    int simcyc;      // simulation clock count
    int tick;        // each clk cycle has two ticks for two edges
    int prev_data = 0;
    int waiting = 0; // 1 = lights are off, stopwatch running, waiting for press

    Verilated::commandArgs(argc, argv);
    Vtop *top = new Vtop;

    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("top.vcd");

    if (vbdOpen() != 1)
        return (-1);
    vbdHeader("L3T4:F1 FSM");
    vbdSetMode(1); // one-shot flag mode

    top->clk = 1;
    top->rst = 0;
    top->en = 1;           // must be high or nothing runs
    top->trigger = 0;
    top->N = 40;   // or hardcode your calibrated N

    for (simcyc = 0; simcyc < MAX_SIM_CYC; simcyc++)
    {
        for (tick = 0; tick < 2; tick++)
        {
            tfp->dump(2 * simcyc + tick);
            top->clk = !top->clk;
            top->eval();
        }

        // show LEDs only when they change
        if (top->data_out != prev_data)
        {
            vbdBar(top->data_out & 0xFF);

            // all lights just went OFF -> start the stopwatch
            if (prev_data == 0xFF && top->data_out == 0x00)
            {
                vbdInitWatch();
                waiting = 1;
            }
            prev_data = top->data_out;
        }

        top->rst = (simcyc < 2);

        // route the single switch: reaction press OR start trigger
        int flag = vbdFlag();
        top->trigger = 0;
        if (waiting)
        {
            if (flag)
            {
                int rtime = vbdElapsed();   // ms
                vbdHex(3, (rtime / 100) % 10);
                vbdHex(2, (rtime / 10) % 10);
                vbdHex(1, rtime % 10);
                waiting = 0;
            }
        }
        else
        {
            top->trigger = flag;
        }

        vbdCycle(simcyc);

        if (Verilated::gotFinish() || vbdGetkey() == 'q')
            break;
    }

    vbdClose();
    tfp->close();
    exit(0);
}
