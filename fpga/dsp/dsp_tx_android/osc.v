module osc(
    input xtal_in,
    output rst, clk,
);


// $ icepll -i 48

// F_PLLIN:    48.000 MHz (given)
// F_PLLOUT:   60.000 MHz (requested)
// F_PLLOUT:   60.000 MHz (achieved)

// FEEDBACK: SIMPLE
// F_PFD:   48.000 MHz
// F_VCO:  960.000 MHz

// DIVR:  0 (4'b0000)
// DIVF: 19 (7'b0010011)
// DIVQ:  4 (3'b100)

// FILTER_RANGE: 4 (3'b100)


   SB_PLL40_CORE #(
          .FEEDBACK_PATH("SIMPLE"),
          .PLLOUT_SELECT("GENCLK"),
          .DIVR(4'b0000),
          .DIVF(7'b0010011),
          .DIVQ(3'b100),
          .FILTER_RANGE(3'b100),
        ) SB_PLL40_CORE_inst (
          .RESETB(1'b1),
          .BYPASS(1'b0),
          .PLLOUTCORE(clk),
          .REFERENCECLK(xtal_in)
    );

    reg [3:0] startup;
    assign rst = !startup[3];

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