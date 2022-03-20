module top(
    output lvds_in,
    input xtal_in
);

    reg [7:0] ctr;
    always@(posedge xtal_in)
    begin
      ctr <= ctr + 1;
    end

    assign lvds_in = ctr[1];

endmodule
