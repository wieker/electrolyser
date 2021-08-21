module input_stab(
    input clk, rst, in,
    output sig,
);

    sig_source sig_source(.clk(clk), .rst(rst), .period0(48 * 1024), .period1(48 * 1024), .phase(4), .code(sig), .start_code(0));

endmodule