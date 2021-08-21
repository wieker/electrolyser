module correlator(
    input clk, rst, sig, code,
    output reg rdy, output reg [7:0] result,
);

    reg [31:0] miss_counter;
    reg [31:0] match_counter;


	always @(posedge clk)
	begin
		if(rst)
		begin
		    miss_counter <= 0;
		    match_counter <= 0;
		    result <= 0;
		    rdy <= 0;
		end else if ((match_counter[26] == 1) && rdy == 0) begin
		    result <= miss_counter[13:6];
		    rdy <= 1;
		end else if (rdy == 1) begin
		    miss_counter <= 0;
		    match_counter <= 0;
		    result <= 0;
		    rdy <= 0;
		end else begin
		    match_counter <= match_counter + (code == sig);
		    miss_counter <= miss_counter + (code != sig);
		end
	end

endmodule