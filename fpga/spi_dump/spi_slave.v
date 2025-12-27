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

  reg [1:0] spi_clk_reg;
  reg [1:0] spi_ss_reg;
  wire spi_ss_falling_edge;
  wire spi_ss_rising_edge;

   assign spi_clk_rising_edge = (spi_clk_reg[1:0] == 2'b01);
   assign spi_clk_falling_edge = (spi_clk_reg[1:0] == 2'b10);

   assign SPI_MISO = counter_read[0];


   always @(posedge clk)
   begin
      if(reset == 1) begin
         rd_data <= 0;
         rd_data_available <= 0;
      end else begin
               spi_clk_reg <= {spi_clk_reg[0], SPI_SCK};


         if (spi_ss_falling_edge == 1 || spi_ss_rising_edge == 1) begin
            counter_read <= 0;
         end


         if(spi_clk_rising_edge == 1'b1) begin
            rd_data <= {SPI_MOSI, rd_data[0:14]};
            counter_read <= counter_read + 1;
         end

         if (counter_read == 16) begin
            counter_read <= 0;
            rd_data_available <= 1;
         end else begin
            rd_data_available <= 0;
         end

         if (wr_en) begin
            wr_reg <= wr_data;
         end
      end
   end
endmodule
