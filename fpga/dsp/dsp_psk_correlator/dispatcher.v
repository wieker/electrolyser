module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output stb,
);
    wire i_code, q_code, b_code;
    reg [12:0] fcw;
    reg [12:0] pcw;
    nco i_nco(.clk(clk), .rst(rst_in), .control_word(fcw), .i_code(i_code), .phase_control_word(pcw));
    nco q_nco(.clk(clk), .rst(rst_in), .control_word(fcw), .i_code(q_code), .phase_control_word(pcw + 13'b0100000000000));
    nco b_nco(.clk(clk), .rst(rst_in), .control_word(fcw), .i_code(b_code), .phase_control_word(pcw + 13'b1100000000000));

    wire [7:0] i_value;
    wire i_match, q_match, b_match;
    correlator i_correlator(.clk(clk), .rst(rst), .sig(sig), .code(i_code), .value(i_value), .match(i_match));
    correlator q_correlator(.clk(clk), .rst(rst), .sig(sig), .code(q_code), .match(q_match));
    correlator b_correlator(.clk(clk), .rst(rst), .sig(sig), .code(b_code), .match(b_match));

    wire rst;
    dispatcher_ctl ctl(
        .clk(clk),
        .rst_in(rst_in),
        .rst_out(rst),
        .stb(stb)
    );

    reg q_save, b_save, i_save;
    always@(posedge clk)
    begin
        if (rst_in) begin
            fcw <= 13'b0001000000000;
            pcw <= 13'b0000000000000;
        end else if (rst) begin
            value <= {3'b100, i_save, 2'b00, q_save, b_save};
            q_save <= 0;
            b_save <= 0;
            i_save <= 0;
            if (i_value[7] == 0) begin
                pcw <= pcw + 13'b1000000000000;
            end else if (i_save && b_save) begin
                pcw <= pcw + 13'b0000001000000;
            end else if (i_save && q_save) begin
                pcw <= pcw + 13'b1111111000000;
            end else if (b_save) begin
                pcw <= pcw + 13'b0000010000000;
            end else if (q_save) begin
                pcw <= pcw + 13'b1111110000000;
            end
        end else begin
            q_save <= q_save | q_match;
            b_save <= b_save | b_match;
            i_save <= i_save | i_match;
        end
    end


endmodule