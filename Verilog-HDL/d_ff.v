module D_FF(input D, input clk, output reg Q, output reg NQ);
    always @(posedge clk)
        begin
            Q = D;
            NQ = ~Q;
        end
endmodule