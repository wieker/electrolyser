module top(
    output LED1, LED2,
    input xtal_in,
    output xclk, pwdn, reset, sda, scl,
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

    assign xclk = counter1[1];
    assign pwdn = 0;
    assign reset = 1;

endmodule
