module top(
    output LED1, led2, led3, rf, fpga_tx,
    input CLK1, CLK2, fpga_rx,
);

    reg [3:0] startup;
    assign rst = !startup[3];

    always@(posedge CLK1)
    begin
      if (rst)
        startup <= startup + 1;
    end

    hex_dump hex_dump(.clk(CLK1), .rst(rst), .fpga_tx(fpga_tx), .sig(counter2));

    reg counter2;

    always @(posedge CLK2)
        begin
            counter2 <= counter2 + 1;
        end

endmodule
