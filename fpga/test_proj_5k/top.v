module top(
    output LED1, LED2,
    input xtal_in,
);

    reg [24:0] counter;
    wire clk;

    always @(posedge clk) begin
        counter <= counter + 1;
    end


    SB_HFOSC inthosc (
      .CLKHFPU(1'b1),
      .CLKHFEN(1'b1),
      .CLKHF(clk)
    );

    assign LED1 = counter[24];
    assign LED2 = ~counter[24];

endmodule
