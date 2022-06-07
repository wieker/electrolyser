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

    assign LED2 = rf_rx_stb;
    wire rf_rx_stb;
    hex_dump hex_dump(.clk(clk), .rst(rst), .fpga_tx(fpga_tx), .sig(tx_en ? 1 : sig_in), .fpga_rx(uart_rx_stb), .rdy3(rf_rx_stb));

    adjust adjust(.clk(clk), .rst(rst), .pwm_out(pwm_out));

    wire tx_stb;
    wire tx_en;
    tx tx(.xtal_in(xtal_in), .tx_out(tx_out), .tx_stb(tx_counter == (8'hff + mid_counter)), .tx_en(tx_en));

    reg rcvd;
    reg tx_prep;
    reg [7:0] end_counter;
    reg [7:0] mid_counter;
    reg [8:0] tx_counter;
    always@(posedge clk)
    begin
        if (rf_rx_stb) begin
            rcvd <= 1;
            end_counter <= end_counter + 1;
            mid_counter <= end_counter[7:1];
            tx_counter <= tx_counter + 1;
        end else if (tx_counter == (8'hff + mid_counter)) begin
            rcvd <= 0;
            end_counter <= 0;
            tx_counter <= 0;
        end else if (rcvd == 1) begin
            tx_counter <= tx_counter + 1;
        end
    end

    wire uart_rx_stb;
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
