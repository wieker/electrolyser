module correlator(
    input clk, rst, sig, code,
    output reg match,
);

    reg [11:0] match_counter;
    reg sig_buf;
    reg code_buf;

	always @(posedge clk)
	begin
	    match <= (match_counter[10] == 1) && ((match_counter[8] == 1) || (match_counter[9] == 1));
	    sig_buf <= sig;
	    code_buf <= code;
		if(rst) begin
		    match_counter <= (code_buf == sig_buf);
		end else begin
		    match_counter <= match_counter + (code_buf == sig_buf);
		end
	end

endmodule