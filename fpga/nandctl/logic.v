// tst_6502.v - test 6502 core
// 02-11-19 E. Brombaugh

module logic(
    input clk,              // 4..0MHz CPU clock
    input reset,

	input RX,				// serial RX
	output TX,				// serial TX

    output nand_oe,
    input [7:0] nand_din,
    output [7:0] nand_dout,
    output ce, cle, we, re, ale,

    output led,
);

    flash_ifc flash_ifc(
        .clk(clk),				// system clock

		.nand_oe(nand_oe),
		.nand_din(nand_din),
		.nand_dout(nand_dout),

        .ce(ce),
        .cle(cle),
        .we(we),
        .re(re),
        .ale(ale),

        .uart_out(din),
        .tx_start(tx_start),
    );

	wire [7:0] din;
	wire tx_start;
	wire tx_busy;

	assign led = tx_start;

	localparam sym_rate = 1200;
    localparam clk_freq = 12000000;
    localparam sym_cnt = clk_freq / sym_rate;
	localparam SCW = $clog2(sym_cnt);

	acia_tx #(
        .SCW(SCW),              // rate counter width
        .sym_cnt(sym_cnt)       // rate count value
    )
    my_tx(
        .clk(clk),				// system clock
        .rst(reset),			// system reset
        .tx_dat(din),           // transmit data byte
        .tx_start(tx_start),    // trigger transmission
        .tx_serial(TX),         // tx serial output
        .tx_busy(tx_busy)       // tx is active (not ready)
    );

endmodule
