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
    output [18:0] addr,
);

    wire cfg_we;
    wire [7:0] rx_dat;
    wire [7:0] cfg_addr;

    always @(posedge clk)
        begin
            if (cfg_we) begin
                if (cfg_addr == 8'h01) begin
                    gpio_o <= rx_dat;
                end
            end
        end

    uart_ctl uart_ctl(
        .clk(clk),				// system clock
        .reset(reset),			// system reset
        .RX(RX),		    // raw serial input

        .rx_dat(rx_dat),        // received byte
        .cfg_we(cfg_we),        // received data available

        .rx_busy(0),         // received data error
        .cfg_addr(cfg_addr)         // received data error
    );
	
endmodule
