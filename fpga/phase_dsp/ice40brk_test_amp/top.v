module top(
    output LED1, LED2, LED3, LED4, tx_out,
    input lvds_in, xtal_in,
);

    wire clk = xtal_in;
    wire rst;
    osc osc(.clk(clk), .rst(rst));

    wire sig_in;
    wire comp_in;
	digitizer digitizer(.clk(clk), .rst(rst), .lvds_in(lvds_in), .sig(sig_in), .comp_in(comp_in));

	wire[7:0] ones;
	wire[7:0] zeros;
	correlator one(.clk(clk), .rst(rst), .code(1), .sig(sig_in), .value(ones));
	correlator zero(.clk(clk), .rst(rst), .code(0), .sig(sig_in), .value(zeros));

	assign LED1 = ones[7];
	assign LED2 = ones[6];
	assign LED3 = ones[5];
	assign LED4 = ones[4];

    reg [10:0] tx_counter;
    always@(posedge xtal_in)
    begin
        tx_counter <= tx_counter + 1;
    end

    assign tx_out = tx_counter[3];
endmodule
