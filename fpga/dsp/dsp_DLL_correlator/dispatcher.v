module dispatcher(
    input clk, rst_in, sig,
    output [7:0] value,
    output rdy,
);
    //iq_demod early(.clk(clk), .rst_in(rst_in), .sig(sig), .rdy(rdy), .value(value));
    //iq_demod late(.clk(clk), .rst_in(rst_in), .sig(sig), .rdy(rdy), .value(value));
    psk_demod actual(.clk(clk), .rst_in(rst_in), .sig(sig), .rdy(rdy), .value(value)); //PSK modem


endmodule