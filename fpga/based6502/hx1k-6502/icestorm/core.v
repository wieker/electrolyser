module core( clk, reset, AB, DI, DO, WE, IRQ, NMI, RDY );

input clk;              // CPU clock
input reset;            // reset signal
output reg [15:0] AB;   // address bus
input [7:0] DI;         // data in, read bus
output [7:0] DO;        // data out, write bus
output WE;              // write enable
input IRQ;              // interrupt request
input NMI;              // non-maskable interrupt request
input RDY;              // Ready signal. Pauses CPU when RDY=0

    reg  [15:0] PC;
    wire inc;
    wire dec;
    wire

    assign WE = 0;

    always @(posedge clk) begin
        if (inc)
            PC <= PC + 1;
        if (!inc && dec)
            PC <= PC - 1;
        AB <= PC;
    end

endmodule