// tst_6502.v - test 6502 core
// 02-11-19 E. Brombaugh

module logic(
    input clk,              // 4..0MHz CPU clock
    input reset,            // Low-true reset
    
	output reg [7:0] gpio_o,
	input [7:0] gpio_i,
	
	input RX,				// serial RX
	output TX,				// serial TX

    output sram_oe,
    input [7:0] sram_din,
    output [7:0] sram_dout,
    output [15:0] addr,
);

	// hard-coded bit-rate
	localparam sym_rate = 1200;
    localparam clk_freq = 32768;
    localparam sym_cnt = clk_freq / sym_rate;
	localparam SCW = $clog2(sym_cnt);

    reg [14:0] counter;

    reg [3:0] sram_addr_reg;
    assign addr = { 4'b000, sram_addr_reg };
    reg [15:0] sram_dout_reg;
    assign sram_dout = sram_dout_reg;
    reg sram_oe_reg;
    assign sram_oe = sram_oe_reg;

    wire rx_stb;
    wire [7:0] rx_dat;

    always @(posedge clk)
        begin
            counter <= counter + 1;
            if (rx_stb) begin
                sram_dout_reg <= rx_dat;
                sram_oe_reg <= 1;
                sram_addr_reg <= sram_addr_reg + 1;
            end else if  (counter != 0) begin
                sram_oe_reg <= 0;
            end else if  (counter == 0) begin
                sram_oe_reg <= 0;
                sram_addr_reg <= sram_addr_reg + 1;
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

	wire [7:0] din;
	wire tx_start;

	assign din = sram_din;
	assign tx_start = counter == 0;

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
