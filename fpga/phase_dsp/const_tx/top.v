module top(
    output tx_out, tx_out_n,
    input xtal_in
);

    wire txc;

    tx tx(.xtal_in(xtal_in), .tx_out(txc));

    assign tx_out_n = ~ txc;
    assign tx_out = txc;


endmodule
