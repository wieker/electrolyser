// tst_6502.v - test 6502 core
// 02-11-19 E. Brombaugh

module flash_ifc(
    input clk,              // 4..0MHz CPU clock

    output reg nand_oe,
    input [7:0] nand_din,
    output reg [7:0] nand_dout,

    output ce,
    output we,
    output re,
    output cle,
    output ale,

	output reg [7:0] uart_out,
	output reg tx_start,
);

    reg [24:0] counter;
    reg [4:0] waveform;
    wire [3:0] state;
    wire [20:0] mnt;
    assign {ce, we, re, cle, ale} = waveform;
    assign state = counter[24:21];
    assign mnt = counter[20:0];


    always @(posedge clk)
        begin
            if ((state == 10) && (mnt == 0)) begin
                uart_out <= nand_din;
            end else if ((state == 10) && (mnt == 1)) begin
                tx_start <= 1;
            end else if ((state == 10) && (mnt == 2)) begin
                tx_start <= 0;
            end
        end

    always @(posedge clk)
        begin
            counter <= counter + 1;

            if ((state == 0) && (mnt == 0)) begin
                // ce#, we#, re#, cle, ale
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end else if ((state == 1) && (mnt == 0)) begin
                // Prepare all states in one moment for CMD
                // ce#, we#, re#, cle, ale
                waveform <= {0, 0, 1, 1, 0};
                nand_oe <= 1;
                nand_dout <= 8'hff;
            end else if ((state == 2) && (mnt == 0)) begin
                // WE# CMD edge
                // ce#, we#, re#, cle, ale
                waveform <= {0, 1, 1, 1, 0};
                nand_oe <= 1;
                nand_dout <= 8'hff;
            end else if ((state == 3) && (mnt == 0)) begin
                // RELAX all after CMD
                // ce#, we#, re#, cle, ale
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end else if ((state == 4) && (mnt == 0)) begin
                // RELAX all after CMD
                // ce#, we#, re#, cle, ale
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end else if ((state == 5) && (mnt == 0)) begin
                // Prepare all states in one moment for CMD
                // ce#, we#, re#, cle, ale
                waveform <= {0, 0, 1, 1, 0};
                nand_oe <= 1;
                nand_dout <= 8'h70;
            end else if ((state == 6) && (mnt == 0)) begin
                // WE# CMD edge
                // ce#, we#, re#, cle, ale
                waveform <= {0, 1, 1, 1, 0};
                nand_oe <= 1;
                nand_dout <= 8'h70;
            end else if ((state == 7) && (mnt == 0)) begin
                // RELAX all after CMD
                // ce#, we#, re#, cle, ale
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end else if ((state == 8) && (mnt == 0)) begin
                // Prepare CE# for DATA read
                // ce#, we#, re#, cle, ale
                waveform <= {0, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'h70;
            end else if ((state == 9) && (mnt == 0)) begin
                // RE# EDGE for DATA read
                // ce#, we#, re#, cle, ale
                waveform <= {0, 1, 0, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'h70;
            end else if ((state == 10) && (mnt == 0)) begin
                // Read data from DATA READ and relax ALL
                // ce#, we#, re#, cle, ale
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'h70;
            end else if ((state == 11) && (mnt == 0)) begin
                // RELAX all after DATA read
                // ce#, we#, re#, cle, ale
                waveform <= {1, 1, 1, 0, 0};
                nand_oe <= 0;
                nand_dout <= 8'hff;
            end
        end

endmodule
