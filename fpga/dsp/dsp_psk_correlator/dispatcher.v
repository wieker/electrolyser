module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output reg rdy,
);
    wire i_code, q_code;
    nco i_nco(.clk(clk), .rst(rst_in), .control_word(12'h100), .i_code(i_code), .phase_control_word(12'h000));
    nco q_nco(.clk(clk), .rst(rst_in), .control_word(12'h100), .i_code(q_code), .phase_control_word(12'h400));

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

    reg [7:0] counter;

    always@(posedge clk)
    begin
        if (rst_in) begin
            rdy <= 0;
            st1 <= 0;
        end else if (stb) begin
            q1 <= i_value[7];
            st1 <= 1;
        end else if (st1) begin
            if (q2 == q1) begin
                counter ++;
            end else begin
                q2 <= q1;
                value <= counter;
                rdy <= counter > 50;
                counter <= 1;
            end
            st1 <= 0;
        end else begin
            rdy <= 0;
        end
    end


endmodule