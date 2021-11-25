module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output reg rdy,
);
    iq_demod early(.clk(clk), .rst_in(rst), .sig(sig), .rdy(stb), .value(value));
    iq_demod late(.clk(clk), .rst_in(rst), .sig(sig), .rdy(stb), .value(value));
    iq_demod actual(.clk(clk), .rst_in(rst), .sig(sig), .rdy(stb), .value(value)); //PSK modem


endmodule