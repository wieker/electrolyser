module correlator(
    input clk, rst, sig, code,
    output reg match,
    output reg [7:0] value,
);

    reg [11:0] match_counter;
    reg [11:0] match_stage;
    reg sig_buf;
    reg code_buf;

	always @(posedge clk)
	begin
	    match_stage <= match_counter;
	    match <= (match_stage[10] == 1) && ((match_stage[8] == 1) || (match_stage[9] == 1));
	    sig_buf <= sig;
	    code_buf <= code;
		if(rst) begin
		    match_counter <= (code_buf == sig_buf);
	        value <= match_stage[11:4];
		end else begin
		    match_counter <= match_counter + (code_buf == sig_buf);
		end
	end

endmodule