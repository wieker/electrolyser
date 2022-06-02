module fifo_stage(input wire clk, reset, input wire enin, input wire[7:0] din, output reg wire[7:0] dout);
    always@(posedge clk)
    begin
        if (enin) begin
            dout <= din;
        end
    end
endmodule
