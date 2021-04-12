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

    reg acia_cs = 0;
    reg acia_we = 0;
    reg acia_reg_sel = 0;
    wire irq;
    reg [7:0] acia_din;
    wire [7:0] acia_do;

    reg [14:0] counter;
    reg [7:0] data;
    reg stb;
    initial
        acia_din <= 8'h56;

    reg [3:0] sram_addr_reg;
    assign addr = { 4'b000, sram_addr_reg };
    reg [15:0] sram_dout_reg;
    assign sram_dout = sram_dout_reg;
    reg sram_oe_reg;
    assign sram_oe = sram_oe_reg;

    always @(posedge clk)
        begin
            counter <= counter + 1;
            if (irq && !stb) begin
                sram_dout_reg <= 8'h50;
                sram_oe_reg <= 1;
                sram_addr_reg <= sram_addr_reg + 1;

                acia_cs <= 1;
                acia_reg_sel <= 1;
                acia_we <= 0;

                stb <= 1;
            end else if (irq && stb) begin
                sram_dout_reg <= acia_do;
                sram_oe_reg <= 1;
                sram_addr_reg <= sram_addr_reg + 1;

                acia_cs <= 0;
                acia_we <= 0;
                acia_reg_sel <= 0;

                stb <= 1;
            end else if (stb) begin
                sram_oe_reg <= 0;

                acia_cs <= 0;
                acia_we <= 0;
                acia_reg_sel <= 0;

                stb <= 0;
            end else if  (counter == 0) begin
                sram_addr_reg <= sram_addr_reg + 1;
                acia_din <= sram_din;

                acia_cs <= 1;
                acia_we <= 1;
                acia_reg_sel <= 1;

                stb <= 1;
            end
        end

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
