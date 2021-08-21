module sig_source(
    input clk, rst, start, input [7:0] period0, input [7:0] period1, input [7:0] phase, input start_code,
    output reg code,
);

    reg [31:0] phase_counter;
    wire [31:0] next_counter = phase_counter + 1;


	always @(posedge clk)
	begin
		if(rst)
		begin
		    phase_counter <= phase;
		    code <= start_code;
		end else if ((code == 0) && (next_counter == period0)) begin
		    code <= 1;
		    phase_counter <= 0;
		end else if ((code == 1) && (next_counter == period1)) begin
		    code <= 0;
		    phase_counter <= 0;
		end else begin
		    phase_counter <= next_counter;
		end
	end

endmodule