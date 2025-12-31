module ipll(
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

// $ icepll -i 48 -o 100

// F_PLLIN:    48.000 MHz (given)
// F_PLLOUT:  100.000 MHz (requested)
// F_PLLOUT:  100.000 MHz (achieved)

// FEEDBACK: SIMPLE
// F_PFD:   16.000 MHz
// F_VCO:  800.000 MHz

// DIVR:  2 (4'b0010)
// DIVF: 49 (7'b0110001)
// DIVQ:  3 (3'b011)

// FILTER_RANGE: 1 (3'b001)

// $ icepll -i 48 -o 150

// F_PLLIN:    48.000 MHz (given)
// F_PLLOUT:  150.000 MHz (requested)
// F_PLLOUT:  150.000 MHz (achieved)

// FEEDBACK: SIMPLE
// F_PFD:   24.000 MHz
// F_VCO:  600.000 MHz

// DIVR:  1 (4'b0001)
// DIVF: 24 (7'b0011000)
// DIVQ:  2 (3'b010)

// FILTER_RANGE: 2 (3'b010)




   SB_PLL40_PAD #(
          .FEEDBACK_PATH("SIMPLE"),
          .PLLOUT_SELECT("GENCLK"),
          .DIVR(4'b0000),
          .DIVF(7'b0111100),
          .DIVQ(3'b101),
          .FILTER_RANGE(3'b001),
        ) SB_PLL40_CORE_inst (
          .RESETB(1'b1),
          .BYPASS(1'b0),
          .PLLOUTCORE(clk),
          .PACKAGEPIN(xtal_in)
    );


endmodule
