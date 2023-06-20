module top(
    output LED2, LED3,
    input lvds_in, xtal_in,
);

    wire clk = xtal_in;
    wire rst;
    osc osc(.clk(clk), .rst(rst));

    wire sig_in;
    wire comp_in;
	digitizer digitizer(.clk(clk), .rst(rst), .lvds_in(lvds_in), .sig(sig_in), .comp_in(comp_in));

	reg [7:0] counter;
	reg ou;
	reg [7:0] on;

    reg [10:0] tx_counter;
    always@(posedge xtal_in)
    begin
        tx_counter <= tx_counter + 1;
        counter <= counter + 1;
        if (counter == 0) begin
            ou <= on[7];
            on <= sig_in;
        end else begin
            on <= on + sig_in;
        end
    end

    assign LED2 = ou;
    assign LED3 = !ou;
endmodule
