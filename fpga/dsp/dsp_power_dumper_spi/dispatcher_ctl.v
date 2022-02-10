module dispatcher_ctl(
    input clk,
    input rst_in,
    output rst_out,
    output reg stb,
);


    wire rst_out = rst_in | capture;

    wire capture;
    wire [8:0] next_ctr;
    assign capture = next_ctr[8] == 1;
    assign next_ctr = ctr + 1;

    reg [7:0] ctr;
    always@(posedge clk)
    begin
        if (rst_out) begin
            ctr <= 0;
            stb <= 1;
        end else begin
            ctr <= next_ctr;
            stb <= 0;
        end
    end

endmodule