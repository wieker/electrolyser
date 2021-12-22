module hex_dump(
    input clk, rst, sig, fpga_rx,
    output fpga_tx, rdy3, rdy4,
    output SPI_SCK, output SPI_SS, output SPI_MOSI, input SPI_MISO,
);

    assign rdy4 = done;
    assign rdy3 = sig;
    wire [7:0] i_value;
    wire [7:0] q_value;

    dispatcher dispatcher(.clk(clk), .rst_in(rst), .sig(sig), .rdy(stb),
        .i_value_reg(i_value), .q_value_reg(q_value));

    reg [10:0] ram_addr;
    wire [15:0] ram_data_in = {q_value[7:0], i_value[7:0]};
    wire [15:0] ram_data_out;
    wire ram_wren = (!ram_addr[10] && stb) & ~ done;

    SB_SPRAM256KA spram
    (
        .ADDRESS({6'h00, ram_addr[9:0]}),
        .DATAIN(ram_data_in),
        .MASKWREN({ram_wren, ram_wren, ram_wren, ram_wren}),
        .WREN(ram_wren),
        .CHIPSELECT(1),
        .CLOCK(clk),
        .STANDBY(1'b0),
        .SLEEP(1'b0),
        .POWEROFF(1'b1),
        .DATAOUT(ram_data_out)
    );

    reg tx_start;
    reg [7:0] touart;
    reg bugfix001;
    reg part;
    reg done;

    always@(posedge clk)
    begin
        if (ram_addr[10]) begin
            done <= 1;
        end else if (rst) begin
            done <= 0;
        end
        if (ram_wren) begin
            ram_addr <= ram_addr + 1;
        end else if (ram_addr[10] && !tx_busy && !bugfix001) begin
            bugfix001 <= 1;
            tx_start <= 1;
            touart <= part ? ram_data_out[15:8] : ram_data_out[7:0];
            part = ~ part;
            if (part) begin
                ram_addr <= ram_addr + 1;
            end
        end else begin
            if (!tx_busy && bugfix001) begin
                bugfix001 <= 0;
            end
            tx_start <= 0;
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
        .tx_start(tx_start),    // trigger transmission
        .tx_serial(fpga_tx),         // tx serial output
        .tx_busy(tx_busy)       // tx is active (not ready)
    );


    spi_master spi_master_inst(.clk(clk), .reset(spi_reset),
          .SPI_SCK(SPI_SCK), .SPI_SS(SPI_SS), .SPI_MOSI(SPI_MISO), .SPI_MISO(SPI_MOSI),
          .addr_buffer_free(spi_addr_buffer_free), .addr_en(spi_addr_en), .addr_data(spi_addr_data),
          .rd_data_available(spi_rd_data_available), .rd_ack(spi_rd_ack), .rd_data(spi_rd_data)
    );

endmodule