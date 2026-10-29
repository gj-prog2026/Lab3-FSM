module f1_fsm (
    input   logic       rst,
    input   logic       en,
    input   logic       clk,
    output  logic [7:0] data_out
);

// bit => a data type describing a bit can only take 0/1 value
// logic => a bit can take 0/1/X/Z
// enum is by default 32-bit so you can reduce the array size
typedef enum bit [3:0] {s0, s1, s2, s3, s4, s5, s6, s7, s8} f1_states;
f1_states current_state, next_state;

always_ff @(posedge clk) begin
    if(rst) current_state <= s0;
    else current_state <= next_state;
end

always_comb begin
    next_state = current_state;
    case(current_state)
        s0:     if(en)  next_state = s1;
        s1:     if(en)  next_state = s2;
        s2:     if(en)  next_state = s3;
        s3:     if(en)  next_state = s4;
        s4:     if(en)  next_state = s5;
        s5:     if(en)  next_state = s6;
        s6:     if(en)  next_state = s7;
        s7:     if(en)  next_state = s8; 
        s8:     if(en)  next_state = s0; 
        default: next_state = s0;
    endcase
end

always_comb begin
    case(current_state)
        s1:     data_out = {{7{1'b0}}, 1'b1};
        s2:     data_out = {{6{1'b0}}, {2{1'b1}}};
        s3:     data_out = {{5{1'b0}}, {3{1'b1}}};
        s4:     data_out = {{4{1'b0}}, {4{1'b1}}};
        s5:     data_out = {{3{1'b0}}, {5{1'b1}}};
        s6:     data_out = {{2{1'b0}}, {6{1'b1}}};
        s7:     data_out = {{1{1'b0}}, {7{1'b1}}};
        s8:     data_out = {8{1'b1}};
        default: data_out = {{8{1'b0}}};
    endcase
end
endmodule
