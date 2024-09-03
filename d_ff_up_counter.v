module D_FF(input D, input clk, output reg Q, output reg NQ);
    always @(posedge clk)
        begin
            Q = D;
            NQ = ~Q;
        end
endmodule

module D_FF_UP_COUNTER(input sw_mode, input sw_control, input clk input [4:0] sw_nums, output [4:0] leds);
    wire[4:0] Q, NQ;
    reg[4:0] temp;
    always @(negedge clk)
        begin
            if(sw_mode) //Johnson Counter
                begin
                    temp[3:0] = Q[3:0];
                    temp[4] = Qbar[4];
                end
            else //Ring Counter
                begin
                    temp = Q;
                end

            if(sw_control) //Load Number
                begin
                    temp = sw_nums;
                end
        end

    D_FF (temp[4], clk, Q[0], NQ[0]);
    D_FF (temp[0], clk, Q[1], NQ[1]);
    D_FF (temp[1], clk, Q[2], NQ[2]);
    D_FF (temp[2], clk, Q[3], NQ[3]);
    D_FF (temp[3], clk, Q[4], NQ[4]);
    assign leds = temp;
endmodule

