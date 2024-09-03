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