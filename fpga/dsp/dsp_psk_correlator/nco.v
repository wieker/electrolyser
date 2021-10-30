module nco(
    input clk, rst, input [15:0] control_word,
    input [15:0] phase_control_word,
    output reg i_code,
);

    reg [15:0] base_sig;
    reg [15:0] base_sig_pipeline0;
    reg [15:0] base_sig_pipeline1;
    reg [15:0] latched_word;
    reg [15:0] latched_pcw;
    always@(posedge clk)
    begin
        base_sig_pipeline0 <= base_sig;
        base_sig_pipeline1 <= base_sig_pipeline0 + latched_pcw;
        latched_word <= control_word;
        latched_pcw <= phase_control_word;
        if (rst) begin
            base_sig <= 0;
        end else begin
            base_sig <= base_sig + latched_word;
        end
        i_code <= base_sig_pipeline1[15];
    end

endmodule