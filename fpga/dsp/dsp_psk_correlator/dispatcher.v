module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output reg rdy,
);
    wire i_code, q_code;
    nco i_nco(.clk(clk), .rst(rst_in), .control_word(16'h1001), .i_code(i_code), .phase_control_word(16'h0000));
    nco q_nco(.clk(clk), .rst(rst_in), .control_word(16'h1001), .i_code(q_code), .phase_control_word(16'h4000));

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

    wire stb;
    reg st1;
    reg q1;
    reg q2;
    reg q3;
    reg q4;

    reg i_q;
    reg [7:0] res_counter;
    reg [7:0] pulse_counter;

    always@(posedge clk)
    begin
        if (rst_in) begin
            rdy <= 0;
            st1 <= 0;
        end else if (stb) begin
            q1 <= i_value[7];
            q3 <= q_value[7];
            st1 <= 1;
            res_counter ++;
        end else if (st1) begin
            if (q2 != q1) begin
                q2 <= q1;
            end
            if (q4 != q3) begin
                q4 <= q3;
            end
            if ((q4 != q3) && i_q) begin
                i_q <= 0;
                pulse_counter <= pulse_counter + 1;
            end else if ((q2 != q1) && !i_q) begin
                i_q <= 1;
                pulse_counter <= pulse_counter + 1;
            end
            st1 <= 0;
        end else if (res_counter == 0) begin
            res_counter <= 1;
            value <= pulse_counter;
            pulse_counter <= 0;
            rdy <= 1;
        end else begin
            rdy <= 0;
        end
    end


endmodule