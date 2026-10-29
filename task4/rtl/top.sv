module top#(
    N_WIDTH = 5,
    K_WIDTH = 7,
    D_WIDTH = 8
)(
    input logic                     clk,
    input logic                     rst,
    input logic                     trigger,
    input logic                     en,
    input logic     [N_WIDTH-1:0]   N,

    output logic    [D_WIDTH-1:0]   data_out
);

logic tick;
logic [K_WIDTH-1:0] K;
logic cmd_seq;
logic cmd_delay;
logic time_out;
logic enable;

clktick #(
    .WIDTH(N_WIDTH)
)myclk(
    .clk(clk),
    .rst(rst),
    .tick(tick),
    .N(N),
    .en(en)
);

lfsr_7 myrand(
    .clk(clk),
    .en(en),
    .rst(rst),
    .data_out(K)
);

delay #(.WIDTH(K_WIDTH))mydelay(
    .clk(clk),
    .rst(rst),
    .trigger(cmd_delay),
    .n(K),
    .time_out(time_out)
);

always_comb begin
    if(cmd_seq) enable <= tick;
    else enable <= time_out;
end


f1_fsm myf1(
    .rst(rst),
    .en(enable),
    .trigger(trigger),
    .clk(clk),
    .cmd_delay(cmd_delay),
    .cmd_seq(cmd_seq),
    .data_out(data_out)
);
endmodule
