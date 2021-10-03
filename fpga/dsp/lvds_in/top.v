module top(
    output LED1, LED2, fpga_tx,
    input lvds_in
);

    wire comp_in;

	SB_IO #(
		.PIN_TYPE(6'b000001),
		.IO_STANDARD("SB_LVDS_INPUT")
	) lp_compare (
		.PACKAGE_PIN(lvds_in),
		.D_IN_0(comp_in)
    );

    wire clk;

    SB_HFOSC inthosc (
      .CLKHFPU(1'b1),
      .CLKHFEN(1'b1),
      .CLKHF(clk)
    );
    reg [1:0] digitizer;

    wire sig = digitizer[1];
    assign LED1 = sig;

    reg [7:0] shift;

    wire code = shift[0];

    reg [13:0] match_counter;
    reg [13:0] match_stage;
    reg sig_buf;
    reg code_buf;
    reg match;
    reg [15:0] counter;
    wire [15:0] next = counter + 1;

    reg temp;
    reg value;

    always@(posedge clk)
    begin
      digitizer <= { digitizer[0], comp_in };
        shift <= { shift[6:0], !shift[7] };
        match_stage <= match_counter;
        match <= (match_stage[10] == 1) && (match_stage[8] == 1);
        sig_buf <= sig;
        code_buf <= code;
        if (next[11] == 1) begin
            value <= temp;
            temp <= 0;
            match_counter <= (code_buf == sig_buf);
            counter <= 0;
        end else begin
            match_counter <= match_counter + (code_buf == sig_buf);
            counter <= next;
            temp <= temp | match;
        end
    end

    assign LED2 = value;


endmodule
