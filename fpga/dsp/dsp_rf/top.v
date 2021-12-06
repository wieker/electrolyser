module top(
    output LED1, led2, led3, rf,
    input CLK1, CLK2, fpga_rx
);

    reg [21:0] counter1;

    always @(posedge CLK1)
        begin
            counter1 <= counter1 + 1;
        end

    reg [24:0] counter2;

    always @(posedge CLK2)
        begin
            counter2 <= counter2 + 1;
        end

    assign LED1 = counter2[24];
    assign led2 = counter1[21];
    assign led3 = counter1[19];

    reg select;
    reg [17:0] phase;

    always @(posedge q_code)
        begin
            phase <= phase + 1;
        end

    nco i_nco(.clk(CLK1), .rst(0), .control_word(16'h4000), .i_code(i_code), .phase_control_word(16'h0000));
    nco q_nco(.clk(CLK1), .rst(0), .control_word(16'h0001), .i_code(q_code), .phase_control_word(16'h0000));

    assign rf = i_code ^ phase[7];

    always @(posedge CLK2)
    begin
        if (rx_stb) begin  // cross-clock domain :)
            select <= !select;
        end
    end


    localparam sym_rate = 1200;
    localparam clk_freq = 4000000;
    localparam sym_cnt = clk_freq / sym_rate;
    localparam SCW = $clog2(sym_cnt);

    wire rx_stb;
    wire [7:0] rx_dat;

    acia_rx #(
        .SCW(SCW),				// rate counter width
        .sym_cnt(sym_cnt)		// rate count value
    )
    my_rx(
        .clk(CLK2),				// system clock
        .rst(counter2[24:2] == 0),			// system reset
        .rx_serial(fpga_rx),		    // raw serial input
        .rx_dat(rx_dat),        // received byte
        .rx_stb(rx_stb),        // received data available
        .rx_err(rx_err)         // received data error
    );

endmodule
