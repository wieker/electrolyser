module top(
    output LED1, LED2, fpga_tx, pwm_out,
    input btn1, btn2, lvds_in, fpga_rx, xtal_in,
    output SPI_SCK, output SPI_SS, output SPI_MOSI, input SPI_MISO,
);

    wire clk = xtal_in;
    wire rst;
    osc osc(.clk(clk), .rst(rst));

    wire sig_in;
    wire comp_in;
	digitizer digitizer(.clk(clk), .rst(rst), .lvds_in(lvds_in), .sig(sig_in), .comp_in(comp_in));

    wire rdy3, rdy4;
    hex_dump hex_dump(.clk(clk), .rst(rst), .fpga_tx(fpga_tx), .sig(sig_in), .fpga_rx(fpga_rx), .rdy3(rdy3), .rdy4(rdy4),
        .SPI_SCK(SPI_SCK), .SPI_SS(SPI_SS), .SPI_MOSI(SPI_MISO), .SPI_MISO(SPI_MOSI));

    reg [7:0] ctr;
    always@(posedge clk)
    begin
      ctr <= ctr + 1;
    end

    assign LED2 = rdy3;
    assign LED1 = rdy4;

    assign pwm_out = 0;

endmodule
