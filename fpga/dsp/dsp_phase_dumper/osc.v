module osc(
    input clk,
    output rst,
);


    reg [3:0] startup;
    assign rst = !startup[3];

    always@(posedge clk)
    begin
      if (rst)
        startup <= startup + 1;
    end

endmodule