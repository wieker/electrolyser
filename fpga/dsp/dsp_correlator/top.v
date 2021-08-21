module top(
    output LED1, LED2, fpga_tx, pwm_out,
    input btn1, btn2, lvds_in,
);

    wire clk;
    wire rst;
    osc osc(.clk(clk), .rst(rst));

    wire sig_in;
	digitizer digitizer(.clk(clk), .rst(rst), .lvds_in(lvds_in), .sig(sig_in));

    wire codes[8];
    wire [7:0] rdy;
    genvar j;
    for (j=0; j < 8; j++) begin
	    sig_source sig_source(.clk(clk), .rst(rst), .period0(16), .period1(16), .phase(j * 4), .start_code(0), .code(codes[j]));
        correlator correlator(.clk(clk), .rst(rst), .sig(sig_in), .code(codes[j]), .rdy(rdy[j]));
    end

    hex_dump hex_dump(.clk(clk), .rst(rst), .rdy(rdy), .fpga_tx(fpga_tx));

    reg [32:0] ctr;
    always@(posedge clk)
    begin
      ctr <= ctr + 1;
    end

    assign LED2 = ctr[25];
    assign LED1 = sig_in;

    assign pwm_out = 0;


endmodule
