module dispatcher(
    input clk, rst_in, sig,
    input addr_latch,
    input [7:0] data_in,
    output [7:0] data_out,
    output rdy,
);

    wire codes[8];
    wire [7:0] results[8];
    assign codes[0] = 0;
    assign codes[1] = 1;

    sig_source sig_source(.clk(clk), .rst(rst), .period(8), .phase(0), .start_code(0), .code(codes[2]));
    sig_source sig_sourceB(.clk(clk), .rst(rst), .period(6), .phase(0), .start_code(0), .code(codes[3]));

    genvar j;
    for (j=4; j < 8; j++) begin
        sig_source sig_source(.clk(clk), .rst(rst), .period(6), .phase(2 + j % 2), .start_code(j < 6), .code(codes[j]));
    end

    for (j=0; j < 8; j++) begin
        correlator correlator(.clk(clk), .rst(rst), .sig(sig), .code(codes[j]), .capture(capture), .select(addr_reg[0]), .result(results[j]));
    end

    wire [7:0] data_out1 = addr_reg[2] ? addr_reg[1] ? results[3] : results[2] : addr_reg[1] ? results[1] : results[0];
    wire [7:0] data_out2 = addr_reg[2] ? addr_reg[1] ? results[7] : results[6] : addr_reg[1] ? results[5] : results[4];
    assign data_out = addr_reg[3] ? data_out2 : data_out1;


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
    always@(posedge clk)
    begin
        if (addr_latch)
            addr_reg <= data_in;
        else if (data_rdy)
            data_rdy <= 0;
        else if (data_out_latch) begin
            data_out <= correlator_out;
            data_rdy <= 1;
        end
    end


endmodule