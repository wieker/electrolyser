module psk_demod(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output reg rdy,
    input [15:0] phase,
);
    wire i_code, q_code;
    nco i_nco(.clk(clk), .rst(rst_in), .control_word(16'h1000), .i_code(i_code), .phase_control_word(16'h0000 + phase));
    nco q_nco(.clk(clk), .rst(rst_in), .control_word(16'h1000), .i_code(q_code), .phase_control_word(16'h4000 + phase));

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
            pulse_counter <= pulse_counter + q1;
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