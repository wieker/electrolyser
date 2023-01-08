module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] value,
);
    wire i_code, q_code;
    nco i_nco(.clk(clk), .rst(rst_in), .control_word(16'h1000), .i_code(i_code), .phase_control_word(16'h0000));
    nco q_nco(.clk(clk), .rst(rst_in), .control_word(16'h1000), .i_code(q_code), .phase_control_word(16'h4000));

    wire [7:0] i_value;
    wire [7:0] q_value;
    correlator i_correlator(.clk(clk), .rst(rst_in), .sig(sig), .code(i_code), .value(i_value), .shift(0));
    correlator q_correlator(.clk(clk), .rst(rst_in), .sig(sig), .code(q_code), .value(q_value), .shift(0));

    reg [7:0] q1;
    reg [7:0] q2;
    reg [7:0] q3;
    reg [7:0] q4;

    always@(posedge clk)
    begin
        q1 <= i_value[7] ? i_value : ~ i_value;
        q2 <= q_value[7] ? q_value : ~ q_value;
        q3 <= {0, q1[6:0]};
        q4 <= {0, q2[6:0]};
        value <= q3 + q4;
    end

endmodule