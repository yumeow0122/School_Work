module half_adder(input din1, input din2, output sum, output cout)
    assign sum = din1 ^ din2;
    assign cout = din1 & din2;
endmodule