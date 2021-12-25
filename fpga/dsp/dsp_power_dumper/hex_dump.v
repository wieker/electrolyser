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
        end else if (ram_addr[10] && (rd_data_available_old != spi_rd_data_available)) begin
            rd_data_available_old <= spi_rd_data_available;
            spi_rd_ack <= ~ spi_rd_ack;
            wr_data <= ram_data_out;
            ram_addr <= ram_addr + 1;
        end
    end

   wire spi_rd_data_available;
   reg spi_rd_ack;
   reg [15:0] wr_data;
   reg rd_data_available_old;

   reg [20:0] clk_counter;
   always @(posedge clk) begin
       clk_counter <= clk_counter + 1;
   end


    spi_writer spi_master_inst(.clk(clk_counter[10]), .reset(rst),
          .SPI_SCK(SPI_SCK), .SPI_SS(SPI_SS), .SPI_MOSI(SPI_MOSI), .SPI_MISO(SPI_MISO),
          .addr_buffer_free(spi_addr_buffer_free), .addr_en(1), .addr_data(24'h100000),
          .rd_data_available(spi_rd_data_available), .rd_ack(spi_rd_ack), .wr_data(wr_data)
    );

endmodule