module correlator(
    input clk, rst, sig, code,
    input capture,
    output reg [31:0] result_miss,
    output reg [31:0] result_match,
);

    reg [31:0] miss_counter;
    reg [31:0] match_counter;

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
		    miss_counter <= 0;
		    match_counter <= 0;
		end else begin
		    match_counter <= match_counter + (code == sig);
		    miss_counter <= miss_counter + (code != sig);
		end
	end

endmodule