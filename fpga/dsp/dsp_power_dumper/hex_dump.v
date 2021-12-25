module hex_dump(
    input clk, rst, sig, fpga_rx,
    output fpga_tx, rdy3, rdy4,
    output SPI_SCK, output SPI_SS, output SPI_MOSI, input SPI_MISO,
);

    assign rdy4 = SPI_MISO;
    assign rdy3 = SPI_SCK;

    reg [10:0] ram_addr;

    reg tx_start;
    reg [7:0] touart;
    reg bugfix001;
    reg [1:0] state;

    always@(posedge clk)
    begin
        if (rst) begin

        end else if ((ram_addr[8] == 0) && spi_rd && !tx_busy && !bugfix001) begin
            spi_rd_ack <= state == 3;
            bugfix001 <= 1;
            tx_start <= 1;
            touart <= state == 0 ? spi_data[7:0] : state == 1 ? spi_data[15:8] : state == 2 ? spi_data[23:16] : spi_data[31:24];
            ram_addr <= ram_addr + 1;
            state <= state + 1;
        end else begin
            spi_rd_ack <= 0;
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

    reg spi_rd;
    reg spi_ack;
    reg [31:0] spi_data;

    reg [10:0] clk_counter;
    reg xclock;
    always @(posedge clk) begin
        clk_counter <= clk_counter + 1;
        if (xclock == 0 && clk_counter[4] == 0) begin
            xclock <= 1;
            spi_ack <= spi_rd_ack;
            spi_rd <= spi_rd_data_available;
            spi_data <= spi_rd_data;
        end else if (clk_counter[4] == 1) begin
            xclock <= 0;
        end
    end

   reg spi_reset;
   wire spi_addr_buffer_free;
   reg spi_addr_en;
   reg [23:0] spi_addr_data;
   wire spi_rd_data_available;
   reg spi_rd_ack;
   wire [31:0] spi_rd_data;

    spi_writer spi_master_inst(.clk(clk_counter[4]), .reset(rst),
          .SPI_SCK(SPI_SCK), .SPI_SS(SPI_SS), .SPI_MOSI(SPI_MOSI), .SPI_MISO(SPI_MISO),
          .addr_buffer_free(spi_addr_buffer_free), .addr_en(1), .addr_data(24'h100000),
          .rd_data_available(spi_rd_data_available), .rd_ack(spi_rd_ack), .rd_data(spi_rd_data)
    );

endmodule