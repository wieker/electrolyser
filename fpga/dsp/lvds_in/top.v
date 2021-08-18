module top(
    output LED1, LED2, fpga_tx, pwm_out,
    input btn1, btn2, lvds_in,
);

    wire comp_in;
    integer i;

	SB_IO #(
		.PIN_TYPE(6'b000001),
		.IO_STANDARD("SB_LVDS_INPUT")
	) lp_compare (
		.PACKAGE_PIN(lvds_in),
		.D_IN_0(comp_in)
    );

    assign LED1 = digitizer[1];

    wire clk;

    SB_HFOSC inthosc (
      .CLKHFPU(1'b1),
      .CLKHFEN(1'b1),
      .CLKHF(clk)
    );

    localparam  counter_width = 32;

    reg [counter_width-1:0] ctr;
    reg [1:0] digitizer;
    reg [counter_width-1:0] counter[12];
    reg [11:0] code;

    wire [7:0] temp[12];
    assign temp[0] = (counter[0][27:0] == 28'h0400000) ? 1 : 0;
    genvar j;
    for (j=1; j < 12; j++) begin
        assign temp[j] = (counter[j][27:0] == 28'h0400000) ? j + 1 : temp[j - 1];
    end

    reg [7:0] result;

    always@(posedge clk)
    begin
      ctr <= ctr + 1;
      digitizer <= { digitizer[0], comp_in };
      for(i = 0; i < 12; i = i + 1)
        if (result == 0)
          counter[i] <= counter[i] + (code[i] == digitizer[1]);
        else
          counter[i] <= 0;
      if (result == 0)
        code <= { code[10:0], code[11] };
      else
        code <= 12'b111111000000;

      result <= temp[11];
    end

    assign LED2 = ctr[25];

    localparam sym_rate = 1200;
    localparam clk_freq = 48000000;
    localparam sym_cnt = clk_freq / sym_rate;
    localparam SCW = $clog2(sym_cnt);

    acia_tx #(
        .SCW(SCW),              // rate counter width
        .sym_cnt(sym_cnt)       // rate count value
    )
    my_tx(
        .clk(clk),				// system clock
        .rst(ctr[27:0] == 28'h0001000),			// system reset
        .tx_dat(8'h30 + result),           // transmit data byte
        .tx_start(result != 0),    // trigger transmission
        .tx_serial(fpga_tx),         // tx serial output
        .tx_busy(tx_busy)       // tx is active (not ready)
    );



  initial begin
    code = 12'b111111000000;
  end

  assign pwm_out = code[0];


endmodule
