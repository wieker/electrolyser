module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output stb,
    input [15:0] data_in,
    input [1:0] addr,
    input we
);
    wire i_code, q_code;
    reg [12:0] fcw;
    reg [12:0] pcw;
    nco nco(.clk(clk), .rst(rst_in), .control_word(fcw), .i_code(i_code), .q_code(q_code));

    wire [7:0] i_value;
    wire [7:0] q_value;
    correlator i_correlator(.clk(clk), .rst(rst), .sig(sig), .code(i_code), .value(i_value));
    correlator q_correlator(.clk(clk), .rst(rst), .sig(sig), .code(q_code), .value(q_value));

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
            value <= i_value;
        end
        if (we) begin
            if (addr == 0) begin
                fcw <= data_in[15:3];
            end else if (addr == 1) begin
                pcw <= data_in[15:3];
            end
        end
    end


endmodule