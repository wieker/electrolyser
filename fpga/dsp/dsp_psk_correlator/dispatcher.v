module dispatcher(
    input clk, rst_in, sig,
    output lock,
    output stb,
);
    reg [15:0] base_sig;
    wire left, right, center;
    wire left_flag, right_flag, center_flag;
    always@(posedge clk)
    begin
        if (rst_in) begin
            base_sig <= 16'h00ff;
        end else if (rst) begin
            if ((left == 0) || (center == 0)) begin
            end else if (right == 0) begin
                base_sig <= {base_sig[13:0], base_sig[15], base_sig[14]};
            end else begin
                base_sig <= {base_sig[14:0], base_sig[15]};
            end
        end else begin
            base_sig <= {base_sig[14:0], base_sig[15]};
        end
    end

    correlator correlator_left(.clk(clk), .rst(rst), .sig(sig), .code(base_sig[0]), .match(left_flag));
    correlator correlator(.clk(clk), .rst(rst), .sig(sig), .code(base_sig[2]), .match(center_flag));
    correlator correlator_right(.clk(clk), .rst(rst), .sig(sig), .code(base_sig[4]), .match(right_flag));

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