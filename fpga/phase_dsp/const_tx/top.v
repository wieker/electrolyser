module top(
    output tx_out,
    input xtal_in
);

    tx tx(.xtal_in(xtal_in), .tx_out(tx_out));


endmodule
