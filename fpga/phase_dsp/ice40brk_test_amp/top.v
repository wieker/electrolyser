module top(
    output LED2, LED3, tx_out,
    input lvds_in, xtal_in,
);

    wire clk = xtal_in;
    wire rst;
    osc osc(.clk(clk), .rst(rst));

    wire sig_in;
    wire comp_in;
	digitizer digitizer(.clk(clk), .rst(rst), .lvds_in(lvds_in), .sig(sig_in), .comp_in(comp_in));

	reg [8:0] counter;
	reg ou1;
	reg [8:0] on;

    reg [10:0] tx_counter;
    always@(posedge xtal_in)
    begin
        tx_counter <= tx_counter + 1;
        if (counter[8] == 1) begin
            counter <= 0;
            ou1 <= on[8] | on[7];
            on <= sig_in;
        end else begin
            counter <= counter + 1;
            on <= on + sig_in;
        end
    end

    assign tx_out = ou1;
endmodule
