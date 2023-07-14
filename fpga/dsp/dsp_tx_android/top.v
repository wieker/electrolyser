module top(
    output lvds_in, tx_out, LED1, LED2,
    input xtal_in
);

    reg [27:0] ctr;
    always@(posedge xtal_in)
    begin
      ctr <= ctr + 1;
    end
    wire clk;

   SB_PLL40_CORE #(
          .FEEDBACK_PATH("SIMPLE"),
          .PLLOUT_SELECT("GENCLK"),
          .DIVR(4'b0010),
          .DIVF(7'b0100111),
          .DIVQ(3'b100),
          .FILTER_RANGE(3'b001),
        ) SB_PLL40_CORE_inst (
          .RESETB(1'b1),
          .BYPASS(1'b0),
          .PLLOUTCORE(clk),
          .REFERENCECLK(xtal_in)
    );

    assign lvds_in = ctr[3];
    assign LED1 = ctr[27];
    assign LED2 = !ctr[27];

endmodule
