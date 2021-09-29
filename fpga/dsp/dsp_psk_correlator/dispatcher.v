module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output stb,
);
    reg [0:15] base_sig;
    always@(posedge clk)
    begin
        if (rst_in) begin
            base_sig <= 16'h00ff;
        end else if (stb) begin
            if (!left) begin
                base_sig <= {base_sig[2:15], base_sig[0], base_sig[1]};
            end else if (right) begin
                base_sig <= {base_sig[1:15], base_sig[0]};
            end else if (!right) begin
                base_sig <= base_sig;
            end
        end else begin
            base_sig <= {base_sig[1:15], base_sig[0]};
        end
    end

    wire left = value[2];
    wire right = value[4];
    wire [7:0] rdys;
    genvar j;
    for (j=0; j < 8; j++) begin
        correlator correlator(.clk(clk), .rst(rst), .sig(sig), .code(base_sig[2 * j]), .match(rdys[j]));
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