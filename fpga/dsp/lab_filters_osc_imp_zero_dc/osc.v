module osc(
    input clk,
    output rst,
);


    reg [27:0] startup;
    assign rst = !startup[27];

    SB_HFOSC inthosc (
      .CLKHFPU(1'b1),
      .CLKHFEN(1'b1),
      .CLKHF(ttt)
    );

    always@(posedge clk)
    begin
      if (rst)
        startup <= startup + 1;
    end

endmodule