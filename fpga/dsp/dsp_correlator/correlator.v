module correlator(
    input clk, rst, sig, code,
    input [15:0] threshold,
    output matched,
);

    reg [15:0] match_counter;

    assign matched = match_counter == threshold;

	always @(posedge clk)
	begin
		if(rst)
		begin
		    match_counter <= 0;
		end else begin
		    match_counter <= match_counter + (code == sig);
		end
	end

endmodule