module hex_dump(
    input clk, rst, input [7:0] rdy,
    output fpga_tx,
);


    genvar j;
    wire rdy_tmp[8];
    wire [7:0] sym_tmp[8];
    for (j=0; j < 8; j++) begin
        if (j == 0)
            assign rdy_tmp[j] = rdy[0];
        else
            assign rdy_tmp[j] = rdy[j] | rdy_tmp[j - 1];
        if (j == 0)
            assign sym_tmp[j] = rdy[0] ? 8'h30 : 0;
        else
            assign sym_tmp[j] = rdy[j] ? 8'h30 + j : sym_tmp[j - 1];
    end

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

endmodule