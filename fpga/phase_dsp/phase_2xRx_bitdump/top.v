module top(
    output LED1, LED2, fpga_tx, pwm_out, tx_out, pwm_out1,
    input btn1, btn2, lvds_in, fpga_rx, xtal_in, lvds_in1,
    input SPI_SCK, input SPI_SS, input SPI_MOSI, output SPI_MISO,
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
    hex_dump hex_dump(.clk(clk), .rst(rst), .fpga_tx(fpga_tx), .sig(tx_en ? 1 : sig_in), .sig1(tx_en ? 1 : sig_in1),
        .next(next), .pause(!SPI_SS), .byte(byte), .mode(mode));

    wire oe;
    wire prm;
    adjust adjust(.clk(clk), .rst(rst), .pwm_out(pwm_out), .oe(oe), .sig(sig_in), .param(param), .prm(prm));
    adjust1 adjust1(.clk(clk), .rst(rst), .pwm_out(pwm_out1));
    //assign pwm_out = sig_in;
    //assign pwm_out1 = sig_in1;

    wire tx_stb;
    wire tx_en;
    tx tx(.xtal_in(xtal_in), .tx_out(tx_out), .tx_stb(0), .tx_en(tx_en));

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

    reg spi_state;
    reg [7:0] param;
    reg [7:0] addr;
    reg [7:0] echo;
    wire [7:0] byte;
    reg next;
    reg pause;
    reg mode;
    always@(posedge clk)
    begin
        if (SPI_SS == 1) begin
            spi_state <= 0;
            pause <= 0;
        end else if (spi_valid == 1 && spi_state == 0) begin
            spi_state <= 1;
            addr <= spi_data;
            if (spi_data == 8'h57) begin
                pause <= 1;
            end
        end else if (spi_valid == 1 && addr == 8'h55 && spi_state == 1) begin
            param <= spi_data;
            spi_state <= 0;
        end else if (spi_valid == 1 && addr == 8'h58 && spi_state == 1) begin
            mode <= spi_data[7];
            spi_state <= 0;
        end else if (spi_valid == 1 && addr == 8'h56 && spi_state == 1) begin
            echo <= spi_data;
        end else if (spi_valid == 1 && addr == 8'h57 && spi_state == 1) begin
            echo <= byte;
            next <= 1;
        end else if (next == 1 && addr == 8'h57 && spi_state == 1) begin
            next <= 0;
        end
    end

    assign LED1 = prm;
    assign LED2 = oe;

    wire spi_valid;
    wire [7:0] spi_data;
    spi_slave spi_slave
      (
        .i_Clk(clk),
        .i_Rst_L(~rst),

        .i_SPI_Clk(SPI_SCK),
        .o_SPI_MISO(SPI_MISO),
        .i_SPI_MOSI(SPI_MOSI),
        .i_SPI_CS_n(SPI_SS),

        .o_RX_DV(spi_valid),
        .o_RX_Byte(spi_data),
        .i_TX_DV(spi_valid),
        .i_TX_Byte(echo),

       );

endmodule
