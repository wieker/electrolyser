module uart_ctl(
    input clk,              // 4..0MHz CPU clock
    input reset,            // Low-true reset

	input RX,				// serial RX

    output [7:0] rx_dat,
    output cfg_we,

    input rx_busy,
);

	// hard-coded bit-rate
	localparam sym_rate = 1200;
    localparam clk_freq = 3000000;
    localparam sym_cnt = clk_freq / sym_rate;
	localparam SCW = $clog2(sym_cnt);

    wire rx_stb;

    reg [7:0] cfg_addr;
    reg stage;
    assign cfg_we = stage && rx_stb && !rx_busy;

    always @(posedge clk)
        begin
            if (stage == 0 && rx_dat == 0x00 && rx_stb) begin
                stage <= 0;
            end
            if (stage == 0 && rx_dat != 0x00 && rx_stb && !rx_busy) begin
                cfg_addr <= rx_dat;
                stage <= 1;
            end
            if (stage) begin
                if (rx_stb) begin
                    stage <= 0;
                end
            end
        end

	acia_rx #(
		.SCW(SCW),				// rate counter width
		.sym_cnt(sym_cnt)		// rate count value
	)
	my_rx(
		.clk(clk),				// system clock
		.rst(reset),			// system reset
		.rx_serial(RX),		    // raw serial input
		.rx_dat(rx_dat),        // received byte
		.rx_stb(rx_stb),        // received data available
		.rx_err(rx_err)         // received data error
	);

endmodule
