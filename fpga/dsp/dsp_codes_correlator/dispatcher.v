module dispatcher(
    input clk, rst_in, sig,
    output [7:0] i_value_reg,
    output [7:0] q_value_reg,
    output rdy,
);
    iq_demod early(.clk(clk), .rst_in(rst_in), .sig(sig ^ code[0]), .rdy(lrdy), .value(q_value_reg), .phase(0));
    iq_demod late(.clk(clk), .rst_in(rst_in), .sig(sig ^ code[1]), .rdy(rdy), .value(i_value_reg), .phase(0));

    wire e_code, l_code;
    nco e_nco(.clk(clk), .rst(rst_in), .control_word(16'h0800), .i_code(e_code), .phase_control_word(16'h0000));

    reg [5:0] code;

    always @(posedge e_code)
        begin
            if (!code[0] && !code[1] && !code[2]) begin
                code <= {1, 1, 0, 0, 1, 0};
            end else begin
                code <= {code[5:0], code[6]};
            end
        end

endmodule