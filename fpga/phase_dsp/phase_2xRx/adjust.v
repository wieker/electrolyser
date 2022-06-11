module adjust(
    input clk, rst,
    output pwm_out,
);

    reg [9:0] period;
    always@(posedge clk)
    begin
        period <= period + 1;
    end
    wire [10:0] state = period + 10'h001;

	SB_IO #(
		.PIN_TYPE(6'b101001)
	) lp_compare (
		.PACKAGE_PIN(pwm_out),
		.OUTPUT_ENABLE(state[10]),
		.D_OUT_0(0)
    );

endmodule