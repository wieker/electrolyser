module osc(
    output clk, rst
);

    wire bclk;
    reg [19:0] bcount;

    reg [3:0] startup;
    assign rst = !startup[3];

    SB_HFOSC inthosc (
      .CLKHFPU(1'b1),
      .CLKHFEN(1'b1),
      .CLKHF(bclk)
    );

    wire clk = bcount[9];

    wire [19:0] bnext = bcount + 1;
    always@(posedge bclk)
    begin
        if (bnext == 1000) begin
            bcount <= 0;
        end else begin
            bcount <= bnext;
        end
    end

    always@(posedge clk)
    begin
      if (rst)
        startup <= startup + 1;
    end

endmodule