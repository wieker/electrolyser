module dispatcher(
    input clk, rst_in, sig,
    output [7:0] value,
    output rdy,
);
    wire [7:0] lvalue;
    wire [7:0] rvalue;
    reg [7:0] lvalue_reg;
    reg [7:0] rvalue_reg;
    reg st;

    reg [7:0] phase;

    iq_demod early(.clk(clk), .rst_in(rst_in), .sig(sig), .rdy(lrdy), .value(lvalue), .phase(phase + 16'hffff));
    iq_demod late(.clk(clk), .rst_in(rst_in), .sig(sig), .rdy(lrdy), .value(rvalue), .phase(phase + 16'hffff));
    psk_demod actual(.clk(clk), .rst_in(rst_in), .sig(sig), .rdy(rdy), .value(value), .phase(phase)); //PSK modem

    always @(posedge clk) begin
        if (rdy) begin
            lvalue_reg <= lvalue;
            rvalue_reg <= rvalue;
            st <= lvalue > rvalue;
            if (st) begin
                phase <= phase + 16'hffff;
            end else begin
                phase <= phase + 16'h0010;
            end
        end
    end


endmodule