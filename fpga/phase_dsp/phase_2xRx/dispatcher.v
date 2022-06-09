module dispatcher(
    input clk, rst_in, sig,
    output rdy,
);
    wire i_code, q_code;
    nco i_nco(.clk(clk), .rst(rst_in), .control_word(16'h2000), .i_code(i_code), .phase_control_word(16'h0000));
    nco q_nco(.clk(clk), .rst(rst_in), .control_word(16'h2000), .i_code(q_code), .phase_control_word(16'h4000));

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

    wire [7:0] i_value1;
    wire [7:0] q_value1;
    correlator i_correlator1(.clk(clk), .rst(rst_in), .sig(sig), .code(i_code), .value(i_value1), .shift(8'h80));
    correlator q_correlator1(.clk(clk), .rst(rst_in), .sig(sig), .code(q_code), .value(q_value1), .shift(8'h80));

    reg [7:0] q11;
    reg [7:0] q21;
    reg [7:0] q31;
    reg [7:0] q41;

    always@(posedge clk)
    begin
        q11 <= i_value1[7] ? i_value1 : ~ i_value1;
        q21 <= q_value1[7] ? q_value1 : ~ q_value1;
        q31 <= {0, q11[6:0]};
        q41 <= {0, q21[6:0]};
        value1 <= q31 + q41;
    end

    reg [7:0] value;
    reg [7:0] value1;
    assign rdy = value[7] | (value[6] && value[5]) | value1[7] | (value1[6] && value1[5]);

endmodule