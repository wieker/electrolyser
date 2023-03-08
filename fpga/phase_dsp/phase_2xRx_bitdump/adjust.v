module adjust(
    input clk, rst, sig,
    output pwm_out,
    output oe,
    input [7:0] param,
);

    reg [10:0] period;
    always@(posedge clk)
    begin
        period <= period + 1;
    end
    reg [10:0] z_o_counter;
    always@(posedge clk)
    begin
        if (period == 0) begin
            z_o_counter <= 0;
        end else begin
            z_o_counter <= z_o_counter + sig;
        end
    end
    wire [11:0] state = period + {4'b0000, param[6:0]};
    assign oe = state[11];

	SB_IO #(
		.PIN_TYPE(6'b101001)
	) lp_compare (
		.PACKAGE_PIN(pwm_out),
		.OUTPUT_ENABLE(oe),
		.D_OUT_0(param[7])
    );

endmodule