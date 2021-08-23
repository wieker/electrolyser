module correlator(
    input clk, rst, sig, code,
    output [7:0] result_miss,
    output [7:0] result_match,
);

    reg [7:0] miss_counter;
    reg [7:0] match_counter;

    assign result_miss = miss_counter;
    assign result_match = match_counter;

	always @(posedge clk)
	begin
		if(rst)
		begin
		    miss_counter <= 0;
		    match_counter <= 0;
		end else begin
		    match_counter <= match_counter + (code == sig);
		    miss_counter <= miss_counter + (code != sig);
		end
	end

endmodule