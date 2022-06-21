module adjust1(
    input clk, rst,
    output pwm_out,
);

    reg [12:0] period;
    always@(posedge clk)
    begin
        period <= period + 1;
    end
    wire [13:0] state = period + 13'h007;

	SB_IO #(
		.PIN_TYPE(6'b101001)
	) lp_compare (
		.PACKAGE_PIN(pwm_out),
		.OUTPUT_ENABLE(state[13]),
		.D_OUT_0(1)
    );

endmodule