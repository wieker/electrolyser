`include "../common/shift.v"
`include "../common/counter.v"
`include "../common/ring.v"

module top(
    output LED1, led2, led3,
    A0, A1, A2, A3, A4,
    A5, A6, A7, A8, A9,
    A10, A11, A12, A13, A14,
    A15, A16, A17, A18,
    CEn, WEn, OEn,
    inout D0, D1, D2, D3,
    inout D4, D5, D6, D7,
);

    wire clk;
    reg [23:0] counter;

    reg sram_oe;
    wire [7:0] sram_din;
    wire [7:0] sram_dout;
    reg [7:0] sram_din_r;
    reg [7:0] sram_dout_r;

    ringoscillator #(.DELAY_LUTS(200)) rng(clk);

    assign CEn = 0;
    assign OEn = 0;
    assign WEn = !sram_oe;
    assign LED1 = sram_din_r[0];
    assign led2 = sram_din_r[1];
    assign led3 = sram_din_r[2];

    assign sram_dout = sram_dout_r;

    assign A0 = counter[0];
    assign A1 = counter[1];
    assign A2 = counter[2];
    assign A3 = counter[3];
    assign A4 = counter[4];
    assign A5 = counter[5];
    assign A6 = counter[6];
    assign A7 = counter[7];
    assign A8 = counter[8];
    assign A9 = counter[9];
    assign A10 = counter[10];
    assign A11 = counter[11];
    assign A12 = counter[12];
    assign A13 = counter[13];
    assign A14 = counter[14];
    assign A15 = counter[15];
    assign A16 = counter[16];
    assign A17 = counter[17];
    assign A18 = counter[18];

    always @(posedge clk)
        begin
            counter <= counter + 1;
            if (counter[19] == 0) begin
                sram_oe <= 1;
                sram_dout_r <= (counter[18] == 0) ? 8'h 55 : 8'h AA;

                sram_din_r <= 0;
            end
            if (counter[19] == 1) begin
                sram_oe <= 0;
                sram_din_r <= sram_din;
            end
        end

	SB_IO #(
		.PIN_TYPE(6'b 1010_01),
		.PULLUP(1'b 0)
	) io_buf [7:0] (
		.PACKAGE_PIN({D0, D1, D2, D3, D4, D5, D6, D7}),
		.OUTPUT_ENABLE({sram_oe, sram_oe, sram_oe, sram_oe, sram_oe, sram_oe, sram_oe, sram_oe}),
		.D_IN_0({sram_din[0], sram_din[1], sram_din[2], sram_din[3], sram_din[4], sram_din[5], sram_din[6], sram_din[7]}),
		.D_OUT_0({sram_dout[0], sram_dout[1], sram_dout[2], sram_dout[3], sram_dout[4], sram_dout[5], sram_dout[6], sram_dout[7]})
	);

endmodule
