module dispatcher(
    input clk, rst_in, sig,
    output reg rdy3,
    output reg rdy4,
    output reg stb,
);

    wire rst = rst_in | capture;

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

    wire rdys4[6];
    wire rdys3[8];
    assign rdys4[0] = rdys[2];
    assign rdys3[0] = rdys[8];
    for (j=1; j < 6; j++) begin
        assign rdys4[j] = rdys4[j - 1] | rdys[2 + j];
    end
    for (j=1; j < 8; j++) begin
        assign rdys3[j] = rdys3[j - 1] | rdys[8 + j];
    end

    wire capture;
    wire [32:0] next_ctr;
    assign capture = next_ctr == 32'h00000800;
    assign next_ctr = ctr + 1;

    reg [32:0] ctr;
    reg prerdy4;
    reg prerdy3;
    always@(posedge clk)
    begin
        if (rst) begin
            ctr <= 0;
            rdy3 <= prerdy3;
            rdy4 <= prerdy4;
            prerdy4 <= 0;
            prerdy3 <= 0;
            stb <= 1;
        end else begin
            ctr <= next_ctr;
            prerdy4 <= prerdy4 | rdys4[5];
            prerdy3 <= prerdy3 | rdys3[7];
            stb <= 0;
        end
    end


endmodule