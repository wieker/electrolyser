module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output stb
);
    wire code4, code3;
    nco nco4(.clk(clk), .rst(rst_in), .control_word(13'b0001010101000), .i_code(code4));
    nco nco3(.clk(clk), .rst(rst_in), .control_word(13'b0001000000000), .i_code(code3));

    wire [7:0] value3;
    wire [7:0] value4;
    correlator correlator4(.clk(clk), .rst(rst), .sig(sig), .code(code4), .value(value4));
    correlator correlator3(.clk(clk), .rst(rst), .sig(sig), .code(code3), .value(value3));

    wire rst;
    dispatcher_ctl ctl(
        .clk(clk),
        .rst_in(rst_in),
        .rst_out(rst),
        .stb(stb)
    );

    always@(posedge clk)
    begin
        if (rst) begin
            value <= value3;
            value[3] <= (value3[6] == 1) && ((value3[4] == 1) || (value3[5] == 1));
            value[4] <= (value4[6] == 1) && ((value4[4] == 1) || (value4[5] == 1));
        end
    end


endmodule