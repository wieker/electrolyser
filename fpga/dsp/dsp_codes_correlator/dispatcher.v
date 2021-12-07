module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output reg rdy,
);
    wire [7:0] lvalue;
    wire [7:0] rvalue;

    wire lrdy;

    reg phase;

    iq_demod early(.clk(clk), .rst_in(rst_in), .sig(sig), .rdy(lrdy), .value(lvalue), .phase(0));
    iq_demod late(.clk(clk), .rst_in(rst_in), .sig(sig ^ l_code), .rdy(rrdy), .value(rvalue), .phase(0));

    wire e_code, l_code;
    nco e_nco(.clk(clk), .rst(rst_in), .control_word(16'h0200), .i_code(e_code), .phase_control_word(16'h0000));
    nco l_nco(.clk(clk), .rst(rst_in), .control_word(16'h0200), .i_code(l_code), .phase_control_word(16'h8000));

    always @(posedge clk) begin
        if (lrdy) begin
            phase = ~ phase;
            rdy <= 1;
            if (phase) begin
                value <= lvalue;
            end else begin
                value <= rvalue;
            end
        end else begin
            rdy <= 0;
        end
    end


endmodule