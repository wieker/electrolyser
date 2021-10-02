module osc(
    output clk, rst
);


    reg [3:0] startup;
    assign rst = !startup[3];

    SB_HFOSC inthosc (
      .CLKHFPU(1'b1),
      .CLKHFEN(1'b1),
      .CLKHF(clk)
    );

    always@(posedge clk)
    begin
      if (rst)
        startup <= startup + 1;
    end

endmodule