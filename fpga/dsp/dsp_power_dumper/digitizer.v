module digitizer(
    input clk, rst, lvds_in,
    output sig, comp_in,
);

	SB_IO #(
		.PIN_TYPE(6'b000001),
		.IO_STANDARD("SB_LVDS_INPUT")
	) lp_compare (
		.PACKAGE_PIN(lvds_in),
		.D_IN_0(comp_in)
    );

    reg [1:0] digitizer;


    always@(posedge clk)
    begin
      digitizer <= { digitizer[0], comp_in };
    end

    assign sig = digitizer[1];

endmodule