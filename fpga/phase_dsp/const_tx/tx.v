module tx(
    output tx_out,
    input xtal_in,
);

	SB_IO #(
		.PIN_TYPE(6'b101001)
	) lp_tx_out (
		.PACKAGE_PIN(tx_out),
		.OUTPUT_ENABLE(1),
		.D_OUT_0(pll_out)
    );

    wire pll_out;
    ipll ipll(.xtal_in(xtal_in), .clk(pll_out));


endmodule
