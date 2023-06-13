module top(
    output LED1, LED2, LED3, LED4,
    input lvds_in, xtal_in,
);

    wire clk = xtal_in;
    wire rst;
    osc osc(.clk(clk), .rst(rst));

    wire sig_in;
    wire comp_in;
	digitizer digitizer(.clk(clk), .rst(rst), .lvds_in(lvds_in), .sig(sig_in), .comp_in(comp_in));

	reg [7:0] counter;

    reg [10:0] tx_counter;
    always@(posedge xtal_in)
    begin
        tx_counter <= tx_counter + 1;
    end

    assign LED2 = sig_in;
    assign LED2 = !sig_in;
endmodule
