module tx(
    output tx_out,
    input xtal_in,
);

    wire pll_out;
    ipll ipll(.xtal_in(xtal_in), .clk(tx_out));


endmodule
