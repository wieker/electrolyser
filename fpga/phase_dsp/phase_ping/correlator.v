module correlator(
    input clk, rst, sig, code,
    output reg [7:0] value,
);

    reg [7:0] match_counter;
    reg sig_buf;
    reg code_buf;
    reg [7:0] svd1;
    reg [7:0] svd2;

	always @(posedge clk)
	begin
	    sig_buf <= sig;
	    code_buf <= code;
	    ram_addr <= ram_addr + 1;
        value <= match_counter[7:0];
        svd1 <= match_counter + (code_buf == sig_buf);
        svd2 <= ram_data_out[0] ? 8'hff : 0;
        match_counter <= svd1 + svd2;
	end

    assign ram_data_in[0] = (code_buf == sig_buf);
    reg [7:0] ram_addr;
    wire [15:0] ram_data_in;
    wire [15:0] ram_data_out;

    SB_RAM40_4K SB_RAM40_4K_inst (
        .RDATA(ram_data_out),
        .RADDR(ram_addr + 1),
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