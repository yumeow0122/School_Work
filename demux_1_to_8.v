module demux_4_to_8(input din, input [2:0]sel, output reg[7:0]dout);
	always @(sel)begin
		case(sel)
			3'b000: dout = {7'b0000000, din};
			3'b001: dout = {6'b000000, din, 1'b0};
			3'b010: dout = {5'b00000, din, 2'b00};
			3'b011: dout = {4'b0000, din, 3'b000};
			3'b100: dout = {3'b000, din, 4'b0000};
			3'b101: dout = {2'b00, din, 5'b00000};
			3'b110: dout = {1'b0, din, 6'b000000};
			3'b111: dout = {din, 7'b0000000};
		default: 
			dout = 8'b00000000;
		endcase
	end
endmodule	

