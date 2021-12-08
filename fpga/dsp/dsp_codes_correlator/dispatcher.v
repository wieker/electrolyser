module dispatcher(
    input clk, rst_in, sig,
    output reg [7:0] i_value_reg,
    output reg [7:0] q_value_reg,
    output reg rdy,
);

    wire [7:0] value[6];
    wire rdys[6];
    genvar i;
    for (i = 0; i < 6; i ++) begin
        iq_demod i0(.clk(clk), .rst_in(rst_in), .sig(sig ^ code[i]), .rdy(rdys[i]), .value(value[i]), .phase(0));
    end

    wire e_code, l_code;
    nco e_nco(.clk(clk), .rst(rst_in), .control_word(16'h0800), .i_code(e_code), .phase_control_word(16'h0000));

    reg [5:0] code;

    always @(posedge e_code)
        begin
            if (!code[0] && !code[1] && !code[2]) begin
                code <= 6'b110010;
            end else begin
                code <= {code[4:0], code[5]};
            end
        end

    reg [2:0] pipeline;
    always@(posedge clk)
    begin
        if (rdys[0]) begin
            pipeline[0] <= 1;
            i_value_reg <= value[0];
            q_value_reg <= value[1];
            rdy <= 1;
        end else if (pipeline[0]) begin
            pipeline[0] <= 0;
            pipeline[1] <= 1;
            i_value_reg <= value[2];
            q_value_reg <= value[3];
        end else if (pipeline[1]) begin
            pipeline[1] <= 0;
            pipeline[2] <= 1;
            i_value_reg <= value[4];
            q_value_reg <= value[5];
        end else if (pipeline[2]) begin
            pipeline[2] <= 0;
            rdy <= 0;
        end
    end

endmodule