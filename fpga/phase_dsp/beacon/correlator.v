module correlator(
    input clk, rst, sig, code,
    input [7:0] shift,
    output reg [7:0] value,
);

    reg [7:0] match_counter;
    reg sig_buf;
    reg code_buf;
    reg [7:0] ram_addr;

	always @(posedge clk)
	begin
	    if (rst) begin
	        ram_addr <= shift;
	    end else begin
            sig_buf <= sig;
            code_buf <= code;
            ram_addr <= ram_addr + 1;
            if (ram_addr == 0) begin
                value <= match_counter[7:0];
                match_counter <= 0;
            end else begin
                match_counter <= xored + match_counter;
            end
        end
	end

    wire xored = (code_buf == sig_buf);

endmodule