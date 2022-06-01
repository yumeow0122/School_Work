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