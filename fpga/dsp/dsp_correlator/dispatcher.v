module dispatcher(
    input clk, rst, sig,
    input [7:0] addr_in,
    input [7:0] data_in,
    input cs, we, oe,
    output [7:0] data_out,
    output rdy,
);

    wire codes[2];
    wire [7:0] results[2];
    assign codes[0] = 0;
    assign codes[1] = 1;

    correlator correlator0(.clk(clk), .rst(rst), .sig(sig), .code(codes[0]), .capture(capture), .select(addr_in[0]), .result(results[0]));
    correlator correlator1(.clk(clk), .rst(rst), .sig(sig), .code(codes[1]), .capture(capture), .select(addr_in[0]), .result(results[1]));

    assign data_out = addr_in[1] ? results[1] : results[0];


    wire capture;
    assign capture = ctr == 32'h00001000;

    reg [20:0] ctr;
    always@(posedge clk)
    begin
      ctr <= ctr + 1;
    end


endmodule