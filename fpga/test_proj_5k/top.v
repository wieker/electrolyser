module top(
    output LED1, LED2,
    input xtal_in,
);

    reg [24:0] counter;

    always @(posedge xtal_in) begin
        counter <= counter + 1;
    end

    assign LED1 = counter[24];
    assign LED2 = ~counter[24];

endmodule
