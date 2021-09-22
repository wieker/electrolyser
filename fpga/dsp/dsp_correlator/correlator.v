module correlator(
    input clk, rst, sig, code,
    input capture,
    output [7:0] result,
);

    reg [15:0] miss_counter;
    reg [15:0] match_counter;
    reg [15:0] result_miss;
    reg [15:0] result_match;

    assign result = result_match[11:4];

	always @(posedge clk)
	begin
		if(rst)
		begin
		    miss_counter <= 0;
		    match_counter <= 0;
		    result_miss <= 0;
		    result_match <= 0;
		end else if (capture == 1) begin
            result_miss <= miss_counter;
            result_match <= match_counter;
		end else begin
		    match_counter <= match_counter + (code == sig);
		    miss_counter <= miss_counter + (code != sig);
		end
	end

endmodule