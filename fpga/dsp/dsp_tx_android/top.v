module top(
    output lvds_in,
    input xtal_in
);

    reg [7:0] ctr;
    always@(posedge xtal_in)
    begin
      ctr <= ctr + 1;
    end
    wire clk;
    osc osc(.clk(clk), .rst(rst), .xtal_in(xtal_in));

    assign lvds_in = clk;

endmodule
