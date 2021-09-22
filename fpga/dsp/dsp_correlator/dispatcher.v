module dispatcher(
    input clk, rst_in, sig,
    input [7:0] addr_in,
    input [7:0] data_in,
    input cs, we, oe,
    output reg [7:0] data_out,
    output rdy,
);

    wire rst = rst_in || (addr_in == 8'hff);

    wire codes[16];
    wire [7:0] results[16];

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
        correlator correlator(.clk(clk), .rst(rst), .sig(sig), .code(codes[j]), .capture(capture), .select(addr_in[0]), .result(results[j]));
    end


    wire capture;
    wire [32:0] next_ctr;
    assign capture = next_ctr == 32'h00000800;
    assign next_ctr = ctr + 1;

    reg [32:0] ctr;
    always@(posedge clk)
    begin
        if (rst)
            ctr <= 0;
        else if (!capture)
            ctr <= next_ctr;
        data_out <= results[addr_in[7:1]];
    end


endmodule