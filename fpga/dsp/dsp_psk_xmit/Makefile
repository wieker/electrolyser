filename = top
pcf_file = io.pcf

build:
	yosys -p "synth_ice40 -blif $(filename).blif" $(filename).v acia_tx.v fifo.v hex_dump.v dispatcher.v dispatcher_ctl.v correlator.v
	arachne-pnr -d 1k -P vq100 -p $(pcf_file) $(filename).blif -o $(filename).asc
	icepack $(filename).asc $(filename).bin

prog:
	iceprog -S $(filename).bin

prog_flash:
	iceprog -d d:001/012 $(filename).bin

clean:
	rm -rf $(filename).blif $(filename).asc $(filename).bin
