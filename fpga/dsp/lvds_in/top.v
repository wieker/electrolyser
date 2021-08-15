module top(
    output LED1, LED2,
    input btn1, btn2, lvds_in,
);

    wire comp_in;

	SB_IO #(
		.PIN_TYPE(6'b000001),
		.IO_STANDARD("SB_LVDS_INPUT")
	) lp_compare (
		.PACKAGE_PIN(lvds_in),
		.D_IN_0(comp_in)
    );

    assign LED1 = comp_in;

    wire clk;

    SB_HFOSC inthosc (
      .CLKHFPU(1'b1),
      .CLKHFEN(1'b1),
      .CLKHF(clk)
    );

    localparam  counter_width = 32;

    reg [counter_width-1:0] ctr;

    always@(posedge clk)
    begin
      ctr <= ctr + 1;
    end

    assign LED2 = ctr[25];


endmodule
