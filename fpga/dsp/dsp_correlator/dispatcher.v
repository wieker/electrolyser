module dispatcher(
    input clk, rst, sig,
    output fpga_tx,
);

    wire codes[8];
    wire [7:0] rdy;
    wire [7:0] results[8];
    wire ready = rdy_tmp[7];
    genvar j;
    for (j=0; j < 8; j++) begin
	    sig_source sig_source(.clk(clk), .rst(rst | rdy_tmp[7]), .period0(48 * 1024), .period1(48*1024), .phase(j * 6 * 1024), .start_code(0), .code(codes[j]));
        correlator correlator(.clk(clk), .rst(rst | rdy_tmp[7]), .sig(sig_in), .code(codes[j]), .rdy(rdy[j]), .result(results[j]));
    end

    reg [2:0] state;
    wire [2:0] next_state;
    for (j=0; j < 8; j++) begin
	    if ((state == j) && ready && !tx_busy) begin
	        assign next_state == j + 1;
	    end
    end

    wire tx_busy;
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
        .rst(rst),			// system reset
        .tx_dat(sym_tmp[7]),           // transmit data byte
        .tx_start(rdy_tmp[7]),    // trigger transmission
        .tx_serial(fpga_tx),         // tx serial output
        .tx_busy(tx_busy)       // tx is active (not ready)
    );

    genvar j;
    wire rdy_tmp[8];
    for (j=0; j < 8; j++) begin
        if (j == 0)
            assign rdy_tmp[j] = rdy[0];
        else
            assign rdy_tmp[j] = rdy[j] | rdy_tmp[j - 1];
    end

endmodule