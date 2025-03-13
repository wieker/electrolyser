module top(
    output LED1, LED2,
    input xtal_in,
    input xclk, pwdn, reset, sda, scl,
);

    reg [24:0] counter;
    reg [24:0] counter1;
    wire clk;

    always @(posedge clk) begin
        counter <= counter + 1;
    end
    always @(posedge xtal_in) begin
        counter1 <= counter1 + 1;
    end


    SB_HFOSC inthosc (
      .CLKHFPU(1'b1),
      .CLKHFEN(1'b1),
      .CLKHF(clk)
    );

    assign LED1 = counter[24];
    assign LED2 = xclk;

endmodule
