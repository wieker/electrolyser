module dispatcher(
    input clk, rst, sig,
    output fpga_tx,
);

    wire codes[10];
    wire rdy[10];
    wire [7:0] results[10];
    wire ready = rdy_tmp[9];
    genvar j;

    for (j=0; j < 8; j++) begin
	    sig_source sig_source(.clk(clk), .rst(rst), .period0(48 * 1024), .period1(48*1024), .phase(j * 6 * 1024), .start_code(0), .code(codes[j]));
    end
    sig_source sig_source0(.clk(clk), .rst(rst), .period0(48 * 1024), .period1(1), .phase(0), .start_code(0), .code(codes[8]));
    sig_source sig_source1(.clk(clk), .rst(rst), .period0(1), .period1(48 * 1024), .phase(0), .start_code(1), .code(codes[9]));

    for (j=0; j < 10; j++) begin
        correlator correlator(.clk(clk), .rst(rst), .sig(sig_in), .code(codes[j]), .rdy(rdy[j]), .result(results[j]));
    end

    reg [3:0] state;
    reg tx_start;
    reg [7:0] symb;
    reg started;

    always@(posedge clk)
    begin
        started <= ready | started;
        if (rst || !started) begin
            state <= 0;
        end else if (state == 10) begin
        end else if (tx_busy == 0) begin
            tx_start <= 1;
            symb <= results[state];
            state <= state + 1;
        end else begin
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
        .tx_dat(symb),           // transmit data byte
        .tx_start(tx_start),    // trigger transmission
        .tx_serial(fpga_tx),         // tx serial output
        .tx_busy(tx_busy)       // tx is active (not ready)
    );

    genvar j;
    wire rdy_tmp[10];
    for (j=0; j < 10; j++) begin
        if (j == 0)
            assign rdy_tmp[j] = rdy[0];
        else
            assign rdy_tmp[j] = rdy[j] | rdy_tmp[j - 1];
    end

endmodule