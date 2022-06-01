module top(
    output LED1, LED2, fpga_tx, pwm_out, tx_out,
    input btn1, btn2, lvds_in, fpga_rx, xtal_in
);

    wire clk = xtal_in;
    wire rst;
    osc osc(.clk(clk), .rst(rst));

    wire sig_in;
    wire comp_in;
	digitizer digitizer(.clk(clk), .rst(rst), .lvds_in(lvds_in), .sig(sig_in), .comp_in(comp_in));

    wire rdy3, rdy4;
    wire dump_start = rx_stb;
    hex_dump hex_dump(.clk(clk), .rst(rst), .fpga_tx(fpga_tx), .sig(sig_in), .fpga_rx(dump_start), .rdy3(rdy3), .rdy4(rdy4));

    reg [7:0] ctr;
    always@(posedge clk)
    begin
      ctr <= ctr + 1;
    end

    assign LED2 = rdy3;
    assign LED1 = rdy4;


    reg [8:0] period;
    reg [7:0] cmp_cntr;
    reg [7:0] mirror;
    always@(posedge clk)
    begin
        period <= period + 1;
      if (rx_stb == 0) begin
        mirror <= rx_dat;
      end
    end
    wire [9:0] state = period + 9'h001;

	SB_IO #(
		.PIN_TYPE(6'b101001)
	) lp_compare (
		.PACKAGE_PIN(pwm_out),
		.OUTPUT_ENABLE(state[9]),
		.D_OUT_0(1)
    );

	SB_IO #(
		.PIN_TYPE(6'b101001)
	) lp_tx_out (
		.PACKAGE_PIN(tx_out),
		.OUTPUT_ENABLE(pll_enable),
		.D_OUT_0(pll_out)
    );

    reg pll_enable;
    wire pll_out;
    ipll ipll(.xtal_in(xtal_in), .clk(pll_out));

    wire [7:0] rx_dat;
    wire rx_stb;
    reg got;
    reg [10:0] counter;

    reg [9:0] pll_samples;
    reg pll_lock;
    reg [10:0] free_pll;
    always@(posedge clk)
    begin
        if (rdy3 && !got && !pll_lock) begin
            got <= 1;
            counter <= 0;
        end else if (got) begin
            counter <= counter + 1;
        end
        if (counter[10] && got) begin
            got <= 0;
            pll_enable <= 1;
            pll_lock <= 1;
        end
        period <= period + 1;
        if (pll_samples[9] == 1 && pll_samples[8] == 1) begin
            pll_enable <= 0;
            pll_samples <= 0;
        end else if (rx_stb == 1) begin
        end else if (pll_enable == 1) begin
            pll_samples <= pll_samples + 1;
        end
        if (pll_lock && free_pll[10]) begin
            pll_lock <= 0;
            free_pll <= 0;
        end else if (pll_lock) begin
            free_pll <= free_pll + 1;
        end
    end


    localparam sym_rate = 1200;
    localparam clk_freq = 48000000;
    localparam sym_cnt = clk_freq / sym_rate;
    localparam SCW = $clog2(sym_cnt);
    acia_rx #(
        .SCW(SCW),              // rate counter width
        .sym_cnt(sym_cnt)       // rate count value
    )
    my_rx(
        .clk(clk),
        .rst(rst),
        .rx_serial(fpga_rx),
        .rx_dat(rx_dat),
        .rx_stb(rx_stb),
        .rx_err(tx_busy)
    );

endmodule
