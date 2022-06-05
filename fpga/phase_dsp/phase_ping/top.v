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
    hex_dump hex_dump(.clk(clk), .rst(rst), .fpga_tx(fpga_tx), .sig(tx_en ? 1 : sig_in), .fpga_rx(total_counter[17] == 1 && process), .rdy3(rf_rx_stb),
        .rx_counter(rx_counter[19:4]));

    adjust adjust(.clk(clk), .rst(rst), .pwm_out(pwm_out));

    wire tx_stb;
    wire tx_en;
    tx tx(.xtal_in(xtal_in), .tx_out(tx_out), .tx_stb(process && total_counter[11:0] == 0), .tx_en(tx_en));

    reg process;
    reg [24:0] total_counter;
    reg alg;
    reg [23:0] rx_counter;
    always@(posedge clk)
    begin
        if (uart_rx_stb) begin
            alg <= 1;
            process <= 1;
            rx_counter <= 0;
            total_counter <= 0;
        end else if (process && total_counter[11:0] == 0) begin
            alg <= 1;
        end else if (rf_rx_stb) begin
            alg <= 0;
        end else if (alg) begin
            rx_counter <= rx_counter + 1;
        end
        if (process) begin
            if (total_counter[17] == 1) begin
                process <= 0;
            end else begin
                total_counter <= total_counter + 1;
            end
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
