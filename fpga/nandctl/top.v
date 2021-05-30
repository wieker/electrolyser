module top(
    output LED1, LED2, LED3,
    input CLK,

    inout d0, d1, d2, d3,
    inout d4, d5, d6, d7,
    output ce, cle, we, re, ale,

    input  RX,
    output TX,
);

    reg [21:0] counter;

    always @(posedge CLK)
        begin
            counter <= counter + 1;
        end
    assign LED1 = counter[19];
    assign LED2 = counter[20];
    assign LED3 = counter[21];

    wire nand_oe;
    wire [7:0] nand_din;
    wire [7:0] nand_dout;

	logic logic(
		.clk(clk),

		.RX(RX),
		.TX(TX),

		.nand_oe(nand_oe),
		.nand_din(nand_din),
		.nand_dout(nand_dout)
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
