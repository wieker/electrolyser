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
    reg [7:0] shift;
    reg [3:0] len;

    always @(posedge clk)
        begin
            clk_state <= SWDCLK;
            if ((clk_state == 0) && (SWDCLK == 1)) begin
                len ++;
                shift <= { shift[6:0], SWDIO };
                if (len == 0) begin
                    dt <= shift;
                    rdy_swd <= 1;
                end
            end else begin
                rdy_swd <= 0;
            end
        end

endmodule
