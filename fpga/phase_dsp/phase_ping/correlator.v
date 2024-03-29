module correlator(
    input clk, rst, sig, code,
    input [7:0] shift,
    output reg [7:0] value,
);

    reg [7:0] match_counter;
    reg sig_buf;
    reg code_buf;
    reg [7:0] ram_addr;
    wire [7:0] next_addr = ram_addr + 1;

	always @(posedge clk)
	begin
	    if (rst) begin
	        ram_addr <= shift;
	    end else begin
            sig_buf <= sig;
            code_buf <= code;
            ram_addr <= ram_addr + 1;
            match_counter <= match_counter + (ram_data_out[0] ? 8'hff : 0) + xored;
            value <= match_counter;
        end
	end

    wire xored = (code_buf == sig_buf);
    assign ram_data_in[0] = xored;
    reg [7:0] ram_addr;
    wire [15:0] ram_data_in;
    wire [15:0] ram_data_out;

    SB_RAM40_4K SB_RAM40_4K_inst (
        .RDATA(ram_data_out),
        .RADDR(next_addr),
        .RCLK(clk),
        .RCLKE(1),
        .RE(1),
        .WADDR(ram_addr),
        .WCLK(clk),
        .WCLKE(1),
        .WDATA(ram_data_in),
        .WE(1)
    );

endmodule