

module fifo(input wire clk, reset, wr, rd, input wire[7:0] din, output wire empty, full, output wire[7:0] dout);
    wire [3:0] rd_addr, wr_addr;
    wire [3:0] rd_inc, wr_inc;
    wire [15:0] fakeDout;
    wire wr_load, rd_load;

    assign wr_inc = wr_addr + 1;
    assign rd_inc = rd_addr + 1;
    wire empty = (rd_addr == wr_addr);
    wire full = (rd_addr == wr_inc);

    assign rd_load = rd && !empty;
    assign wr_load = wr && !full;

    reg [3:0] rd_addr;
    reg [3:0] wr_addr;
    always@(posedge clk)
    begin
        if (reset) begin
            rd_addr <= 0;
            wr_addr <= 0;
        end else begin
            if (rd_load) begin
                rd_addr <= rd_inc;
            end
            if (wr_load) begin
                wr_addr <= wr_inc;
            end
        end
    end

    reg [7:0] regs[16];
    assign dout = regs[rd_addr];
    integer i;
    always@(posedge clk)
    begin
        if (wr_load) begin
            for (i = 0; i < 16; i ++) begin
                if (wr_addr == i) begin
                    regs[i] <= din;
                end
            end
        end
    end
endmodule
