module Bidirectional_Shift_Register(sw_mode, sw_control, sw_nums, dout, clk);
    input mode;     // johnson or ring
    input control; // load or not
    input [4:0] sw_nums;
    input clk;

    output reg [4:0] dout;

    wire[4:0] Q;

    always @(negedge clk)
        begin
            if(mode)      // choose johnson counter
                begin
                    dout[3:0] =  Q[3:0];
                    dout[4]   = ~Q[4];
                end
                else            // choose ring counter
                begin
                    dout = Q;
                end
            if(control) // load
                begin
                    dout = sw_nums;
                 end
        end

    J_K_FF (dout[4], ~dout[4], clk, Q[0]);
    J_K_FF (dout[0], ~dout[0], clk, Q[1]);
    J_K_FF (dout[1], ~dout[1], clk, Q[2]);
    J_K_FF (dout[2], ~dout[2], clk, Q[3]);
    J_K_FF (dout[3], ~dout[3], clk, Q[4]);

endmodule


module J_K_FF(J, K, clk, Q);
    input J, K;
    input  clk;

    output reg Q;

    always @(posedge clk)
        begin
            if(J==0 & K==1) // reset
                Q = 0;
            if(J==1 & K==0) // set
                Q = 1;
            if(J==1 & K==1) // toggle
                Q = ~Q;
        end
endmodule
