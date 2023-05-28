module dispatcher(
    input clk, rst_in, sig,
    output [7:0] i_value, output [7:0] q_value,
);

    reg ctr;
    always@(posedge clk)
    begin
      ctr <= ctr + 1;
    end

    wire i_code, q_code;
    nco i_nco(.clk(clk), .rst(rst_in), .control_word(16'h3300), .i_code(i_code), .phase_control_word(16'h0000));
    nco q_nco(.clk(clk), .rst(rst_in), .control_word(16'h3300), .i_code(q_code), .phase_control_word(16'h4000));

    correlator i_correlator(.clk(clk), .rst(rst_in), .sig(sig), .code(ctr), .value(i_value), .shift(0));
    //correlator q_correlator(.clk(clk), .rst(rst_in), .sig(sig), .code(q_code), .value(q_value), .shift(0));

endmodule