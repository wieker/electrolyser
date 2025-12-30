`include "spi_slave.v"

//opcodes:
//0x00 nop
//0x01 init
//0x02 write 16bits inverted
//0x03 read 16bits inverted
//0x04 write leds (16bits LSB)
//0x05 read leds (16bits LSB)
//0x06 write vector, the computer will send 4 * 24bit values
//0x07 read vector, the fpga will send 4 * 24bit values

module top(input [3:0] SW, input clk, output LED_R, output LED_G, output LED_B, input SPI_SCK, input SPI_SS, input SPI_MOSI, output SPI_MISO, input [3:0] SW);

   reg spi_reset;
   wire spi_rd_data_available;
   reg spi_rd_ack;
   wire [0:15] spi_rd_data;

   //assign SPI_MISO = SPI_MOSI;
   wire tmp;

   spi_slave spi_slave_inst(.clk(clk), .reset(spi_reset),
      .SPI_SCK(SPI_SCK), .SPI_SS(SPI_SS), .SPI_MOSI(SPI_MOSI), .SPI_MISO(SPI_MISO),
      .wr_buffer_free(spi_wr_buffer_free), .wr_en(spi_wr_en), .wr_data(ram_data_out),
      .rd_data_available(spi_rd_data_available), .rd_ack(spi_rd_ack), .rd_data(spi_rd_data),
      .cnt(cnt)
   );

   wire [2:0] cnt;

   reg [2:0] led;

   assign LED_R = led[0];
   assign LED_G = led[1];
   assign LED_B = led[2];

   integer i;

   initial begin

      for(i = 0; i < 4; i=i+1) begin
         vector[i] = 0;
      end

      spi_reset = 0;
      spi_wr_en = 0;
      spi_wr_data = 0;
      spi_rd_ack = 0;

      vec_ptr = 0;
      sending_vector = 0;

      led = 0;
      spi_recv_data_reg = 0;
      handle_data = 0;
   end


    reg [7:0] ram_wr_addr;
    reg [7:0] ram_rd_addr;
    wire [0:15] ram_data_out;

    SB_RAM40_4K SB_RAM40_4K_inst (
        .RDATA(ram_data_out),
        .RADDR(ram_rd_addr),
        .RCLK(clk),
        .RCLKE(1),
        .RE(1),
        .WADDR(ram_wr_addr),
        .WCLK(clk),
        .WCLKE(1),
        .WDATA(ram_wr_data),
        .WE(ram_wr_en)
    );

    wire i_code;
    nco i_nco(.clk(clk), .rst(0), .control_word(16'h2100), .i_code(i_code), .phase_control_word(16'h0000));
    reg [0:5] counter;
    reg [0:15] ram_wr_data;
    reg [0:15] shift_sig_reg;
    reg ram_wr_en;

   always @(posedge clk)
   begin

      if(spi_rd_data_available == 1) begin // rising edge
         ram_rd_addr <= ram_rd_addr + 1;
         if (spi_rd_data[8:15] == 8'h01) begin
            led[0:2] <= spi_rd_data[5:7];
         end
      end

      if (counter == 15) begin
         counter <= 0;
         ram_wr_addr <= ram_wr_addr + 1;
         ram_wr_en <= 1;
         ram_wr_data <= shift_sig_reg;
      end else begin
         counter <= counter + 1;
         ram_wr_en <= 0;
      end

      shift_sig_reg <= {shift_sig_reg[1:15], i_code};
   end

endmodule
