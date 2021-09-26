module flag_holder(
    input clk,
    input set,
    input inp,
    output reg val,
);

    reg temp;

    always@(posedge clk)
    begin
        if (set) begin
            val <= temp;
            temp <= 0;
        end else begin
            temp <= temp | inp;
        end
    end

endmodule