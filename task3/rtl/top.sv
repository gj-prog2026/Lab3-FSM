module top#(
    parameter D_WIDTH = 8,
    parameter N_WIDTH = 16
)(
    input logic clk,
    input logic rst,
    input logic en,
    input logic [N_WIDTH-1:0] N,
    output logic [D_WIDTH-1:0] data_out
);

logic tick;

clktick #(.WIDTH(N_WIDTH)) myclk(
    .clk(clk),
    .N(N),
    .rst(rst),
    .en(en),
    .tick(tick)
);

f1_fsm myf1(
    .rst(rst),
    .en(tick),
    .clk(clk),
    .data_out(data_out)
);

endmodule