module adjust1(
    input clk, rst,
    output pwm_out,
);

    reg [10:0] period;
    always@(posedge clk)
    begin
        period <= period + 1;
    end
    wire state = period[10:2] == 8'h00;

	SB_IO #(
		.PIN_TYPE(6'b101001)
	) lp_compare (
		.PACKAGE_PIN(pwm_out),
		.OUTPUT_ENABLE(state),
		.D_OUT_0(period[0])
    );

endmodule