module dispatcher(
    input clk, rst_in, sig,
    input addr_latch,
    input [7:0] data_in,
    output [7:0] data_out,
    output rdy,
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
        correlator correlator(.clk(clk), .rst(rst), .sig(sig), .code(codes[j]), .threshold(16'h0450), .matched(results[j]));
    end

    assign data_out = addr_reg[0] ? results[15:8] : results[7:0];


    reg [32:0] ctr;
    wire [32:0] next_ctr;
    wire rst = rst_in || (next_ctr == 32'h00000800);
    assign next_ctr = ctr + 1;

    always@(posedge clk)
    begin
        if (rst)
            ctr <= 0;
        else
            ctr <= next_ctr;
    end

    reg [7:0] addr_reg;
    always@(posedge clk)
    begin
        if (addr_latch)
            addr_reg <= data_in;
        end
    end


endmodule