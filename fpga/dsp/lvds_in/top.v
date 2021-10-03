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

    always@(posedge clk)
    begin
      digitizer <= { digitizer[0], comp_in };
    end

    wire sig = digitizer[1];
    assign LED1 = sig;

    reg [15:0] shift;

    initial begin
        shift = 16'h00ff;
    end

    always@(posedge clk)
    begin
        shift <= { shift[14:0], shift[15] };
    end

    wire code = shift[15];

    reg [13:0] match_counter;
    reg [13:0] match_stage;
    reg sig_buf;
    reg code_buf;
    reg match;
    reg [15:0] counter;
    wire [15:0] next = counter + 1;

    always @(posedge clk)
    begin
        match_stage <= match_counter;
        match <= (match_stage[10] == 1) && (match_stage[8] == 1);
        sig_buf <= sig;
        code_buf <= code;
        if (next[11] == 1) begin
            match_counter <= (code_buf == sig_buf);
            counter <= 0;
        end else begin
            match_counter <= match_counter + (code_buf == sig_buf);
            counter <= next;
        end
    end

    reg temp;
    reg value;

    always@(posedge clk)
    begin
        if (next[11] == 1) begin
            value <= temp;
            temp <= 0;
        end else begin
            temp <= temp | match;
        end
    end

    assign LED2 = value;


endmodule
