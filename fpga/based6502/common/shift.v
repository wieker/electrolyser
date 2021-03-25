module shift(input wire clk, input wire reset, input wire enable, input wire in, input wire[7:0] initial_value,
                output wire out);

    reg[7:0] shift_reg;
    wire[7:0] next_value;

    initial
    begin
        shift_reg <= 'h9f;
    end

    always @(posedge clk)
    begin
        if (enable)
            shift_reg <= next_value;
        else if (reset)
            shift_reg <= 'h9f;
        else
            shift_reg <= shift_reg;
    end

    assign next_value = {shift_reg[6:0], in};
    assign out = shift_reg[7];

endmodule