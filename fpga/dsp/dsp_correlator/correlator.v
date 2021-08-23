module correlator(
    input clk, rst, sig, code,
    output reg rdy,
    output reg [31:0] result_miss,
    output reg [31:0] result_match,
);

    reg [31:0] miss_counter;
    reg [31:0] match_counter;
    reg [31:0] counter;


	always @(posedge clk)
	begin
		if(rst)
		begin
		    miss_counter <= 0;
		    match_counter <= 0;
		    counter <= 0;
		    result_miss <= 0;
		    result_match <= 0;
		    rdy <= 0;
		end else begin
		    counter <= counter + 1;
		    match_counter <= match_counter + (code == sig);
		    miss_counter <= miss_counter + (code != sig);
		    if ((counter[26] == 1) && rdy == 0) begin
		        result_miss <= miss_counter;
		        result_match <= match_counter;
		        rdy <= 1;
		    end else if (rdy == 1) begin
		        rdy <= 0;
		    end
		end
	end

endmodule