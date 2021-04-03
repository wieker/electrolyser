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

    wire acia_cs = 1;
    wire acia_we = 1;
    wire reg_sel = 1;
    wire irq;
    wire [7:0] acia_din = 8'h56;
    wire [7:0] acia_do;

    always @(posedge clk)
    begin

    end

	wire [7:0] acia_do;
	acia uacia(
		.clk(clk),				// system clock
		.rst(reset),			// system reset
		.cs(acia_cs),				// chip select
		.we(acia_we),			// write enable
		.rs(acia_reg_sel),			// register select
		.rx(RX),				// serial receive
		.din(acia_din),			// data bus input
		.dout(acia_do),			// data bus output
		.tx(TX),				// serial transmit
		.irq(irq)			// interrupt request
	);
	
endmodule
