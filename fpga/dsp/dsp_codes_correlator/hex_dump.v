module hex_dump(
    input clk, rst, sig, fpga_rx,
    output fpga_tx, rdy3, rdy4,
);

    assign rdy4 = value[3];
    assign rdy3 = sig;
    wire [7:0] i_value;
    wire [7:0] q_value;

    dispatcher dispatcher(.clk(clk), .rst_in(rst), .sig(sig), .rdy(stb),
        .i_value_reg(i_value), .q_value_reg(q_value));

    reg [16:0] ram_addr;
    wire [15:0] ram_data_in = {q_value[7:0], i_value[7:0]};
    wire [15:0] ram_data_out;
    wire [15:0] ram_data_out_tmp[4];
    wire ram_wren = !ram_addr[16] && stb;

    genvar i;
    for (i = 0; i < 4; i ++) begin
        SB_SPRAM256KA spram
        (
            .ADDRESS(ram_addr[13:0]),
            .DATAIN(ram_data_in),
            .MASKWREN({ram_wren, ram_wren, ram_wren, ram_wren}),
            .WREN(ram_wren),
            .CHIPSELECT(i == ram_addr[15:14]),
            .CLOCK(clk),
            .STANDBY(1'b0),
            .SLEEP(1'b0),
            .POWEROFF(1'b1),
            .DATAOUT(ram_data_out_tmp[i])
        );
    end


	always @(*)
		casez(ram_addr[15:14])
			0: ram_data_out = ram_data_out_tmp[0];
			1: ram_data_out = ram_data_out_tmp[1];
			2: ram_data_out = ram_data_out_tmp[2];
			3: ram_data_out = ram_data_out_tmp[3];
		endcase

    reg tx_start;
    reg [7:0] touart;
    reg bugfix001;
    reg part;

    always@(posedge clk)
    begin
        if (ram_wren) begin
            ram_addr <= ram_addr + 1;
        end else if (ram_addr[16] && !tx_busy && !bugfix001) begin
            bugfix001 <= 1;
            tx_start <= 1;
            touart <= part ? ram_data_out[15:8] : ram_data_out[7:0];
            part = ~ part;
            if (part) begin
                ram_addr <= ram_addr + 1;
            end
        end else begin
            if (!tx_busy && bugfix001) begin
                bugfix001 <= 0;
            end
            tx_start <= 0;
        end
    end

    wire tx_busy;
    localparam sym_rate = 1200;
    localparam clk_freq = 48000000;
    localparam sym_cnt = clk_freq / sym_rate;
    localparam SCW = $clog2(sym_cnt);

    acia_tx #(
        .SCW(SCW),              // rate counter width
        .sym_cnt(sym_cnt)       // rate count value
    )
    my_tx(
        .clk(clk),				// system clock
        .rst(rst),			// system reset
        .tx_dat(touart),           // transmit data byte
        .tx_start(tx_start),    // trigger transmission
        .tx_serial(fpga_tx),         // tx serial output
        .tx_busy(tx_busy)       // tx is active (not ready)
    );

endmodule