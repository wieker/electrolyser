module correlator(
    input clk, rst, sig, code,
    input [7:0] trigger,
    output [7:0] result,
    output reg match,
);

    reg [15:0] match_counter;

    assign result = match_counter[11:4];

	always @(posedge clk)
	begin
	    match <= result == trigger;
		if(rst) begin
		    match_counter <= (code == sig);
		end else begin
		    match_counter <= match_counter + (code == sig);
		end
	end

endmodule