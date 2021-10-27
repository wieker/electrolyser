module fifo(input wire clk, reset, input wire enin, input wire[7:0] din, output reg enout, output reg wire[7:0] dout);
    always@(posedge clk)
    begin
        if (enin) begin
            enout <= 1;
            dout <= din;
        end else begin
            enout <= 0;
        end
    end
endmodule
