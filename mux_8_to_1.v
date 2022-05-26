module JK_FF(Q,J,K,Clk,EN);
	input EN;
	output Q;
	input J,K,Clk;
	reg Q;
	always @(negedge Clk)
		case({J,K,EN})
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

module FreqDiv(iClk,oClk);
	input iClk;
	output oClk;
	reg oClk;
	integer count;
	parameter RATE =  50000000 ;
	initial 
		count = 0;
	always @(posedge iClk)
		begin 
			if(count < RATE/2)
				oClk = 1;
			else
				oClk = 0;
			count = (count + 1) % RATE;
		end 
endmodule 

module MUX( I,A,F );
	input [7:0]I;
	input [2:0]A;
	output F;
	reg F;
	always @(A)
		begin	
			case({A})
				0:F = I[0];
				1:F = I[1];
				2:F = I[2];
				3:F = I[3];
				4:F = I[4];
				5:F = I[5];
				6:F = I[6];
				7:F = I[7];
				default:F = 0; //HI
			endcase
	end
endmodule 
	
module experiment10(F,f,a,A,I,IClk,EN);
	input EN;
	input IClk;
	input [7:0]I;
	output F;
	output [2:0]A;
	wire OClk;
	wire [2:0]temp;
	wire en;
	assign en = EN;
	assign temp[0] = A[0];
	assign temp[1] = A[1];
	assign temp[2] = A[2];
	FreqDiv Fre( IClk, OClk);
	JK_FF JK0( A[0], 1, 1, OClk, en);
	JK_FF JK1( A[1], 1, 1, temp[0], en);
	JK_FF JK2( A[2], 1, 1, temp[1], en);
	MUX M( I, temp, F);
	
	wire ffff;
	output [2:0]a;
	output f;
	assign a[0] = temp[0];
	assign a[1] = temp[1];
	assign a[2] = temp[2];
	assign ffff = F;
	assign f = ffff;
	
endmodule
	