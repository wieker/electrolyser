module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] i_value_reg,
    output reg [7:0] q_value_reg,
    output reg rdy,
);
    wire i_code, q_code;
    nco i_nco(.clk(clk), .rst(rst_in), .control_word(16'h4000), .i_code(i_code), .phase_control_word(16'h0000));
    nco q_nco(.clk(clk), .rst(rst_in), .control_word(16'h4000), .i_code(q_code), .phase_control_word(16'h4000));

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

    wire [7:0] z_value;
    correlator zero_correlator(.clk(clk), .rst(rst), .sig(sig), .code(0), .value(z_value));

    wire stb;

    reg [3:0] pipeline;
    always@(posedge clk)
    begin
        if (rst_in) begin
            rdy <= 0;
        end else if (stb) begin
            i_value_reg <= i_value;
            q_value_reg <= q_value;
            rdy <= 1;
            pipeline[0] <= 1;
        end else if (pipeline[0]) begin
            pipeline[0] <= 0;
            i_value_reg <= z_value;
            q_value_reg <= 0;
        end else begin
            rdy <= 0;
        end
    end


endmodule