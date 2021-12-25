module osc(
    input clk, ctl1, ctl2,
    output rst,
);


    reg [3:0] startup;
    assign rst = !startup[3] || ctl1 || !ctl2;

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