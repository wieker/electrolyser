module input_stab(
    input clk, rst, in, input [7:0] phase,
    output sig,
);

    sig_source sig_source(.clk(clk), .rst(rst), .period0(48 * 1024), .period1(48 * 1024), .phase(phase * 6 * 1024), .code(sig), .start_code(0));

endmodule