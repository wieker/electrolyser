module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] data_out,
);

    wire codes[16];
    wire results[16];

    assign codes[0] = 0;
    assign codes[1] = 1;
    genvar j;
    for (j=0; j < 6; j++) begin
        sig_source sig_source(.clk(clk), .rst(rst), .half_period(6), .phase((j % 3) * 2), .start_code(j < 3), .code(codes[2 + j]));
    end
    for (j=0; j < 8; j++) begin
        sig_source sig_source(.clk(clk), .rst(rst), .half_period(8), .phase((j % 4) * 2), .start_code(j < 4), .code(codes[8 + j]));
    end

    for (j=0; j < 16; j++) begin
        correlator correlator(.clk(clk), .rst(rst), .sig(sig), .code(codes[j]), .threshold(16'h0400), .matched(results[j]));
    end

    wire mhz3[16];
    wire mhz4[16];
    assign mhz3[0] = results[2];
    assign mhz4[0] = results[8];
    for (j=1; j < 6; j++) begin
        assign mhz3[j] = results[2 + j] | mhz3[j - 1];
    end
    for (j=1; j < 8; j++) begin
        assign mhz4[j] = results[8 + j] | mhz4[j - 1];
    end

    wire [7:0] pre_data_out = {6'h00, mhz3[5], mhz4[7]};

    reg [32:0] ctr;
    wire [32:0] next_ctr;
    wire rst = rst_in || (next_ctr == 32'h00000800);
    assign next_ctr = ctr + 1;

    always@(posedge clk)
    begin
        if (rst) begin
            ctr <= 0;
            data_out <= pre_data_out;
        end
        else
            ctr <= next_ctr;
    end


endmodule