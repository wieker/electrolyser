module nco(
    input clk, rst, input [12:0] control_word,
    output reg i_code, output reg q_code,
);

    reg [12:0] base_sig;
    reg [12:0] base_sig_pipeline;
    reg [12:0] latched_word;
    reg pipe0;
    reg [1:0] quad;
    wire [1:0] shifted = quad + 3;
    always@(posedge clk)
    begin
        base_sig_pipeline <= base_sig;
        latched_word <= control_word;
        if (rst) begin
            base_sig <= 0;
        end else begin
            base_sig <= base_sig + latched_word;
        end
        pipe0 <= base_sig_pipeline[12];
        quad <= base_sig_pipeline[12:11];
        i_code <= pipe0;
        q_code <=  shifted[1];
    end

endmodule