module dispatcher(
    input clk, rst_in, sig,
    output lock,
    output stb,
);
    reg [0:15] base_sig;
    wire left, right, center;
    wire left_flag, right_flag, center_flag;
    always@(posedge clk)
    begin
        if (rst_in) begin
            base_sig <= 16'h00ff;
        end else if (rst) begin
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

    correlator correlator_left(.clk(clk), .rst(rst), .sig(sig), .code(base_sig[6]), .match(left_flag));
    correlator correlator(.clk(clk), .rst(rst), .sig(sig), .code(base_sig[8]), .match(center_flag));
    correlator correlator_right(.clk(clk), .rst(rst), .sig(sig), .code(base_sig[10]), .match(right_flag));

    wire rst;
    dispatcher_ctl ctl(
        .clk(clk),
        .rst_in(rst_in),
        .rst_out(rst),
        .stb(stb),
    );

    flag_holder fhl(
        .clk(clk),
        .set(rst),
        .inp(left_flag),
        .val(left),
    );
    flag_holder fhc(
        .clk(clk),
        .set(rst),
        .inp(center_flag),
        .val(center),
    );
    flag_holder fhr(
        .clk(clk),
        .set(rst),
        .inp(right_flag),
        .val(right),
    );

    assign lock = center;


endmodule