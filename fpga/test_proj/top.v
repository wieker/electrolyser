module top(
    output LED1, led2, led3,
    input CLK
);

    reg [21:0] counter;

    always @(posedge CLK)
        begin
            counter <= counter + 1;
        end

    assign LED1 = counter[20];
    assign led2 = counter[21];
    assign led3 = counter[19];

endmodule
