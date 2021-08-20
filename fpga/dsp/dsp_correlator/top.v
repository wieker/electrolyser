module top(
    output LED1, LED2, fpga_tx, pwm_out,
    input btn1, btn2, lvds_in,
);

    wire clk;
    wire rst;
    osc osc(.clk(clk), .rst(rst));

    wire sig_in;
	digitizer digitizer(.clk(clk), .rst(rst), .lvds_in(lvds_in), .sig(sig_in));
    assign LED1 = sig_in;

    localparam  counter_width = 32;

    reg [counter_width-1:0] ctr;
    reg [counter_width-1:0] counter[12];
    reg [11:0] code;

    wire [7:0] temp[12];
    genvar j;
    for (j=0; j < 2; j++) begin
        assign temp[j] = (counter[j][27:0] == 28'h0400000) ? j + 1 : 0;
    end

    reg [7:0] result;

    integer i;
    always@(posedge clk)
    begin
      ctr <= ctr + 1;
      for(i = 0; i < 2; i = i + 1)
        if (temp[i] == 0)
          counter[i] <= counter[i] + (code[i] == digitizer[1]);
        else
          counter[i] <= 0;

      result <= temp[0] | temp[1];
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
        .tx_dat(8'h2f + result),           // transmit data byte
        .tx_start(result != 0),    // trigger transmission
        .tx_serial(fpga_tx),         // tx serial output
        .tx_busy(tx_busy)       // tx is active (not ready)
    );



  initial begin
      code <= 12'b010101010101;
  end

  assign pwm_out = 0;


endmodule
