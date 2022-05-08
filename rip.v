module half_adder(output S, C, input a, b);
	assign S = a & b;
	assign C = a ^ b;
endmodule

module full_adder(output S, C, input a, b, c);
	assign S = a ^ b ^ c;
	assign C = (a & b) | (a ^ b) & c;
endmodule

module ripple_adder(output [3:0]S, output C4, input [3:0] A,B, input C0);
	wire C1, C2, C3;
	full_adder(S[0], C1, A[0], B[0], C0);
	full_adder(S[1], C2, A[1], B[1], C1);
	full_adder(S[2], C3, A[2], B[2], C2);
	full_adder(S[3], C4, A[3], B[3], C3);
endmodule

module seven_seg(dout, din);
	input  [3:0] din;
	output [6:0] dout;

	reg [6:0] dout;
	
	always @(din)
		begin
			case(din)
				4'b0000:dout = 7'b1000000;
				4'b0001:dout = 7'b1111001;
				4'b0010:dout = 7'b0100100;
				4'b0011:dout = 7'b0110000;
				4'b0100:dout = 7'b0011001;
				4'b0101:dout = 7'b0010010;
				4'b0110:dout = 7'b0000010;
				4'b0111:dout = 7'b1111000;
				4'b1000:dout = 7'b0000000;
				4'b1001:dout = 7'b0010000;
				4'b1010:dout = 7'b0001000;
				4'b1011:dout = 7'b0000011;
				4'b1100:dout = 7'b1000110;
				4'b1101:dout = 7'b0100001;
				4'b1110:dout = 7'b0000110;
				4'b1111:dout = 7'b0001110;
				default:dout = 7'b1111111;
			endcase
		end					
endmodule

module rip(output [6:0]dout, output C4, input [3:0] A,B, input C0);
	wire [3:0]S;
	ripple_adder(S[3:0], C4, A[3:0], B[3:0], C0);
	seven_seg(dout[6:0], S[3:0]);	
endmodule
