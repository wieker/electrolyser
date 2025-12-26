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
   wire spi_wr_buffer_free;
   reg spi_wr_en;
   reg [31:0] spi_wr_data;
   wire spi_rd_data_available;
   reg spi_rd_data_available_buf;
   reg spi_rd_ack;
   wire [31:0] spi_rd_data;

   assign SPI_MISO = SPI_MOSI;
   wire tmp;

   parameter NOP=0, INIT=1, WR_INVERTED=2, RD_INVERTED=3, WR_LEDS=4, RD_LEDS=5, WR_VEC=6, RD_VEC=7;

   spi_slave spi_slave_inst(.clk(clk), .reset(spi_reset),
      .SPI_SCK(SPI_SCK), .SPI_SS(SPI_SS), .SPI_MOSI(SPI_MOSI), .SPI_MISO(tmp),
      .wr_buffer_free(spi_wr_buffer_free), .wr_en(spi_wr_en), .wr_data(spi_wr_data),
      .rd_data_available(spi_rd_data_available), .rd_ack(spi_rd_ack), .rd_data(spi_rd_data),
      .cnt(cnt)
   );

   wire [2:0] cnt;

   reg [2:0] led;

   reg [31:0] spi_recv_data_reg;
   reg handle_data;

   reg [23:0] reg_bits_inversion;

   reg [23:0] vector [0:4];
   reg [7:0] vec_ptr;
   reg sending_vector;

   assign LED_R = ~led[0];
   assign LED_G = ~led[1];
   assign LED_B = ~led[2];

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


    reg [2:0] ram_wr_addr;
    reg [2:0] ram_rd_addr;
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
        .WDATA(spi_recv_data_reg),
        .WE(handle_data)
    );

   always @(posedge clk)
   begin

      //defaults
      spi_rd_ack <= 0;
      spi_wr_en <= 0;

      spi_rd_data_available_buf <= spi_rd_data_available;

      if(spi_rd_data_available == 1 && spi_rd_data_available_buf == 0) begin // rising edge
         spi_recv_data_reg <= spi_rd_data;
         spi_rd_ack <= 1;
         handle_data <= 1;
      end

      if(handle_data == 1) begin
         led[2:0] <= ~led[2:0];
         spi_wr_en <= 1;
         spi_wr_data <= ram_data_out;
         handle_data <= 0;
         ram_rd_addr <= ram_rd_addr + 1;
         ram_wr_addr <= ram_wr_addr + 1;
      end
   end

endmodule
