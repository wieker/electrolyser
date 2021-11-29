module iq_demod(
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
    reg st2;
    reg [7:0] q1;
    reg [7:0] q2;
    reg [7:0] q3;
    reg [7:0] q4;

    always@(posedge clk)
    begin
        if (rst_in) begin
            rdy <= 0;
            st1 <= 0;
            st2 <= 0;
        end else if (stb) begin
            q1 <= i_value[7] ? i_value : ~ i_value;
            q2 <= q_value[7] ? q_value : ~ q_value;
            st1 <= 1;
        end else if (st1) begin
            q3 <= {0, q1[6:0]};
            q4 <= {0, q2[6:0]};
            st1 <= 0;
            st2 <= 1;
        end else if (st2) begin
            //value <= q3;  // only I
            value <= q3 + q4;
            rdy <= 1;
            st1 <= 0;
            st2 <= 0;
        end else begin
            rdy <= 0;
            st1 <= 0;
            st2 <= 0;
        end
    end


endmodule