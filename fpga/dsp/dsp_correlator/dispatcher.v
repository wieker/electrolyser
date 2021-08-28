module dispatcher(
    input clk, rst_in, sig,
    input [7:0] addr_in,
    input [7:0] data_in,
    input cs, we, oe,
    output [7:0] data_out,
    output rdy,
);

    wire rst = rst_in || (addr_in == 8'hff);

    wire codes[8];
    wire [7:0] results[8];
    assign codes[0] = 0;
    assign codes[1] = 1;

    sig_source sig_source(.clk(clk), .rst(rst), .period(8), .phase(0), .start_code(0), .code(codes[2]));
    correlator correlatorP(.clk(clk), .rst(rst), .sig(sig), .code(codes[2]), .capture(capture), .select(addr_in[0]), .result(results[2]));

    sig_source sig_sourceB(.clk(clk), .rst(rst), .period(6), .phase(0), .start_code(0), .code(codes[3]));
    correlator correlatorB(.clk(clk), .rst(rst), .sig(sig), .code(codes[3]), .capture(capture), .select(addr_in[0]), .result(results[3]));

    correlator correlator0(.clk(clk), .rst(rst), .sig(sig), .code(codes[0]), .capture(capture), .select(addr_in[0]), .result(results[0]));
    correlator correlator1(.clk(clk), .rst(rst), .sig(sig), .code(codes[1]), .capture(capture), .select(addr_in[0]), .result(results[1]));

    genvar j;
    for (j=4; j < 8; j++) begin
        sig_source sig_source(.clk(clk), .rst(rst), .period(6), .phase(2 + j % 2), .start_code(j < 6), .code(codes[j]));
        correlator correlator(.clk(clk), .rst(rst), .sig(sig), .code(codes[j]), .capture(capture), .select(addr_in[0]), .result(results[j]));
    end

    assign data_out1 = addr_in[2] ? addr_in[1] ? results[3] : results[2] : addr_in[1] ? results[1] : results[0];
    assign data_out2 = addr_in[2] ? addr_in[1] ? results[7] : results[6] : addr_in[1] ? results[5] : results[4];
    assign data_out = addr_in[3] ? data_out2 : data_out1;


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
    end


endmodule