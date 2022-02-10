module osc(
    input xtal_in, ctl1, ctl2,
    output rst, clk,
);


   SB_PLL40_CORE #(
          .FEEDBACK_PATH("SIMPLE"),
          .PLLOUT_SELECT("GENCLK"),
          .DIVR(4'b0010),
          .DIVF(7'b0110001),
          .DIVQ(3'b010),
          .FILTER_RANGE(3'b001),
        ) SB_PLL40_CORE_inst (
          .RESETB(1'b1),
          .BYPASS(1'b0),
          .PLLOUTCORE(clk),
          .REFERENCECLK(xtal_in)
    );

    reg [3:0] startup;
    assign rst = !startup[3] || ctl1 || !ctl2;

    SB_HFOSC inthosc (
      .CLKHFPU(1'b1),
      .CLKHFEN(1'b1),
      .CLKHF(ttt)
    );

    always@(posedge xtal_in)
    begin
      if (rst)
        startup <= startup + 1;
    end

endmodule