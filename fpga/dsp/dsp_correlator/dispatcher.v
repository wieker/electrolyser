module dispatcher(
    input clk, rst, sig,
    output fpga_tx,
);

    wire codes[8];
    wire [7:0] rdy;
    wire [7:0] results[8];
    wire ready;
    genvar j;
    for (j=0; j < 8; j++) begin
	    sig_source sig_source(.clk(clk), .rst(rst | rdy_tmp[7]), .period0(48 * 1024), .period1(48*1024), .phase(j * 6 * 1024), .start_code(0), .code(codes[j]));
        correlator correlator(.clk(clk), .rst(rst | rdy_tmp[7]), .sig(sig_in), .code(codes[j]), .rdy(rdy[j]), .result(results[j]));
    end

    hex_dump hex_dump(.clk(clk), .rst(rst), .rdy(rdy), .fpga_tx(fpga_tx));

    genvar j;
    wire rdy_tmp[8];
    for (j=0; j < 8; j++) begin
        if (j == 0)
            assign rdy_tmp[j] = rdy[0];
        else
            assign rdy_tmp[j] = rdy[j] | rdy_tmp[j - 1];
    end

endmodule