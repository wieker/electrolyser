module dispatcher(
    input clk, rst, sig,
    input [7:0] addr_in,
    input [7:0] data_in,
    input cs, we, oe,
    output [7:0] data_out,
    output rdy,
);

    wire codes[10];
    wire [31:0] results[16];
    wire [31:0] resultsB[16];
    wire capture;

    genvar j;

    for (j=0; j < 8; j++) begin
	    sig_source sig_source(.clk(clk), .rst(rst), .period(48 * 1024), .phase(j * 6 * 1024), .start_code(0), .code(codes[j]));
    end
    assign codes[8] = 1;
    assign codes[9] = 0;

    for (j=0; j < 10; j++) begin
        correlator correlator(.clk(clk), .rst(rst), .sig(sig_in), .code(codes[j]), .capture(capture),
                .result_miss(results[j]), .result_match(resultsB[j]));
    end

    assign capture = ctr == 32'h04000000;
    assign rdy = ctr == 32'h04000001;

    reg [32:0] ctr;
    always@(posedge clk)
    begin
      ctr <= ctr + 1;
    end

    wire [31:0] prepare = addr_in[7] ? results[addr_in[6:4]] : resultsB[addr_in[6:4]];
    assign data_out = (prepare[31:24] & (addr_in[1:0] == 3)) | (prepare[23:16] & (addr_in[1:0] == 2)) |
            (prepare[15:8] & (addr_in[1:0] == 1)) | (prepare[7:0] & (addr_in[1:0] == 0));

endmodule