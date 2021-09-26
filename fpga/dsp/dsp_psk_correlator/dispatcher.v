module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] phases,
    output stb,
);
    wire [7:0] codes;
    wire [7:0] rdys;
    genvar j;
    for (j=0; j < 8; j++) begin
        sig_source sig_source(.clk(clk), .rst(rst), .period(8), .phase((j % 4) * 2), .start_code(j < 4), .code(codes[j]));
    end

    for (j=0; j < 8; j++) begin
        correlator correlator(.clk(clk), .rst(rst), .sig(sig), .code(codes[j]), .match(rdys[j]));
    end

    wire rst;
    dispatcher_ctl ctl(
        .clk(clk),
        .rst_in(rst_in),
        .rst_out(rst),
        .stb(stb),
    );

    reg [7:0] matches;
    always@(posedge clk)
    begin
        if (rst) begin
            phases <= matches;
            matches <= 0;
        end else begin
            matches <= matches | rdys;
        end
    end


endmodule