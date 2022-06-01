module seven_segment(input [3:0]din, output reg [6:0]dout);
	always @(din)
		begin
			case(din)
                4'b0000: dout = 7'b1000000; // 0
                4'b0001: dout = 7'b1111001; // 1
                4'b0010: dout = 7'b0100100; // 2
                4'b0011: dout = 7'b0110000; // 3
                4'b0100: dout = 7'b0011001; // 4
                4'b0101: dout = 7'b0010010; // 5
                4'b0110: dout = 7'b0000010; // 6
                4'b0111: dout = 7'b1111000; // 7
                4'b1000: dout = 7'b0000000; // 8
                4'b1001: dout = 7'b0011000; // 9
                4'b1010: dout = 7'b0001000; // A
                4'b1011: dout = 7'b0000011; // b
                4'b1100: dout = 7'b1000110; // C
                4'b1101: dout = 7'b0100001; // d
                4'b1110: dout = 7'b0000110; // E
                4'b1111: dout = 7'b0001110; // F
				default: dout = 7'b1111111; // null
			endcase
		end					
endmodule