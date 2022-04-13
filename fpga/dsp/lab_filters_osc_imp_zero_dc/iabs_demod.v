module iabs_demod(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output reg rdy,
    input [15:0] phase,
);
    wire i_code;
    nco i_nco(.clk(clk), .rst(rst_in), .control_word(16'h1000), .i_code(i_code), .phase_control_word(16'h0000 + phase));

    wire [7:0] i_value;
    correlator i_correlator(.clk(clk), .rst(rst), .sig(sig), .code(i_code), .value(i_value));

    wire rst;
    dispatcher_ctl ctl(
        .clk(clk),
        .rst_in(rst_in),
        .rst_out(rst),
        .stb(stb)
    );

    wire stb;
    reg st1;
    reg [7:0] q1;

    always@(posedge clk)
    begin
        if (rst_in) begin
            rdy <= 0;
            st1 <= 0;
        end else if (stb) begin
            q1 <= i_value[7] ? i_value : ~ i_value;
            st1 <= 1;
        end else if (st1) begin
            value <= {0, q1[6:0]};
            rdy <= 1;
            st1 <= 0;
        end else begin
            rdy <= 0;
        end
    end


endmodule