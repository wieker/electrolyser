filename = top
pcf_file = io.pcf

build:
	yosys -p "synth_ice40 -blif $(filename).blif" top.v acia_tx.v correlator.v sig_source.v osc.v \
	    digitizer.v hex_dump.v input_stab.v dispatcher.v acia_rx.v flag_holder.v mass_or.v dispatcher_ctl.v fifo.v
	arachne-pnr -d 5k -P sg48 -p $(pcf_file) $(filename).blif -o $(filename).asc
	icepack $(filename).asc $(filename).bin

prog:
	iceprog -S $(filename).bin

prog_flash:
	iceprog -d d:001/012 $(filename).bin

clean:
	rm -rf $(filename).blif $(filename).asc $(filename).bin
