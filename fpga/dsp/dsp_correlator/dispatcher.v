module dispatcher(
    input clk, rst_in, sig,
    output rdy3,
    output rdy4,
    output stb,
);
    wire codes[16];
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
        correlator correlator(.clk(clk), .rst(rst), .sig(sig), .code(codes[j]), .match(rdys[j]));
    end

    wire rst;
    dispatcher_ctl ctl(
        .clk(clk),
        .rst_in(rst_in),
        .rst_out(rst),
        .stb(stb),
    );

    reg temp1, temp2;
	always @(posedge clk)
	begin
	    temp1 <= rdys[8] | rdys[9] | rdys[10] | rdys[11];
	    temp2 <= rdys[12] | rdys[13] | rdys[14] | rdys[15];
	end

    flag_holder fh4(
        .clk(clk),
        .set(rst),
        .inp(rdys[2] | rdys[3] | rdys[4] | rdys[5] | rdys[6] | rdys[7]),
        .val(rdy4),
    );
    flag_holder fh3(
        .clk(clk),
        .set(rst),
        .inp(temp1 | temp2),
        .val(rdy3),
    );


endmodule