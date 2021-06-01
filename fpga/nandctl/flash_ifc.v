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

    reg [25:0] counter;
    reg [4:0] waveform;
    wire [4:0] state;
    assign {ce, we, re, cle, ale} = waveform;
    assign state = counter[25:21];

    reg [7:0] dtr;
    assign din = dtr;
    reg [1:0] en;
    assign tx_start = (en > 0) && (en < 3);

    always @(posedge clk)
        begin
            counter <= counter + 1;

            if (state == 0) begin
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
                en <= 0;
            end else if (state == 1) begin
                waveform <= {1, 0, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end else if (state == 2) begin
                waveform <= {0, 0, 1, 1, 0};
                nand_oe <= 1;
                nand_dout <= 8'hff;
            end else if (state == 3) begin
                waveform <= {0, 1, 1, 1, 0};
                nand_oe <= 1;
                nand_dout <= 8'hff;
            end else if (state == 4) begin
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end else if (state == 5) begin
                waveform <= {1, 0, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end else if (state == 6) begin
                waveform <= {0, 0, 1, 1, 0};
                nand_oe <= 1;
                nand_dout <= 8'h70;
            end else if (state == 7) begin
                waveform <= {0, 1, 1, 1, 0};
                nand_oe <= 1;
                nand_dout <= 8'h70;
            end else if (state == 8) begin
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end else if (state == 9) begin
                waveform <= {0, 1, 0, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end else if (state == 10) begin
                waveform <= {0, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'h70;
                dtr <= 8'hff - nand_din;
                if (en < 3) begin
                    en <= en + 1;
                end
            end else if (state == 11) begin
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'h70;
            end else if (state == 12) begin
                waveform <= {1, 0, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end else if (state == 13) begin
                waveform <= {0, 0, 1, 1, 0};
                nand_oe <= 1;
                nand_dout <= 8'h90;
            end else if (state == 14) begin
                waveform <= {0, 1, 1, 1, 0};
                nand_oe <= 1;
                nand_dout <= 8'h90;
            end else if (state == 15) begin
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end else if (state == 16) begin
                waveform <= {1, 0, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end else if (state == 17) begin
                waveform <= {0, 0, 1, 0, 1};
                nand_oe <= 1;
                nand_dout <= 8'h20;
            end else if (state == 18) begin
                waveform <= {0, 1, 1, 0, 1};
                nand_oe <= 1;
                nand_dout <= 8'h20;
            end else if (state == 19) begin
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end else if (state == 20) begin
                waveform <= {0, 1, 0, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
                en <= 0;
            end else if (state == 21) begin
                waveform <= {0, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'h70;
                dtr <= nand_din;
                if (en < 3) begin
                    en <= en + 1;
                end
            end else if (state == 22) begin
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'h70;
            end else if (state == 23) begin
                waveform <= {0, 1, 0, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
                en <= 0;
            end else if (state == 24) begin
                waveform <= {0, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'h70;
                dtr <= nand_din;
                if (en < 3) begin
                    en <= en + 1;
                end
            end else if (state == 25) begin
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'h70;
            end else if (state == 26) begin
                waveform <= {0, 1, 0, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
                en <= 0;
            end else if (state == 27) begin
                waveform <= {0, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'h70;
                dtr <= nand_din;
                if (en < 3) begin
                    en <= en + 1;
                end
            end else if (state == 28) begin
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'h70;
            end else begin
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end
        end

endmodule
