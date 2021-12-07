module dispatcher(
    input clk, rst_in, sig,
    output [7:0] i_value_reg,
    output [7:0] q_value_reg,
    output rdy,
);
    iq_demod early(.clk(clk), .rst_in(rst_in), .sig(sig ^ e_code), .rdy(rdy), .value(i_value_reg), .phase(0));
    iq_demod late(.clk(clk), .rst_in(rst_in), .sig(sig ^ l_code), .rdy(rrdy), .value(q_value_reg), .phase(0));

    wire e_code, l_code;
    nco e_nco(.clk(clk), .rst(rst_in), .control_word(16'h0400), .i_code(e_code), .phase_control_word(16'h0000));
    nco l_nco(.clk(clk), .rst(rst_in), .control_word(16'h0400), .i_code(l_code), .phase_control_word(16'h4000));

endmodule