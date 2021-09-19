module sig_source(
    input clk, rst, input [31:0] half_period, input [31:0] phase, input start_code,
    output reg code,
);

    reg [31:0] phase_counter;
    reg [31:0] half_period_reg;
    wire [31:0] next_counter = phase_counter + 1;


	always @(posedge clk)
	begin
		if(rst)
		begin
		    phase_counter <= phase;
		    code <= start_code;
		    half_period_reg <= half_period;
		end else if (next_counter == half_period_reg) begin
		    code <= !code;
		    phase_counter <= 0;
		end else begin
		    phase_counter <= next_counter;
		end
	end

endmodule