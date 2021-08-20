module sig_source(
    input clk, rst, start, input [7:0] period0, input [7:0] period1, input [7:0] phase,
    output code,
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
		end else if (match_counter[10] == 1) begin
		    result <= miss_counter[13:6];
		    rdy <= 1;
		end else begin
		    match_counter <= match_counter + (code == sig);
		    miss_counter <= miss_counter + (code != sig);
		end
	end

endmodule