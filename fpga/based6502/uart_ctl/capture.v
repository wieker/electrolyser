module capture(
    input clk,              // 4..0MHz CPU clock
    input reset,            // Low-true reset

	input SWDCLK,				// serial RX
	input SWDIO,				// serial RX
	input SWDRST,				// serial RX

    output reg [7:0] counter,
);

    reg clk_state;

    always @(posedge clk)
        begin
            clk_state <= SWDCLK;
            if (clk_state != SWDCLK) begin
                counter ++;
            end
        end

endmodule
