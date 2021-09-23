module correlator(
    input clk, rst, sig, code,
    input capture,
    output [7:0] result,
    output match,
);

    reg [15:0] match_counter;

    assign result = match_counter[11:4];
    assign match = result == 8'h50;

	always @(posedge clk)
	begin
		if(rst)
		begin
		    match_counter <= 0;
		end else if (capture == 1) begin
		end else begin
		    match_counter <= match_counter + (code == sig);
		end
	end

endmodule