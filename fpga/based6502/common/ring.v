`default_nettype none

// Ring oscillator.
//
// Avoid using zero delay LUTs. Zero delay LUTs may be unstable
// and also results in extremely high frequencies at very low amplitudes.
// E.g. on the ice40hx1k, this results in a ~650MHz signal,
// but so weak that other logic will not properly pick it up.
// When connecting it to an output pin, the signal has -25dBm.
module ringoscillator(output wire chain_out);
	parameter DELAY_LUTS = 1;

	wire chain_wire[DELAY_LUTS+1:0];
	assign chain_wire[0] = chain_wire[DELAY_LUTS+1];
	assign chain_out = chain_wire[1];
	// inverter is at [0], so [1] comes freshly from the inverter.
	// if that matters.

	generate
		genvar i;
		for(i=0; i<=DELAY_LUTS; i=i+1) begin: delayline
			(* keep *) (* noglobal *)
			SB_LUT4 #(.LUT_INIT((i==0)?16'd1:16'd2))
				chain_lut(.O(chain_wire[i+1]), .I0(chain_wire[i]), .I1(1'b0), .I2(1'b0), .I3(1'b0));
		end
	endgenerate
endmodule