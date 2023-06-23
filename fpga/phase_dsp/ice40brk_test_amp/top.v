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

	reg [9:0] counter;
	reg ou1, ou2;
	reg [9:0] on;

    reg [10:0] tx_counter;
    always@(posedge xtal_in)
    begin
        tx_counter <= tx_counter + 1;
        if (counter[9] == 1 && counter[8] == 1) begin
            counter <= 0;
            ou1 <= on[7] | on[8] | on[9];
            ou2 <= on[8] | on[9];
            on <= sig_in;
        end else begin
            counter <= counter + 1;
            on <= on + sig_in;
        end
    end

    assign LED2 = ou1;
    assign LED3 = ou2;
endmodule
