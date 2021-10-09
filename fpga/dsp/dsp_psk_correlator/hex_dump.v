module hex_dump(
    input clk, rst, sig, fpga_rx,
    output fpga_tx, rdy3, rdy4
);

    wire lock;
    assign rdy4 = rx_stb;
    assign rdy3 = sig;
    wire [7:0] value;
    dispatcher dispatcher(.clk(clk), .rst_in(rst), .sig(sig), .stb(stb), .value(value));

    reg tx_start;
    wire empty;
    wire full;
    wire [7:0] touart;
    fifo fifo(.clk(clk), .reset(rst), .wr(stb && phase), .rd(tx_start), .din(value), .empty(empty), .full(full), .dout(touart));
    reg [24:0] counter;
    reg phase;
    reg bugfix001;

    reg [1:0] rx_stage;
    reg [15:0] dt;
    reg reg_we;

    always@(posedge clk)
    begin
        counter <= counter + 1;
        if (full) begin
            phase <= 0;
        end
        if ((!empty) && (!tx_busy) && !phase && !bugfix001) begin
            bugfix001 <= 1;
            tx_start <= 1;
        end else begin
            if ((!empty) && (!tx_busy) && !phase && bugfix001) begin
                bugfix001 <= 0;
            end
            tx_start <= 0;
        end
        if (rx_stb && (rx_stage == 0)) begin
            //rx_stage <= 1;
            dt <= {dt[7:0], rx_dat};
            phase <= 1;
        end else if (rx_stb && (rx_stage == 1)) begin
            rx_stage <= 2;
            dt <= {dt[7:0], rx_dat};
        end else if (rx_stage == 2) begin
            rx_stage <= 0;
            if (dt[2] == 1) begin
                phase <= 1;
            end else begin
                reg_we <= 1;
            end
        end else begin
            reg_we <= 0;
        end
    end

    wire tx_busy;
    localparam sym_rate = 1000000;
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

    wire rx_stb;
    wire [7:0] rx_dat;

    acia_rx #(
        .SCW(SCW),				// rate counter width
        .sym_cnt(sym_cnt)		// rate count value
    )
    my_rx(
        .clk(clk),				// system clock
        .rst(rst),			// system reset
        .rx_serial(fpga_rx),		    // raw serial input
        .rx_dat(rx_dat),        // received byte
        .rx_stb(rx_stb),        // received data available
        .rx_err(rx_err)         // received data error
    );

endmodule