// receive: byte2 | byte1 | byte0 | opcode/status
//read all the data, but can write only the two bytes as opcode contains metadata

module spi_slave(input wire clk, input wire reset,
      input wire SPI_SCK, input wire SPI_SS, input wire SPI_MOSI, output wire SPI_MISO,
      output reg wr_buffer_free, input wire wr_en, input wire [31:0] wr_data,
      output reg rd_data_available, input wire rd_ack, output reg [31:0] rd_data,
      output wire [2:0] cnt
   );

   reg [4:0] counter_read; //max 32
   assign cnt = state_rd[2:0];

   reg [1:0] spi_clk_reg;
   reg [1:0] spi_ss_reg;
   wire spi_ss_falling_edge;
   wire spi_ss_rising_edge;

   reg [1:0] mosi_reg;
   reg miso_out_reg;
   reg [7:0] state_rd;

   reg wr_reg_full;
   reg [31:0] wr_data_reg; //written data to send to spi/miso

   reg buffer_rd_ack;
   reg [31:0] rd_data_local;

   //states
   parameter IDLE = 0, INIT=IDLE+1, RD_WAIT_DATA=INIT+1, RD_WAIT_ACK=RD_WAIT_DATA+1, WR_WAIT_DATA=RD_WAIT_ACK+1, WR_WAIT_ACK=WR_WAIT_DATA+1;

   assign SPI_MISO = wr_data_reg[0];
   wire spi_clk_rising_edge;
   wire spi_clk_falling_edge;
   assign spi_clk_rising_edge = (spi_clk_reg[1:0] == 2'b01);
   assign spi_clk_falling_edge = (spi_clk_reg[1:0] == 2'b10);
   assign spi_ss_rising_edge = (spi_ss_reg[1:0] == 2'b01);
   assign spi_ss_falling_edge = (spi_ss_reg[1:0] == 2'b10);

   initial begin
      counter_read = 0;
      spi_clk_reg = 0;
      spi_ss_reg = 0;
      mosi_reg = 0;
      miso_out_reg = 0;
      state_rd = INIT;
      wr_reg_full = 0;
      wr_data_reg = 24'hcafe77;
      wr_queue_full = 0;
      wr_data_queue = 0;

      buffer_rd_ack = 0;
      rd_data = 0;
      rd_data_local = 0;

      rd_data_available = 0;
   end

   assign wr_buffer_free = ((~wr_queue_full) & (~wr_reg_full) & (~wr_en));

   always @(posedge clk)
   begin
      if(reset == 1) begin
         rd_data <= 0;
         rd_data_local <= 0;
         rd_data_available <= 0;
         state_rd <= INIT;
      end else begin

         spi_clk_reg <= {spi_clk_reg[0], SPI_SCK};
         mosi_reg <= {mosi_reg[0], SPI_MOSI};
         spi_ss_reg <= {spi_ss_reg[0], SPI_SS};

         if (spi_ss_falling_edge == 1 || spi_ss_rising_edge == 1) begin
            counter_read <= 0;
         end

            if(spi_clk_rising_edge == 1'b1) begin
               rd_data[31:0] <= {mosi_reg[0], rd_data[31:1]};
               counter_read <= counter_read + 1;

               if(counter_read >= 15) begin //finish recv
                  counter_read <= 0;
                  rd_data_available <= 1;
                  wr_buffer_free <= 1;
               end
            end

            if(spi_clk_falling_edge == 1'b1) begin
               wr_data_reg[31:0] <= {wr_data_reg[0], wr_data_reg[31:1]};
            end

            if (rd_data_available == 1) begin
                  rd_data_available <= 0;
            end

            if (wr_en == 1) begin
                  wr_data_reg <= wr_data;
            end
      end
   end
endmodule
