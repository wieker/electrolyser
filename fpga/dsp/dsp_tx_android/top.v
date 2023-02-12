module top(
    output lvds_in, tx_out,
    input xtal_in
);

    reg [9:0] ctr;
    always@(posedge xtal_in)
    begin
      ctr <= ctr + 1;
    end
    wire clk;
    osc osc(.clk(clk), .rst(rst), .xtal_in(xtal_in));

    assign lvds_in = clk;
    assign tx_out = clk;

endmodule
