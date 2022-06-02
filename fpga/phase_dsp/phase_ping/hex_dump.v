module hex_dump(
    input clk, rst, sig, fpga_rx,
    output fpga_tx, rdy3, rdy4,
);

    // PWM => LVDS ADC (1-bit comparator)

    assign rdy4 = done;
    assign rdy3 = demod[7] | (demod[6] && demod[5]);

    reg [15:0] value;
    reg [15:0] bckp;
    reg [3:0] cntr;
    reg stb;
    wire [7:0] demod;
    dispatcher dispatcher(.clk(clk), .rst_in(rst), .sig(sig), .value(demod));

    always@(posedge clk)
    begin
        if (!rst) begin
            cntr <= cntr + 1;
            value <= {value[14:0], sig};
            if (cntr == 0) begin
                bckp <= value;
                stb <= 1;
            end else begin
                stb <= 0;
            end
        end
    end

    reg [7:0] ram_addr;
    wire [15:0] ram_data_in = {bckp};
    wire [15:0] ram_data_out;
    wire ram_wren = !tx && stb;

    SB_RAM40_4K SB_RAM40_4K_inst (
        .RDATA(ram_data_out),
        .RADDR(ram_addr),
        .RCLK(clk),
        .RCLKE(1),
        .RE(1),
        .WADDR(ram_addr),
        .WCLK(clk),
        .WCLKE(1),
        .WDATA(ram_data_in),
        .WE(ram_wren)
    );

    reg tx_start;
    reg [7:0] touart;
    reg bugfix001;
    reg part;

    reg tx;
    reg got;
    reg [8:0] counter;
    reg [7:0] addr_save;
    reg [16:0] result_cnt;
    reg p;

    always@(posedge clk)
    begin
        p <= fpga_rx;
        if (fpga_rx) begin
            result_cnt <= result_cnt + 1;
        end
        if (!fpga_rx && p) begin
            tx <= 1;
        end
        if (tx && !tx_busy && !bugfix001) begin
            bugfix001 <= 1;
            tx_start <= 1;
            touart <= part ? result_cnt[15:8] : result_cnt[7:0];
            part = ~ part;
            if (part) begin
                tx <= 0;
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