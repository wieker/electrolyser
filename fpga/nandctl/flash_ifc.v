// tst_6502.v - test 6502 core
// 02-11-19 E. Brombaugh

module flash_ifc(
    input clk,              // 4..0MHz CPU clock

    output reg nand_oe,
    input [7:0] nand_din,
    output reg [7:0] nand_dout,
    output ce, cle, we, re, ale,

	output [7:0] din,
	output tx_start,
);

    reg [24:0] counter;
    reg [4:0] waveform;
    wire [3:0] state;
    assign {ce, we, re, cle, ale} = waveform;
    assign state = counter[24:21];

    reg [7:0] dtr;
    assign din = dtr;
    reg [1:0] en;
    assign tx_start = (en > 0) && (en < 3);

    always @(posedge clk)
        begin
            counter <= counter + 1;

            if (state == 0) begin
                // ce#, we#, re#, cle, ale
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
                en <= 0;
            end else if (state == 1) begin
                // Prepare all states in one moment for CMD
                // ce#, we#, re#, cle, ale
                waveform <= {0, 0, 1, 1, 0};
                nand_oe <= 1;
                nand_dout <= 8'hff;
            end else if (state == 2) begin
                // WE# CMD edge
                // ce#, we#, re#, cle, ale
                waveform <= {0, 1, 1, 1, 0};
                nand_oe <= 1;
                nand_dout <= 8'hff;
            end else if (state == 3) begin
                // RELAX all after CMD
                // ce#, we#, re#, cle, ale
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end else if (state == 4) begin
                // RELAX all after CMD
                // ce#, we#, re#, cle, ale
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end else if (state == 5) begin
                // Prepare all states in one moment for CMD
                // ce#, we#, re#, cle, ale
                waveform <= {0, 0, 1, 1, 0};
                nand_oe <= 1;
                nand_dout <= 8'h70;
            end else if (state == 6) begin
                // WE# CMD edge
                // ce#, we#, re#, cle, ale
                waveform <= {0, 1, 1, 1, 0};
                nand_oe <= 1;
                nand_dout <= 8'h70;
            end else if (state == 7) begin
                // RELAX all after CMD
                // ce#, we#, re#, cle, ale
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end else if (state == 8) begin
                // Prepare CE# for DATA read
                // ce#, we#, re#, cle, ale
                waveform <= {0, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'h70;
            end else if (state == 9) begin
                // RE# EDGE for DATA read
                // ce#, we#, re#, cle, ale
                waveform <= {0, 1, 0, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'h70;
                en <= 0;
            end else if (state == 10) begin
                // Read data from DATA READ and relax ALL
                // ce#, we#, re#, cle, ale
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'h70;
                if (en == 0) begin
                    dtr <= nand_din;
                end
                if (en < 3) begin
                    en <= en + 1;
                end
            end else if (state == 11) begin
                // RELAX all after DATA read
                // ce#, we#, re#, cle, ale
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
                en <= 0;
            end
        end

endmodule
