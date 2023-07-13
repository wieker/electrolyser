module top(
    output LED2, LED3, tx_out,
    input lvds_in, xtal_in,
);

    wire clk;
    wire rst;
    osc osc(.clk(clk), .rst(rst));

    wire sig_in;
    wire comp_in;
	digitizer digitizer(.clk(clk), .rst(rst), .lvds_in(lvds_in), .sig(sig_in), .comp_in(comp_in));

	reg [8:0] counter;
	reg ou1;
	reg [8:0] on;

    reg [10:0] tx_counter;
    always@(posedge clk)
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

   SB_PLL40_PAD #(
          .FEEDBACK_PATH("SIMPLE"),
          .PLLOUT_SELECT("GENCLK"),
          .DIVR(4'b0000),
          .DIVF(7'b0111111),
          .DIVQ(3'b011),
          .FILTER_RANGE(3'b001),
        ) SB_PLL40_CORE_inst (
          .RESETB(1'b1),
          .BYPASS(1'b0),
          .PLLOUTCORE(clk),
          .PACKAGEPIN(xtal_in)
    );

    assign tx_out = ou1;


    wire [7:0] value;
    dispatcher dispatcher(.clk(clk), .rst_in(rst), .sig(sig), .value(value));
    assign LED2 = (value[6] == 1);
    assign LED3 = (value[6] == 1) && (value[5] == 1);
endmodule
