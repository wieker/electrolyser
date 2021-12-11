module mstage_fifo(input wire clk, reset, input wire rd, input wire[7:0] din, output wire[7:0] dout);
    integer i;
    wire [7:0] array[16];
    for (i = 0; i < 16; i ++) begin
        if (i == 0) begin
            fifo_stage stage(.clk(clk), .reset(reset), .enin(rd), .din(din), .dout(array[1]));
        else if (i == 15) begin
            fifo_stage stage(.clk(clk), .reset(reset), .enin(rd), .din(array[14]), .dout(dout));
        end else begin
            fifo_stage stage(.clk(clk), .reset(reset), .enin(rd), .din(array[i - 1]), .dout(array[i]));
        end
    end
endmodule
