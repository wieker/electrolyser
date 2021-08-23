module hex_dump(
    input clk, rst, sig,
    output fpga_tx,
);

    wire [7:0] data;
    dispatcher dispatcher(.clk(clk), .rst(rst), .sig(sig), .addr_in(state), .data_out(data));

    reg state;
    reg tx_start;
    reg [7:0] symb;
    wire rdy;

    always@(posedge clk)
    begin
        if (rst) begin
            state <= 0;
        end else if ((tx_busy == 0) && (tx_start == 0)) begin
            tx_start <= 1;
            symb <= data;
            state <= state + 1;
        end else begin
            tx_start <= 0;
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
        .tx_dat(symb),           // transmit data byte
        .tx_start(tx_start),    // trigger transmission
        .tx_serial(fpga_tx),         // tx serial output
        .tx_busy(tx_busy)       // tx is active (not ready)
    );

endmodule