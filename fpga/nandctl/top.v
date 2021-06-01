module top(
    output LED1, LED2, LED3,
    input CLK,

    inout d0, d1, d2, d3,
    inout d4, d5, d6, d7,
    output ce, cle, we, re, ale,

    input  RX,
    output TX,
);

    wire led;

    reg [21:0] counter;
	reg [7:0] reset_cnt;
	reg reset;
	always @(posedge CLK)
	begin
		if(reset_cnt != 8'hff)
        begin
            reset_cnt <= reset_cnt + 8'h01;
            reset <= 1'b1;
        end
        else
            reset <= 1'b0;
	end


    always @(posedge CLK)
        begin
            counter <= counter + 1;
        end
    assign LED1 = TX;
    assign LED2 = 0;
    assign LED3 = led;

    reg en;
    always @(posedge CLK)
        begin
            en <= LED3;
        end

    wire nand_oe;
    wire [7:0] nand_din;
    wire [7:0] nand_dout;

	logic logic(
		.clk(CLK),
		.reset(reset),

		.RX(RX),
		.TX(TX),

		.nand_oe(nand_oe),
		.nand_din(nand_din),
		.nand_dout(nand_dout),

        .ce(ce),
        .cle(cle),
        .we(we),
        .re(re),
        .ale(ale),

        .led(led),
	);

	SB_IO #(
		.PIN_TYPE(6'b 1010_01),
		.PULLUP(1'b 0)
	) io_buf [7:0] (
		.PACKAGE_PIN({d0, d1, d2, d3, d4, d5, d6, d7}),
		.OUTPUT_ENABLE({nand_oe, nand_oe, nand_oe, nand_oe, nand_oe, nand_oe, nand_oe, nand_oe}),
		.D_IN_0({nand_din[0], nand_din[1], nand_din[2], nand_din[3], nand_din[4], nand_din[5], nand_din[6], nand_din[7]}),
		.D_OUT_0({nand_dout[0], nand_dout[1], nand_dout[2], nand_dout[3], nand_dout[4], nand_dout[5], nand_dout[6], nand_dout[7]})
	);

endmodule
