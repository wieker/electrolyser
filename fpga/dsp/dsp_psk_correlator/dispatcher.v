module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output reg rdy,
);
    wire i_code, q_code, ref;
    nco ref_nco(.clk(clk), .rst(rst_in), .control_word(12'h101), .i_code(ref), .phase_control_word(12'h000));
    nco i_nco(.clk(clk), .rst(rst_in), .control_word(12'h100), .i_code(i_code), .phase_control_word(12'h000));
    nco q_nco(.clk(clk), .rst(rst_in), .control_word(12'h100), .i_code(q_code), .phase_control_word(12'h200));

    wire [7:0] i_value;
    wire [7:0] q_value;
    correlator i_correlator(.clk(clk), .rst(rst), .sig(ref), .code(i_code), .value(i_value));
    correlator q_correlator(.clk(clk), .rst(rst), .sig(ref), .code(q_code), .value(q_value));

    wire rst;
    dispatcher_ctl ctl(
        .clk(clk),
        .rst_in(rst_in),
        .rst_out(rst),
        .stb(stb)
    );

    wire stb;
    reg st1;
    reg [7:0] tmp;

    always@(posedge clk)
    begin
        if (rst_in) begin
            rdy <= 0;
        end else if (stb) begin
            rdy <= 1;
            tmp <= q_value;
            value <= st1 ? i_value : q_value;
            st1 <= !st1;
        end else begin
            rdy <= 0;
        end
    end


endmodule