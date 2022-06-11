module top(
    output LED1, LED2, fpga_tx, pwm_out, tx_out, pwm_out1,
    input btn1, btn2, lvds_in, fpga_rx, xtal_in, lvds_in1,
);

    wire clk = xtal_in;
    wire rst;
    osc osc(.clk(clk), .rst(rst));

    wire sig_in;
    wire comp_in;
    wire sig_in1;
    wire comp_in1;
	digitizer digitizer(.clk(clk), .rst(rst), .lvds_in(lvds_in), .sig(sig_in), .comp_in(comp_in));
	digitizer digitizer1(.clk(clk), .rst(rst), .lvds_in(lvds_in1), .sig(sig_in1), .comp_in(comp_in1));

    assign LED2 = rf_rx_stb;
    //assign LED1 = process;
    wire rf_rx_stb;
    hex_dump hex_dump(.clk(clk), .rst(rst), .fpga_tx(fpga_tx), .sig(tx_en ? 1 : sig_in), .sig1(tx_en ? 1 : sig_in1), .fpga_rx(uart_rx_stb));

    adjust adjust(.clk(clk), .rst(rst), .pwm_out(pwm_out));
    adjust1 adjust1(.clk(clk), .rst(rst), .pwm_out(pwm_out1));

    wire tx_stb;
    wire tx_en;
    tx tx(.xtal_in(xtal_in), .tx_out(tx_out), .tx_stb(1), .tx_en(tx_en));

    wire uart_rx_stb;
    wire [8:0] rx_dat;
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
        .rx_stb(uart_rx_stb),
        .rx_err(tx_busy)
    );

endmodule
