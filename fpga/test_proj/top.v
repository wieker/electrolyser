module top(
    output LED1, led2, led3,
    input CLK
);

    reg [16:0] counter;

    always @(posedge CLK)
        begin
            counter <= counter + 1;
        end

    assign LED1 = 1;
    assign led2 = counter[11];
    assign led3 = 1;

endmodule
