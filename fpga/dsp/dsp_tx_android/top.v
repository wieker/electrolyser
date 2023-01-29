module top(
    output lvds_in, tx_out,
    input xtal_in
);

    reg [7:0] ctr;
    always@(posedge xtal_in)
    begin
      ctr <= ctr + 1;
    end
    wire clk;
    osc osc(.clk(clk), .rst(rst), .xtal_in(xtal_in));

    assign lvds_in = ctr[6] == 0 && ctr[7] == 0 ? clk : 0;
    assign tx_out = ctr[6] == 1 && ctr[7] == 0 ? clk : 0;

endmodule
