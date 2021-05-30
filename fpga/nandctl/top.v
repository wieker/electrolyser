module top(
    output LED1, LED2, LED3,
    input CLK,

    inout d0, d1, d2, d3,
    inout d4, d5, d6, d7,
    output ce, cle, we, re, ale,

    input  RX,
    output TX,
);

    reg [21:0] counter;

    always @(posedge CLK)
        begin
            counter <= counter + 1;
        end
    assign LED1 = counter[19];
    assign LED2 = counter[20];
    assign LED3 = counter[21];

endmodule
