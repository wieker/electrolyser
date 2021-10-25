module sigsv2(
    input clk, rst, left, right,
    output reg code
);

    reg [0:15] base_sig;
    wire sig_in;
    always@(posedge clk)
    begin
        code <= base_sig[0];
        if (rst) begin
            base_sig <= 16'h00ff;
        end else if (left) begin
            base_sig <= {base_sig[2:15], base_sig[0], base_sig[1]};
        end else if (right) begin
            base_sig <= base_sig;
        end else begin
            base_sig <= {base_sig[1:15], base_sig[0]};
        end
    end

endmodule