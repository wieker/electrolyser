module hex_dump(
    input clk, rst, sig, fpga_rx,
    output fpga_tx,
);

    wire [7:0] data;
    dispatcher dispatcher(.clk(clk), .rst_in(rst), .sig(sig), .data_out(data));

    reg [1:0] state;
    reg tx_start;
    reg [7:0] symb;
    wire rdy;
    reg [7:0] cmd;

    always@(posedge clk)
    begin
        if (rst) begin
            state <= 0;
        end else if ((rx_stb == 1) && (state == 0)) begin
            cmd <= rx_dat;
            state <= 1;
        end else if ((tx_busy == 0) && (tx_start == 0) && (state == 1)) begin
            tx_start <= 1;
            symb <= data;
            state <= 2;
        end else if (state == 2) begin
            tx_start <= 0;
            state <= 0;
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

    wire rx_stb;
    wire [7:0] rx_dat;

	acia_rx #(
		.SCW(SCW),				// rate counter width
		.sym_cnt(sym_cnt)		// rate count value
	)
	my_rx(
		.clk(clk),				// system clock
		.rst(rst),			// system reset
		.rx_serial(fpga_rx),		    // raw serial input
		.rx_dat(rx_dat),        // received byte
		.rx_stb(rx_stb),        // received data available
		.rx_err(rx_err)         // received data error
	);

endmodule