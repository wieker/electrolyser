module correlator(
    input clk, rst, sig, code,
    output reg [7:0] value,
);

    reg [13:0] match_counter;
    reg sig_buf;
    reg code_buf;

	always @(posedge clk)
	begin
	    sig_buf <= sig;
	    code_buf <= code;
		if(rst) begin
		    value <= match_counter[11:4];
		    match_counter <= (code_buf == sig_buf);
		end else begin
		    match_counter <= match_counter + (code_buf == sig_buf);
		end
	end

endmodule