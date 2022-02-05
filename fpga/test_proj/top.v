module top(
    output LED1
);

    wire clk;

    SB_HFOSC inthosc (
      .CLKHFPU(1'b1),
      .CLKHFEN(1'b1),
      .CLKHF(clk)
    );

    reg [24:0] counter;

    always @(posedge clk) begin
        counter <= counter + 1;
    end

    assign LED1 = counter[22];

endmodule
