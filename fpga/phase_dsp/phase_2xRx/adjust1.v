module adjust1(
    input clk, rst,
    output pwm_out,
);

    reg [10:0] period;
    always@(posedge clk)
    begin
        period <= period + 1;
    end
    wire [11:0] state = period + 11'h026;

	SB_IO #(
		.PIN_TYPE(6'b101001)
	) lp_compare (
		.PACKAGE_PIN(pwm_out),
		.OUTPUT_ENABLE(state[11]),
		.D_OUT_0(0)
    );

endmodule