module capture(
    input clk,              // 4..0MHz CPU clock
    input reset,            // Low-true reset

	input SWDCLK,				// serial RX
	input SWDIO,				// serial RX
	input SWDRST,				// serial RX

    output reg [7:0] dt,
    output reg rdy_swd,
);

    reg clk_state;
    reg [3:0] clk_shift;
    reg [7:0] shift;
    reg [3:0] len;
	wire all_zero = ~|clk_shift;
	wire all_one = &clk_shift;

    initial begin
        clk_shift <= 8'hff;
        clk_state <= 1;
    end

    always @(posedge clk)
        begin
            clk_shift <= { clk_shift[2:0], SWDCLK };
            if ((clk_state == 0) && (all_one == 1)) begin
                clk_state <= 1;
                len ++;
                shift <= { shift[6:0], SWDIO };
                if (len == 0) begin
                    dt <= shift;
                    rdy_swd <= 1;
                end
            end else begin
                rdy_swd <= 0;
            end
            if ((clk_state == 1) && (all_zero == 1)) begin
                clk_state <= 0;
            end
        end

endmodule
