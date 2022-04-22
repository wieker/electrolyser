module top(
    output LED1, LED2, fpga_tx, pwm_out,
    input btn1, btn2, lvds_in, fpga_rx, xtal_in
);

    wire clk = xtal_in;
    wire rst;
    osc osc(.clk(clk), .rst(rst));

    wire sig_in;
    wire comp_in;
	digitizer digitizer(.clk(clk), .rst(rst), .lvds_in(lvds_in), .sig(sig_in), .comp_in(comp_in));

    wire rdy3, rdy4;
    hex_dump hex_dump(.clk(clk), .rst(rst), .fpga_tx(fpga_tx), .sig(sig_in), .fpga_rx(fpga_rx), .rdy3(rdy3), .rdy4(rdy4));

    reg [7:0] ctr;
    always@(posedge clk)
    begin
      ctr <= ctr + 1;
    end

    assign LED2 = rdy3;
    assign LED1 = rdy4;


    reg [7:0] period;
    reg [7:0] cmp_cntr;
    reg [8:0] mirror;
    reg [7:0] zu;
    always@(posedge clk)
    begin
      period <= period + 1;
      if (period == 0) begin
        cmp_cntr <= sig_in;
        if (cmp_cntr[7] == 1) begin
            mirror <= 8'h80;
        end else if (cmp_cntr[7] == 0) begin
            mirror <= 8'h75;
        end
      end else begin
        cmp_cntr <= cmp_cntr + sig_in;
        mirror <= mirror + 1;
      end
    end
    assign pwm_out = mirror[8];

endmodule
