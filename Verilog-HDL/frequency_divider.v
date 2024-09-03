module frequence_divider(input iClk, output reg oClk);
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