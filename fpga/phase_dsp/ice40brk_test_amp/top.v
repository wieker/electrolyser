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

//    $ icepll -o 20 -i 12

//    F_PLLIN:    12.000 MHz (given)
//    F_PLLOUT:   20.000 MHz (requested)
//    F_PLLOUT:   19.875 MHz (achieved)

//    FEEDBACK: SIMPLE
//    F_PFD:   12.000 MHz
//    F_VCO:  636.000 MHz

//    DIVR:  0 (4'b0000)
//    DIVF: 52 (7'b0110100)
//    DIVQ:  5 (3'b101)

//    FILTER_RANGE: 1 (3'b001)

   SB_PLL40_CORE #(
          .FEEDBACK_PATH("SIMPLE"),
          .PLLOUT_SELECT("GENCLK"),
          .DIVR(4'b0000),
          .DIVF(7'b0110100),
          .DIVQ(3'b101),
          .FILTER_RANGE(3'b001),
        ) SB_PLL40_CORE_inst (
          .RESETB(1'b1),
          .BYPASS(1'b0),
          .PLLOUTCORE(clk),
          .REFERENCECLK(xtal_in)
    );

    assign tx_out = ou1;
endmodule
