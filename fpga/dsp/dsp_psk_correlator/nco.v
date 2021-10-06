module nco(
    input clk, rst, input [12:0] control_word,
    output reg i_code, output reg q_code,
);

    reg [12:0] base_sig;
    reg [12:0] base_sig_pipeline;
    wire [1:0] quad = base_sig_pipeline[12:11] + {0, 1};
    always@(posedge clk)
    begin
        base_sig_pipeline <= base_sig;
        if (rst) begin
            base_sig <= 0;
        end else
            base_sig <= base_sig + control_word;
        end
        i_code <= base_sig_pipeline[12];
        q_code <=  quad[1];
    end

endmodule