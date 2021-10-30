module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] value,
    output reg rdy,
);
    wire i_code;
    nco i_nco(.clk(clk), .rst(rst_in), .control_word(12'h100), .i_code(i_code), .phase_control_word(12'h000));

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
    wire bi = (i_value >= 8'ha0);

    reg [7:0] counter_low;
    reg [7:0] counter_up;
    reg up;

    always@(posedge clk)
    begin
        rbi <= bi;
        rbq <= bq;
        if (rst_in) begin
            rdy <= 0;
            st1 <= 0;
        end else if (stb) begin
            if (bi && !up) begin
                up <= 1;
                counter_up <= 1;
                counter_low <= 0;
                value <= counter_low;
                rdy <= 1;
            end else if (counter_up == 8'h20) begin
                counter_up <= 0;
                up <= 0;
            end else if (up) begin
                counter_up <= counter_up + 1;
            end else begin
                counter_low <= counter_low + 1;
            end
        end else begin
            rdy <= 0;
        end
    end


endmodule