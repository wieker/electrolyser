module hex_dump(
    input clk, rst, sig, fpga_rx,
    output fpga_tx, rdy3, rdy4,
    output SPI_SCK, output SPI_SS, output SPI_MOSI, input SPI_MISO,
);

    assign rdy4 = done;
    assign rdy3 = sig;

    reg [23:0] ram_addr;

    reg tx_start;
    reg [7:0] touart;
    reg bugfix001;
    reg state;

    always@(posedge clk)
    begin
        if (rst) begin

        end else if ((ram_addr < 256) && !state && !tx_busy && !bugfix001) begin
            state <= 1;
            spi_addr_en <= 1;
        end else if ((ram_addr < 256) && spi_rd_data_available && !tx_busy && !bugfix001) begin
            spi_rd_ack <= 1;
            bugfix001 <= 1;
            tx_start <= 1;
            touart <= spi_rd_data[7:0];
            ram_addr <= ram_addr + 1;
            state <= 0;
        end else begin
            spi_rd_ack <= 0;
            spi_addr_en <= 0;
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

   reg spi_reset;
   wire spi_addr_buffer_free;
   reg spi_addr_en;
   reg [23:0] spi_addr_data;
   wire spi_rd_data_available;
   reg spi_rd_ack;
   wire [31:0] spi_rd_data;

    spi_master spi_master_inst(.clk(clk), .reset(rst),
          .SPI_SCK(SPI_SCK), .SPI_SS(SPI_SS), .SPI_MOSI(SPI_MOSI), .SPI_MISO(SPI_MISO),
          .addr_buffer_free(spi_addr_buffer_free), .addr_en(spi_addr_en), .addr_data(ram_addr),
          .rd_data_available(spi_rd_data_available), .rd_ack(spi_rd_ack), .rd_data(spi_rd_data)
    );

endmodule