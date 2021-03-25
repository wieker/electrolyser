module counter(input wire clk, input wire[15:0] value, input restart,
    output wire out);
    reg[15:0] counter_reg;
    reg out_reg;

    initial begin
        out_reg <= 0;
    end

    always @(posedge clk)
    begin
        if (counter_reg == value)
            begin
                if (restart)
                    begin
                        counter_reg <= 0;
                        out_reg <= ~out_reg;
                    end
                else
                    begin
                        counter_reg <= counter_reg;
                        out_reg <= 1;
                    end
            end
        else
            begin
                counter_reg <= counter_reg + 1;
            end
    end

    assign out = out_reg;

endmodule