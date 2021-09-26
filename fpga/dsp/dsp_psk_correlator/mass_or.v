module mass_or(
    output val,
    input [WIDTH:0] inp,
);

    parameter WIDTH = 8;

    wire values[WIDTH];
    assign values[0] = inp[0];
    genvar j;
    for (j=1; j < WIDTH; j++) begin
        assign values[j] = values[j - 1] | inp[j];
    end

    assign val = values[WIDTH - 1];

endmodule