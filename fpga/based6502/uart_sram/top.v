module top(
    input  RX,
    output TX,

    output LED1, LED2, LED3,

    A0, A1, A2, A3, A4,
    A5, A6, A7, A8, A9,
    A10, A11, A12, A13, A14,
    A15, A16, A17, A18,
    CEn, WEn, OEn,
    inout D0, D1, D2, D3,
    inout D4, D5, D6, D7,

    input CLK,
);
	// reset generator waits > 10us
	reg [7:0] reset_cnt;
	reg reset;
    wire clk;

    wire sram_oe;
    wire [7:0] sram_din;
    wire [7:0] sram_dout;
    wire [15:0] addr;
    reg [7:0] adc_reg;

	initial
        reset_cnt <= 8'h00;

    assign clk = CLK;

    assign CEn = 0;
    assign OEn = 0;
    assign WEn = !sram_oe;

    assign A0 = addr[0];
    assign A1 = addr[1];
    assign A2 = addr[2];
    assign A3 = addr[3];
    assign A4 = addr[4];
    assign A5 = addr[5];
    assign A6 = addr[6];
    assign A7 = addr[7];
    assign A8 = addr[8];
    assign A9 = addr[9];
    assign A10 = addr[10];
    assign A11 = addr[11];
    assign A12 = addr[12];
    assign A13 = addr[13];
    assign A14 = addr[14];
    assign A15 = addr[15];
    assign A16 = 0;
    assign A17 = 0;
    assign A18 = 0;
    
	always @(posedge clk)
	begin
		if(reset_cnt != 8'hff)
        begin
            reset_cnt <= reset_cnt + 8'h01;
            reset <= 1'b1;
        end
        else
            reset <= 1'b0;
	end
    
	// test unit
	wire [7:0] gpio_o, gpio_i;
	assign gpio_i = 8'h00;
	logic logic(
		.clk(clk),
		.reset(reset),
		
		.gpio_o(gpio_o),
		.gpio_i(gpio_i),
		
		.RX(RX),
		.TX(TX),

		.sram_oe(sram_oe),
		.sram_din(sram_din),
		.sram_dout(sram_dout),
		.addr(addr)
	);
    
	// drive LEDs from GPIO
	assign {LED1, LED2, LED3} = gpio_o[7:5];

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
