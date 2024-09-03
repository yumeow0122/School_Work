module encoder_4to_2(input [3:0] din, output [1:0] dout);
    always @(din)
        begin
            case(din)
                4'b1110: dout=2'b11; // 0
                4'b1101: dout=2'b10; // 1
                4'b1011: dout=2'b01; // 2
                4'b0111: dout=2'b00; // 3
                default: dout=2'b11; // DEFAULT 0
            endcase
        end
endmodule