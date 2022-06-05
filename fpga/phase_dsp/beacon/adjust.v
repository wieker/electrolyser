module adjust(
    input clk, rst,
    output pwm_out,
);

    reg [8:0] period;
    always@(posedge clk)
    begin
        period <= period + 1;
    end
    wire [9:0] state = period + 9'h179;

	SB_IO #(
		.PIN_TYPE(6'b101001)
	) lp_compare (
		.PACKAGE_PIN(pwm_out),
		.OUTPUT_ENABLE(!state[9]),
		.D_OUT_0(0)
    );

endmodule