module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output stb,
);
    wire i_code, q_code;
    reg [12:0] fcw;
    reg [12:0] pcw;
    nco i_nco(.clk(clk), .rst(rst_in), .control_word(fcw), .i_code(i_code), .phase_control_word(pcw));
    nco q_nco(.clk(clk), .rst(rst_in), .control_word(fcw), .i_code(q_code), .phase_control_word(pcw + 13'b0100000000000));

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

    wire q1 = (i_value[7] == 1) && ((i_value[6] == 1) || (i_value[5] == 1));
    wire q3 = (i_value[7] == 0) && ((i_value[6] == 0) || (i_value[5] == 0));
    wire q2 = (q_value[7] == 1) && ((q_value[6] == 1) || (q_value[5] == 1));
    wire q4 = (q_value[7] == 0) && ((q_value[6] == 0) || (q_value[5] == 0));
    always@(posedge clk)
    begin
        if (rst_in) begin
            fcw <= 13'b0001000000000;
            pcw <= 13'b0000000000000;
        end else if (stb) begin
            value <= {q1, 2'b00, pcw[12:8]};
            if (q1 && q2) begin
                pcw <= pcw + 13'b0000100000000;
            end else if (q1 && q4) begin
                pcw <= pcw + 13'b1111100000000;
            end else if (q1) begin
            end else if (q2) begin
                pcw <= pcw + 13'b0100000000000;
            end else if (q4) begin
                pcw <= pcw + 13'b1100000000000;
            end else if (q3) begin
                pcw <= pcw + 13'b1000000000000;
            end
        end
    end


endmodule