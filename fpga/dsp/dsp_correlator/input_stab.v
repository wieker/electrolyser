module input_stab(
    input clk, rst, in,
    output sig,
);

    sig_source sig_source(.clk(clk), .rst(rst), .period0(16), .period1(16), .phase(4), .code(sig), .start_code(0));

endmodule