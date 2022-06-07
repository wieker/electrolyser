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
    assign LED1 = process;
    wire rf_rx_stb;
    hex_dump hex_dump(.clk(clk), .rst(rst), .fpga_tx(fpga_tx), .sig(tx_en ? 1 : sig_in), .fpga_rx(total_counter[16] == 1 && process), .rdy3(rf_rx_stb),
        .rx_counter(svd));

    adjust adjust(.clk(clk), .rst(rst), .pwm_out(pwm_out));

    wire tx_stb;
    wire tx_en;
    tx tx(.xtal_in(xtal_in), .tx_out(tx_out), .tx_stb(process && total_counter[11:0] == 0), .tx_en(tx_en));

    reg process;
    reg [24:0] total_counter;
    reg alg;
    reg [24:0] rx_counter;
    reg [24:0] begin_counter;
    reg [24:0] end_counter;
    reg [23:0] svd;
    reg rcvd;
    reg [24:0] tcb;
    reg [24:0] tce;
    always@(posedge clk)
    begin
        if (rst) begin
            process <= 0;
        end else if (uart_rx_stb) begin
            alg <= 1;
            process <= 1;
            rx_counter <= 0;
            total_counter <= 0;
        end else if (total_counter[16] == 1) begin
            rx_counter <= 0;
            svd <= tcb[15:4] + tce[15:4];
            total_counter <= 0;
            tcb <= 0;
            tce <= 0;
            alg <= 1;
        end else if (process && total_counter[11:0] == 0) begin
            alg <= 1;
            rx_counter <= 0;
            end_counter <= 0;
            begin_counter <= 0;
            rcvd <= 0;
            if (rcvd) begin
                tcb <= tcb + begin_counter;
                tce <= tce + end_counter;
            end
        end else if (rf_rx_stb) begin
            if (alg) begin
                alg <= 0;
                begin_counter <= rx_counter;
            end
            end_counter <= rx_counter;
            rcvd <= 1;
            rx_counter <= rx_counter + 1;
        end else begin
            rx_counter <= rx_counter + 1;
        end
        if (process && total_counter[16] == 0) begin
            total_counter <= total_counter + 1;
        end
    end

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
