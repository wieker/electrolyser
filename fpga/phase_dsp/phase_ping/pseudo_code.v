module pseudo_code (
    input clk, rst,
    output pseudo_code,
);
    reg [3:0] code;

    assign pseudo_code = code[3];

    always @ (posedge clk) begin
        if (rst) begin
            code <= 1;
        end else begin
            code <= {code,code[3]^code[2]};
        end
    end

endmodule