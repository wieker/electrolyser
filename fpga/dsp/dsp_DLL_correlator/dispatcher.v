module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output reg rdy,
);
    iq_demod early();
    iq_demod late();
    iq_demod actual(); //PSK modem


endmodule