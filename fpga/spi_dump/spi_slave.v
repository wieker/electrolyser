// receive: byte2 | byte1 | byte0 | opcode/status
//read all the data, but can write only the two bytes as opcode contains metadata

module spi_slave(input wire clk, input wire reset,
      input wire SPI_SCK, input wire SPI_SS, input wire SPI_MOSI, output wire SPI_MISO,
      output reg wr_buffer_free, input wire wr_en, input wire [0:15] wr_data,
      output reg rd_data_available, input wire rd_ack, output reg [0:15] rd_data,
      output wire [2:0] cnt
   );

   reg [4:0] counter_read; //max 32
   reg [0:15] wr_reg;
   reg [0:15] rd_reg;

  reg [1:0] spi_clk_reg;
  reg [1:0] spi_ss_reg;
  wire spi_ss_falling_edge;
  wire spi_ss_rising_edge;
  wire spi_clk_rising_edge;
  wire spi_clk_falling_edge;

  assign cnt = counter;

   assign spi_clk_rising_edge = (spi_clk_reg[1:0] == 2'b01);
   assign spi_clk_falling_edge = (spi_clk_reg[1:0] == 2'b10);

    reg [0:4] counter;
   assign SPI_MISO = wr_reg[0];


   always @(posedge clk)
   begin
      if(reset == 1) begin
         rd_data <= 0;
         rd_data_available <= 0;
      end else begin
         spi_clk_reg <= {spi_clk_reg[0], SPI_SCK};

         if(spi_clk_rising_edge == 1'b1) begin
            wr_reg <= {wr_reg[1:15], SPI_MOSI};
            counter <= counter + 1;
         end else if (counter == 16) begin
            counter <= 0;
            rd_reg <= wr_reg;
         end
      end
   end
endmodule
