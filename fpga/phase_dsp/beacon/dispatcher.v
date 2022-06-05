module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output reg rdy,
);
    wire i_code, q_code;
    nco i_nco(.clk(clk), .rst(rst_in), .control_word(16'h2000), .i_code(i_code), .phase_control_word(16'h0000));
    nco q_nco(.clk(clk), .rst(rst_in), .control_word(16'h2000), .i_code(q_code), .phase_control_word(16'h4000));

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

    wire stb;
    reg st1;
    reg st2;
    reg [7:0] q1;
    reg [7:0] q2;
    reg [7:0] q3;
    reg [7:0] q4;
    reg [7:0] s1;
    reg [7:0] s2;
    reg [7:0] s3;
    reg [7:0] s4;

    always@(posedge clk)
    begin
        s1 <= i_value;
        s2 <= ~ i_value;
        s3 <= q_value;
        s4 <= ~ q_value;
        q1 <= s1[7] ? s1 : s2;
        q2 <= s3[7] ? s3 : s4;
        q3 <= {0, q1[6:0]};
        q4 <= {0, q2[6:0]};
        value <= q3 + q4;
    end


endmodule