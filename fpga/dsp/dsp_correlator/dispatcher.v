module dispatcher(
    input clk, rst_in, sig,
    output rdy3,
    output rdy4,
    output stb,
);
    wire codes[16];
    wire [7:0] results[16];
    wire rdys[16];

    assign codes[0] = 0;
    assign codes[1] = 1;
    genvar j;
    for (j=0; j < 6; j++) begin
        sig_source sig_source(.clk(clk), .rst(rst), .period(6), .phase((j % 3) * 2), .start_code(j < 3), .code(codes[2 + j]));
    end
    for (j=0; j < 8; j++) begin
        sig_source sig_source(.clk(clk), .rst(rst), .period(8), .phase((j % 4) * 2), .start_code(j < 4), .code(codes[8 + j]));
    end

    for (j=0; j < 16; j++) begin
        correlator correlator(.clk(clk), .rst(rst), .sig(sig), .code(codes[j]), .trigger(8'h50), .result(results[j]), .match(rdys[j]));
    end

    wire rst;
    dispatcher_ctl ctl(
        .rst_in(rst_in),
        .rst_out(rst),
        .stb(stb),
    );

    wire rdyf3, rdyf4;
    mass_or #(
        .WIDTH(6),
    )
    orf4(
        .inp({rdys[2], rdys[3], rdys[4], rdys[5], rdys[6], rdys[7]}),
        .val(rdyf4),
    );
    mass_or #(
        .WIDTH(8),
    )
    orf3(
        .inp({rdys[8], rdys[9], rdys[10], rdys[11], rdys[12], rdys[13], rdys[14], rdys[15]}),
        .val(rdyf3),
    );

    flag_holder fh4(
        .clk(clk),
        .set(rst),
        .inp(rdyf4),
        .val(rdy4),
    );
    flag_holder fh3(
        .clk(clk),
        .set(rst),
        .inp(rdyf3),
        .val(rdy3),
    );


endmodule