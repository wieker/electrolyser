module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output stb
);
    reg [0:15] base_sig;
    wire code;
    always@(posedge clk)
    begin
        if (rst_in) begin
            base_sig <= 16'h00ff;
        end else begin
            base_sig <= {base_sig[1:15], code};
        end
    end

    wire i_code, q_code;
    nco nco(.clk(clk), .rst(rst_in), .control_word(13'b0001000000000), .i_code(i_code), .q_code(q_code));

    wire [7:0] i_value;
    wire [7:0] q_value;
    correlator i_correlator(.clk(clk), .rst(rst), .sig(sig), .code(i_code), .value(i_value));
    correlator q_correlator(.clk(clk), .rst(rst), .sig(sig), .code(q_code), .value(q_value));

    wire rst;
    dispatcher_ctl ctl(
        .clk(clk),
        .rst_in(rst_in),
        .rst_out(rst),
        .stb(stb)
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