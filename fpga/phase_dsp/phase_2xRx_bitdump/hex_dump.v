module hex_dump(
    input clk, rst, sig, sig1, pause, next,
    output fpga_tx, output reg [7:0] byte,
);

    reg [15:0] value;
    reg [15:0] ram_data_in;
    reg [3:0] cntr;
    reg ram_we;

    always@(posedge clk)
    begin
        if (!rst && !pause) begin
            cntr <= cntr + 1;
            value <= {value[14:0], sig};
            if (cntr == 0) begin
                ram_data_in <= value;
                ram_we <= 1;
                ram_wr_addr <= ram_wr_addr + 1;
            end else begin
                ram_we <= 0;
            end
        end
    end

    reg [7:0] ram_wr_addr;
    reg [7:0] ram_rd_addr;
    wire [15:0] ram_data_out;

    SB_RAM40_4K SB_RAM40_4K_inst (
        .RDATA(ram_data_out),
        .RADDR(ram_rd_addr),
        .RCLK(clk),
        .RCLKE(1),
        .RE(1),
        .WADDR(ram_wr_addr),
        .WCLK(clk),
        .WCLKE(1),
        .WDATA(ram_data_in),
        .WE(ram_we)
    );

    reg tx_start;
    reg [7:0] touart;
    reg part;

    always@(posedge clk)
    begin
        if (next && !part) begin
            part <= 1;
            byte <= ram_data_out[15:8];
        end else if (next && part) begin
            part <= 0;
            ram_rd_addr <= ram_rd_addr + 1;
            byte <= ram_data_out[7:0];
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
        .tx_dat(touart),           // transmit data byte
        .tx_start(0),    // trigger transmission
        .tx_serial(fpga_tx),         // tx serial output
        .tx_busy(tx_busy)       // tx is active (not ready)
    );

endmodule