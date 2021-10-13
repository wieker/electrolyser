module correlator(
    input clk, rst, sig, code,
    output reg match,
    output reg [7:0] value,
);

    reg [13:0] match_counter;
    reg [13:0] match_stage;
    reg sig_buf;
    reg code_buf;

	always @(posedge clk)
	begin
	    match_stage <= match_counter;
	    match <= (match_stage[7] == 1) && (match_stage[5] == 1);
	    value <= match_stage[7:0];
	    sig_buf <= sig;
	    code_buf <= code;
		if(rst) begin
		    match_counter <= (code_buf == sig_buf);
		end else begin
		    match_counter <= match_counter + (code_buf == sig_buf);
		end
	end

endmodule