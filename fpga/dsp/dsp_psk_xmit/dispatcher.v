module dispatcher(
    input clk, rst_in, sig,
    output [7:0] value,
    output stb,
);
    reg [0:5] base_sig;
    always@(posedge clk)
    begin
        if (rst_in) begin
            base_sig <= 6'b000111;
        end else begin
            base_sig <= {base_sig[1:5], base_sig[0]};
        end
    end

    wire [7:0] rdys;
    genvar j;
    for (j=0; j < 6; j++) begin
        correlator correlator(.clk(clk), .rst(rst), .sig(sig), .code(base_sig[j]), .match(rdys[j]));
    end

    wire rst;
    dispatcher_ctl ctl(
        .clk(clk),
        .rst_in(rst_in),
        .rst_out(rst),
        .stb(stb),
    );

    reg [7:0] temp;

    always@(posedge clk)
    begin
        if (rst) begin
            value <= temp;
            temp <= 0;
        end else begin
            temp <= temp | rdys;
        end
    end


endmodule