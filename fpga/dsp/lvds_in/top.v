module top(
    output LED1, LED2, fpga_tx,
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
    reg [1:0] digitizer;
    reg [counter_width-1:0] counter[12];

    always@(posedge clk)
    begin
      ctr <= ctr + 1;
      digitizer <= { digitizer[0], comp_in };
      counter[digitizer[1]] ++;
    end

    assign LED2 = ctr[25];

    localparam sym_rate = 1200;
    localparam clk_freq = 48000000;
    localparam sym_cnt = clk_freq / sym_rate;
    localparam SCW = $clog2(sym_cnt);

    acia_tx #(
        .SCW(SCW),              // rate counter width
        .sym_cnt(sym_cnt)       // rate count value
    )
    my_tx(
        .clk(clk),				// system clock
        .rst(ctr[27:0] == 28'h0001000),			// system reset
        .tx_dat(counter[0][27:0] == 28'h4000000 ? 8'h53 : 8'h54),           // transmit data byte
        .tx_start((counter[0][27:0] == 28'h4000000) || (counter[1][27:0] == 28'h4000000)),    // trigger transmission
        .tx_serial(fpga_tx),         // tx serial output
        .tx_busy(tx_busy)       // tx is active (not ready)
    );


endmodule
