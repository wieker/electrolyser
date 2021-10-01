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

    correlator correlator(.clk(clk), .rst(rst), .sig(sig), .code(base_sig[3]), .value(value));

    wire rst;
    dispatcher_ctl ctl(
        .clk(clk),
        .rst_in(rst_in),
        .rst_out(rst),
        .stb(stb),
    );


endmodule