module correlator(
    input clk, rst, sig, code,
    output rdy, output reg [7:0] result,
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
		end
	end

endmodule