module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output stb,
);
    wire i_code, q_code, ref;
    reg [12:0] fcw;
    reg [12:0] pcw;
    nco ref_nco(.clk(clk), .rst(rst_in), .control_word(12'h102), .i_code(ref), .phase_control_word(12'b000000000000));
    nco i_nco(.clk(clk), .rst(rst_in), .control_word(fcw), .i_code(i_code), .phase_control_word(pcw));
    nco q_nco(.clk(clk), .rst(rst_in), .control_word(fcw), .i_code(q_code), .phase_control_word(pcw + 12'b010000000000));

    wire [7:0] i_value;
    wire [7:0] q_value;
    correlator i_correlator(.clk(clk), .rst(rst), .sig(ref), .code(i_code), .value(i_value));
    correlator q_correlator(.clk(clk), .rst(rst), .sig(ref), .code(q_code), .value(q_value));

    wire rst;
    dispatcher_ctl ctl(
        .clk(clk),
        .rst_in(rst_in),
        .rst_out(rst),
        .stb(stb)
    );

    reg [3:0] control;
    wire [15:0] mult = //i_value * i_value;
            q_value * q_value;
    always@(posedge clk)
    begin
        if (rst_in) begin
            fcw <= 12'b000100000000;
            pcw <= 12'b000000000000;
        end else if (rst) begin
            value <= q_value; //mult[15:8];
        end
    end


endmodule