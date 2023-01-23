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

    nco i_nco(.clk(xtal_in), .rst(rst), .control_word(16'h2D00), .i_code(clk), .phase_control_word(16'h0000));


    //assign clk = dvdd[1];


   SB_PLL40_CORE #(
          .FEEDBACK_PATH("SIMPLE"),
          .PLLOUT_SELECT("GENCLK"),
          .DIVR(4'b0001),
          .DIVF(7'b0011000),
          .DIVQ(3'b010),
          .FILTER_RANGE(3'b010),
        ) SB_PLL40_CORE_inst (
          .RESETB(1'b1),
          .BYPASS(1'b0),
          //.PLLOUTCORE(clk),
          .REFERENCECLK(xtal_in)
    );

    reg [3:0] startup;
    assign rst = !startup[3];

    SB_HFOSC inthosc (
      .CLKHFPU(1'b1),
      .CLKHFEN(1'b1),
      .CLKHF(ttt)
    );
    reg [3:0] dvdd;

    always@(posedge xtal_in)
    begin
        dvdd <= dvdd + 1;
      if (rst)
        startup <= startup + 1;
    end

endmodule