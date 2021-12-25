module spi_writer(input wire clk, input wire reset,
      output reg SPI_SCK, output SPI_SS, output reg SPI_MOSI, input wire SPI_MISO,
      output reg addr_buffer_free, input addr_en, input [23:0] addr_data,
      output reg rd_data_available, input wire rd_ack, input [15:0] wr_data
   );

   //states
   parameter IDLE = 0, INIT=IDLE+1, WAIT_READ_ADDR=INIT+1,
             SEND_READ_CMD=WAIT_READ_ADDR+1, SEND_READ_ADDR=SEND_READ_CMD+1, READ_FLASH=SEND_READ_ADDR+1, WAIT_READ_ACK=READ_FLASH+1,
             SEND_WAKE_UP_CMD=WAIT_READ_ACK+1, WAIT_WAKE_UP=SEND_WAKE_UP_CMD+1,
             SEND_WE_CMD=WAIT_WAKE_UP+1, WAIT_WE=SEND_WE_CMD+1, WAIT_BANK=WAIT_WE+1;

   reg [2:0] counter_clk;
   reg [5:0] counter_send; //64 max
   reg [3:0] state;
   reg [23:0] read_addr_reg;
   reg [7:0] read_cmd;
   reg [7:0] wake_up_cmd;
   reg [7:0] we_cmd;
   reg spi_ss_reg;
   reg [31:0] wake_up_wait_counter;
   reg [7:0] counter_written;
   reg [15:0] wr_data_latch;
   reg rd_ack_old;

   assign SPI_SS = spi_ss_reg;

   initial begin
      SPI_SCK = 0;
      rd_data_available = 0;
      rd_ack_old = 0;

      counter_clk = 0;
      counter_send = 0;
      state = WAIT_READ_ADDR;
      addr_buffer_free = 1;
      read_addr_reg = 0;
      counter_written = 0;

      //bunch of commands to read status registers as well as the flash from the datasheet
      read_cmd = 8'h02; //write
      // read_cmd = 8'h0B; //fast read
      // read_cmd = 8'h5d //READ SERIAL FLASH DISCOVERY PARAMETER
      // read_cmd = 8'hB5; //read non volatile parameters
      // read_cmd = 8'h85; //READ VOLATILE CONFIGURATION REGISTER
      // read_cmd = 8'h9F; //read ID
      // read_cmd = 8'h05; //read status register
      wake_up_cmd = 8'hAB; //wakes up the flash, for writing
      we_cmd = 8'h06;

      SPI_MOSI = 0;
      spi_ss_reg = 1; //active low
      wake_up_wait_counter = 0;
   end

   always @(posedge clk)
   begin
      if(reset == 1) begin

      end else begin
         case (state)

         WAIT_READ_ADDR : begin //wait for an address to be written
            spi_ss_reg <= 1; //un select slave
            if(addr_en == 1) begin
               read_addr_reg <= addr_data;
               addr_buffer_free <= 0;
               state <= SEND_WAKE_UP_CMD; //go directly to the sending of the READ command
            end
         end

         //send a wake up command to the flash, not needed when only reading the flash
         //skipped here
         SEND_WAKE_UP_CMD : begin
            counter_clk <= counter_clk + 1;
            spi_ss_reg <= 0;

            if(counter_clk == 3'b000)begin
               SPI_MOSI <= wake_up_cmd[7]; //MSB
               SPI_SCK <= 0;
            end

            if(counter_clk >= 3'b001 && counter_clk <= 3'b110) begin

            end

            if(counter_clk == 3'b100) begin
               SPI_SCK <= 1;

            end

            if(counter_clk == 3'b111) begin
               wake_up_cmd[7:0] <= {wake_up_cmd[6:0], wake_up_cmd[7]};
               counter_clk <= 0;
               counter_send <= counter_send + 1;
               if(counter_send == 7) begin
                  spi_ss_reg <= 1;
                  state <= WAIT_WAKE_UP;
                  counter_send <= 0;
               end
            end
         end

         //after sending the waking up, wait for a bit
         WAIT_WAKE_UP : begin
            spi_ss_reg <= 1;
            wake_up_wait_counter <= wake_up_wait_counter + 1;
            if(wake_up_wait_counter[10] == 1) begin
               wake_up_wait_counter <= 0;
               state <= SEND_WE_CMD;
            end
         end

         SEND_WE_CMD : begin
            counter_clk <= counter_clk + 1;
            spi_ss_reg <= 0;

            if(counter_clk == 3'b000)begin
               SPI_MOSI <= we_cmd[7]; //MSB
               SPI_SCK <= 0;
            end

            if(counter_clk >= 3'b001 && counter_clk <= 3'b110) begin

            end

            if(counter_clk == 3'b100) begin
               SPI_SCK <= 1;

            end

            if(counter_clk == 3'b111) begin
               we_cmd[7:0] <= {we_cmd[6:0], we_cmd[7]};
               counter_clk <= 0;
               counter_send <= counter_send + 1;
               if(counter_send == 7) begin
                  spi_ss_reg <= 1;
                  state <= WAIT_WE;
                  counter_send <= 0;
               end
            end
         end

         //after sending the waking up, wait for a bit
         WAIT_WE : begin
            spi_ss_reg <= 1;
            wake_up_wait_counter <= wake_up_wait_counter + 1;
            if(wake_up_wait_counter[10] == 1) begin
               wake_up_wait_counter <= 0;
               state <= SEND_READ_CMD;
            end
         end

         //send the read command (8 bit)
         SEND_READ_CMD : begin
            counter_clk <= counter_clk + 1;
            spi_ss_reg <= 0;

            if(counter_clk == 3'b000)begin
               SPI_SCK <= 0;
               SPI_MOSI <= read_cmd[7]; //MSB
            end

            if(counter_clk >= 3'b001 && counter_clk <= 3'b110) begin

            end

            if(counter_clk == 3'b100) begin
               SPI_SCK <= 1;

            end

            if(counter_clk == 3'b111) begin
               read_cmd[7:0] <= {read_cmd[6:0], read_cmd[7]};
               counter_clk <= 0;
               counter_send <= counter_send + 1;
               if(counter_send == 7) begin
                  state <= SEND_READ_ADDR;
                  counter_send <= 0;
               end
            end
         end

         //send the 24bit address we want to read from
         SEND_READ_ADDR : begin
            counter_clk <= counter_clk + 1;
            spi_ss_reg <= 0; //slave is selected

            if(counter_clk == 3'b000) begin
               SPI_MOSI <= read_addr_reg[23]; //MSB
               SPI_SCK <= 0;
            end

            if(counter_clk == 3'b100) begin
               SPI_SCK <= 1;
            end

            if(counter_clk == 3'b111) begin
               read_addr_reg[23:0] <= {read_addr_reg[22:0], read_addr_reg[23]};
               counter_clk <= 0;
               counter_send <= counter_send + 1;
               if(counter_send == 23) begin
                  state <= READ_FLASH;
                  counter_send <= 0;
               end
            end
         end

         //read the actual flash value (32bit)
         READ_FLASH: begin
             if (counter_send == 0) begin
                wr_data_latch <= wr_data;
             end
            counter_clk <= counter_clk + 1;
            spi_ss_reg <= 0; //slave is selected

            if(counter_clk == 3'b000) begin
               SPI_MOSI <= wr_data_latch[15]; //MSB
               SPI_SCK <= 0;
            end

            if(counter_clk == 3'b100) begin
               SPI_SCK <= 1;
            end

            if(counter_clk == 3'b110) begin
            end

            if(counter_clk == 3'b111) begin
               wr_data_latch[15:0] <= {wr_data_latch[14:0], wr_data_latch[15]};
               counter_clk <= 0;
               counter_send <= counter_send + 1;
               if(counter_send == 15) begin
                  counter_send <= 0;
                  state <= WAIT_READ_ACK;
                  rd_data_available <= ~ rd_data_available;
                  spi_ss_reg <= 0; //un select slave
               end
            end
         end

         //now that the data is saved, wait for the next read request
         WAIT_READ_ACK: begin
            if(rd_ack != rd_ack_old) begin
                rd_ack_old <= rd_ack;
                if (counter_written == 127) begin
                    spi_ss_reg <= 1;
                    read_addr_reg <= read_addr_reg + 256;
                    state <= WAIT_BANK;
                    counter_written <= 0;
                end else begin
                    spi_ss_reg <= 0;
                    state <= READ_FLASH;
                    counter_written ++;
                end
            end
         end

         //after sending the waking up, wait for a bit
         WAIT_BANK : begin
            spi_ss_reg <= 1;
            wake_up_wait_counter <= wake_up_wait_counter + 1;
            if(wake_up_wait_counter[10] == 1) begin
               wake_up_wait_counter <= 0;
               state <= SEND_WE_CMD;
            end
         end
         default: begin
         end
         endcase

      end
   end
endmodule
