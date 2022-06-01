module full_adder(input din1, input din2, input cin, output sum, output cout) ;
    assign sum = din1 ^ din2 ^ cin;
    assign cout = (din1 & din2) | (din1 ^ din2) & cin;
endmodule