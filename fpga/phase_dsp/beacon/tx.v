module tx(
    output tx_out, tx_en,
    input xtal_in, tx_stb,
);

	SB_IO #(
		.PIN_TYPE(6'b101001)
	) lp_tx_out (
		.PACKAGE_PIN(tx_out),
		.OUTPUT_ENABLE(tx_en),
		.D_OUT_0(pll_out)
    );

    reg pll_enable;
    reg [10:0] pll_samples;
    wire pll_out;
    ipll ipll(.xtal_in(xtal_in), .clk(pll_out));

    always@(posedge xtal_in)
    begin
        if (pll_samples[10] == 1) begin
            pll_enable <= 0;
            pll_samples <= 0;
        end else if (pll_enable == 1) begin
            pll_samples <= pll_samples + 1;
        end else if (tx_stb == 1) begin
            pll_enable <= 1;
        end
    end

    assign tx_en = pll_samples[8];

endmodule
