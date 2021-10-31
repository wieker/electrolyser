module dispatcher(
    input clk, rst_in, sig,
    output [7:0] value,
    output rdy,
);

    reg [4:0] offset;

    wire am_lstb;
    wire am_rstb;
    wire [7:0] lvalue;
    wire [7:0] rvalue;
    am_detector left(.clk(clk), .rst_in(rst), .sig(sig), .offset(offset), .rdy(am_stb), .value(lvalue));
    am_detector right(.clk(clk), .rst_in(rst), .sig(sig), .offset(offset), .rdy(am_rstb), .value(rvalue));
    psk_demod demod(.clk(clk), .rst_in(rst), .sig(sig), .offset(offset), .rdy(rdy), .value(value));


    always@(posedge clk)
    begin
        if (rst_in) begin
            offset <= 16;
        end else if (am_lstb | am_rstb) begin
            if (lvalue > rvalue) begin
                offset <= offset + 1;
            end else if (lvalue < rvalue) begin
                offset <= offset + 5'b11111;
            end
        end
    end


endmodule