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
    output [18:0] sram_addr,
);


    reg [18:0] sram_addr_reg;
    reg [7:0] command;
    reg [7:0] len;

    wire cfg_we;
    wire [7:0] rx_dat;
    wire [7:0] cfg_addr;
    wire rx_stb;
    reg sram_oe_stage;

    always @(posedge clk)
        begin
            if (cfg_we) begin
                if (cfg_addr == 8'h01) begin
                    gpio_o <= rx_dat;
                end
                if (cfg_addr == 8'h02) begin
                    sram_addr_reg <= { rx_dat[2:0] , sram_addr_reg[15:8] , sram_addr_reg[7:0] };
                end
                if (cfg_addr == 8'h03) begin
                    sram_addr_reg <= { sram_addr_reg[18:16] , rx_dat , sram_addr_reg[7:0] };
                end
                if (cfg_addr == 8'h04) begin
                    sram_addr_reg <= { sram_addr_reg[18:16] , sram_addr_reg[15:8] , rx_dat };
                end
                if (cfg_addr == 8'h05) begin
                    len <= rx_dat;
                end
                if (cfg_addr == 8'h06) begin
                    command <= rx_dat;
                end
            end else begin
                if (command == 8'h52 && !tx_busy) begin
                    len <= len - 1;
                    if (len == 1) begin
                        command <= 0;
                    end
                    sram_addr_reg <= sram_addr_reg + 1;
                end
                if (command == 8'h53 && !tx_busy) begin
                    len <= len - 1;
                    if (len == 1) begin
                        command <= 0;
                    end
                end
                if (command == 8'h57 && rx_stb) begin
                    sram_oe_stage <= 1;
                end
                if (command == 8'h57 && sram_oe_stage) begin
                    len <= len - 1;
                    if (len == 1) begin
                        command <= 0;
                    end
                    sram_addr_reg <= sram_addr_reg + 1;
                    sram_oe_stage <= 0;
                end
            end
        end

    uart_ctl uart_ctl(
        .clk(clk),				// system clock
        .reset(reset),			// system reset
        .RX(RX),		    // raw serial input

        .rx_dat(rx_dat),        // received byte
        .cfg_we(cfg_we),        // received data available

        .rx_busy((command == 8'h57) || (command == 8'h52) || (command == 8'h53)),         // received data error
        .cfg_addr(cfg_addr),         // received data error

        .rx_stb(rx_stb)
    );

    // RX
    assign sram_oe = (command == 8'h57) && rx_stb;

    // TX
    assign sram_addr = sram_addr_reg;
    assign sram_dout = rx_dat;

	wire [7:0] din;
	wire tx_start;
	wire tx_busy;

	assign din = (command == 8'h52) ? sram_din : 8'h53;
	assign tx_start = ((command == 8'h52) || (command == 8'h53));

	localparam sym_rate = 1200;
    localparam clk_freq = 3000000;
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
