module dispatcher(
    input clk, rst_in, sig,
    output [7:0] value,
    output rdy,
);
    wire [7:0] lvalue;
    wire [7:0] rvalue;
    wire [7:0] avalue;
    reg [7:0] lvalue_reg;
    reg [7:0] rvalue_reg;
    reg [7:0] avalue_reg;

    reg [15:0] phase;

    iabs_demod early(.clk(clk), .rst_in(rst_in), .sig(sig), .rdy(lrdy), .value(lvalue), .phase(phase + 16'hf000));
    iabs_demod late(.clk(clk), .rst_in(rst_in), .sig(sig), .rdy(rrdy), .value(rvalue), .phase(phase + 16'h1000));
    iabs_demod amod(.clk(clk), .rst_in(rst_in), .sig(sig), .rdy(qrdy), .value(avalue), .phase(phase));
    psk_demod actual(.clk(clk), .rst_in(rst_in), .sig(sig), .rdy(rdy), .value(value), .phase(phase)); //PSK modem

    always @(posedge clk) begin
        if (rdy) begin
            lvalue_reg <= lvalue;
            rvalue_reg <= rvalue;
            avalue_reg <= avalue;
            if (lvalue > avalue) begin
                phase <= phase + 16'hf000;
            end else if (rvalue > avalue) begin
                phase <= phase + 16'h1000;
            end
        end
    end


endmodule