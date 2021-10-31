module dispatcher(
    input clk, rst_in, sig,
    output [7:0] value,
    output rdy,
);

    reg [4:0] offsetl;
    reg [4:0] offsetr;
    reg [4:0] offset;

    wire am_lstb;
    wire am_rstb;
    wire [7:0] lvalue;
    wire [7:0] rvalue;
    wire [7:0] value1;
    wire [7:0] value2;
    wire rdy1;
    wire rdy2;
    assign rdy = rdy1 | rdy2;
    assign value = rdy1 ? value1 : value2;
    wire rst, am_stb;
    am_detector left(.clk(clk), .rst_in(rst), .sig(sig), .offset(offsetl), .rdy(am_stb), .value(lvalue));
    am_detector right(.clk(clk), .rst_in(rst), .sig(sig), .offset(offsetr), .rdy(am_rstb), .value(rvalue));
    psk_demod demod1(.clk(clk), .rst_in(rst), .sig(sig), .offset(offset), .rdy(rdy1), .value(value1));
    psk_demod demod2(.clk(clk), .rst_in(rst), .sig(sig), .offset(19), .rdy(rdy2), .value(value2));


    always@(posedge clk)
    begin
        if (rst_in) begin
            offsetl <= 12;
            offsetr <= 20;
            offset <= 16;
        end else if (am_lstb | am_rstb) begin
            if (lvalue > rvalue) begin
                offsetl <= offset + 1;
                offsetr <= offsetr + 1;
                offset <= offset + 1;
            end else if (lvalue < rvalue) begin
                offsetl <= offsetl + 5'b11111;
                offsetr <= offsetr + 5'b11111;
                offset <= offset + 5'b11111;
            end
        end
    end


endmodule