module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output reg rdy,
);
    wire i_code, q_code;
    nco i_nco(.clk(clk), .rst(rst_in), .control_word(16'h0ff8 + offset), .i_code(i_code), .phase_control_word(16'h0000));
    nco q_nco(.clk(clk), .rst(rst_in), .control_word(16'h0ff8 + offset), .i_code(q_code), .phase_control_word(16'h4000));

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
    reg st2;
    reg q1;
    reg q2;
    reg q3;
    reg q4;

    reg i_q;

    reg [7:0] counter_i;
    reg [7:0] counter_q;

    reg [19:0] summ;

    reg [15:0] summ_counter;
    reg [3:0] offset;

    always@(posedge clk)
    begin
        if (rst_in) begin
            rdy <= 0;
            st1 <= 0;
        end else if (stb) begin
            q1 <= i_value[7];
            q3 <= q_value[7];
            st1 <= 1;
        end else if (st1) begin
            if (q2 == q1) begin
                counter_i <= counter_i + 1;;
            end
            if (q4 == q3) begin
                counter_q <= counter_q + 1;
            end
            if (q2 != q1) begin
                q2 <= q1;
                counter_i <= 1;
            end
            if (q4 != q3) begin
                q4 <= q3;
                counter_q <= 1;
            end
            if ((q4 != q3) && i_q) begin
                summ <= summ + counter_q;
                i_q <= 0;
                summ_counter <= summ_counter + 1;
            end else if ((q2 != q1) && !i_q) begin
                summ <= summ + counter_i;
                i_q <= 1;
                summ_counter <= summ_counter + 1;
            end
            st1 <= 0;
        end else if (summ_counter == 256 * 16) begin
            summ_counter <= 0;
            summ <= 0;
            value <= summ[19:12];
            rdy <= 1;
            offset <= offset + 1;
            st2 <= 1;
        end else if (st2) begin
            st2 <= 0;
            value <= offset;
        end else begin
            rdy <= 0;
        end
    end


endmodule