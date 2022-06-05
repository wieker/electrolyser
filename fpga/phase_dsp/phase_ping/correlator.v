module correlator(
    input clk, rst, sig, code,
    output reg [7:0] value,
);

    reg [7:0] match_counter;
    reg sig_buf;
    reg code_buf;
    reg [6:0] ram_addr;
    reg [7:0] old_value;

	always @(posedge clk)
	begin
        sig_buf <= sig;
        code_buf <= code;
        ram_addr <= ram_addr + 1;
        if (ram_addr == 0) begin
            old_value <= match_counter;
            match_counter <= 0;
        end else begin
            match_counter <= xored + match_counter;
        end
        value <= old_value + match_counter;
	end

    wire xored = (code_buf == sig_buf);

endmodule