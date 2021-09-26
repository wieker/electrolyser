module hex_dump(
    input clk, rst, sig, fpga_rx,
    output fpga_tx, rdy3, rdy4,
);

    wire [7:0] data;
    dispatcher dispatcher(.clk(clk), .rst_in(rst), .sig(sig), .phases(data), .stb(stb));

    reg tx_start;
    reg [7:0] symb;
    reg [7:0] stbcounter;

    always@(posedge clk)
    begin
        tx_start <= stb && (stbcounter[4] == 0);
        symb <= data;
        if (stb && (stbcounter[4] == 0)) begin
            stbcounter <= stbcounter + 1;
        end
    end

    wire tx_busy;
    localparam sym_rate = 1000000;
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