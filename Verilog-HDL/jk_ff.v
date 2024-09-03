module JK_FF(input J, input K, input clk, output reg Q);
    always @(posedge clk)
        begin
            if(J==0 & K==1)
                begin
                    Q = 0;
                end
            if(J==1 & K==0)
                begin
                    Q = 1;
                end
            if(J==1 & K==1)
                begin
                    Q = ~Q;
                end
        end
endmodule

module JK_FF(input J, input K, input clk, input en, output reg Q);
	always @(negedge clk)
		case({J,K,en})
			3'b000 : Q <= Q;
			3'b010 : Q <= 0;
			3'b100 : Q <= 1;
			3'b110 : Q <= ~Q;
			
			3'b001 : Q <= Q;
			3'b011 : Q <= Q;
			3'b101 : Q <= Q;
			3'b111 : Q <= Q;
		endcase 
endmodule 