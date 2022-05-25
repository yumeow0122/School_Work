module decoder_2_to_4(input enable, input [1:0]din, output reg[3:0]dout);
	always @(din)begin
		if(~enable) begin
			case(din)
				2'b00: dout = 4'b1110;
				2'b01: dout = 4'b1101;
				2'b10: dout = 4'b1011;
				2'b11: dout = 4'b0111;
			endcase
		end
		else
			dout = 4'b1111;
	end
endmodule	

module decoder_3_to_8(input enable, input [2:0]din, output reg[7:0]dout);
	always @(din)begin
		if(~enable) begin
			case(din)
				3'b000: dout = 8'b11111110;
				3'b001: dout = 8'b11111101;
				3'b010: dout = 8'b11111011;
				3'b011: dout = 8'b11110111;
				3'b100: dout = 8'b11101111;
				3'b101: dout = 8'b11011111;
				3'b110: dout = 8'b10111111;
				3'b111: dout = 8'b01111111;
			endcase
		end
		else
			dout = 8'b11111111;
	end
endmodule

module decoder(input enable, input [3:0]din, output [15:0]dout);
		wire [7:0] abc;
		decoder_3_to_8(enable, {1'b0,din[1:0]}, abc);
		decoder_2_to_4(abc[0], din[3:2], dout[3:0]);
		decoder_2_to_4(abc[1], din[3:2], dout[7:4]);
		decoder_2_to_4(abc[2], din[3:2], dout[11:8]);
		decoder_2_to_4(abc[3], din[3:2], dout[15:12]);
endmodule

