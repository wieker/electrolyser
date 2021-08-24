module top(
    output LED1, LED2, fpga_tx, pwm_out,
    input btn1, btn2, lvds_in, fpga_rx,
);

    wire clk;
    wire rst;
    osc osc(.clk(clk), .rst(rst));

    wire sig_in;
	digitizer digitizer(.clk(clk), .rst(rst), .lvds_in(lvds_in), .sig(sig_in));

    hex_dump hex_dump(.clk(clk), .rst(rst), .fpga_tx(fpga_tx), .sig(sig_in), .fpga_rx(fpga_rx));

    reg [32:0] ctr;
    always@(posedge clk)
    begin
      ctr <= ctr + 1;
    end

    assign LED2 = ctr[25];
    assign LED1 = sig_in;

    assign pwm_out = 0;

endmodule
