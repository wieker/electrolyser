filename = top
pcf_file = io.pcf

build:
	yosys -p "synth_ice40 -top top -json $(filename).json" top.v acia_tx.v correlator.v sig_source.v osc.v \
	    digitizer.v hex_dump.v input_stab.v dispatcher.v acia_rx.v flag_holder.v mass_or.v dispatcher_ctl.v fifo.v \
	    nco.v iabs_demod.v psk_demod.v fm_demod.v iq_demod.v ipll.v adjust.v tx.v adjust1.v spi_slave.v
	nextpnr-ice40 --u4k --package sg48 --pcf $(pcf_file) --json $(filename).json --asc $(filename).asc
	nextpnr-ice40 --up5k --package sg48 --pcf $(pcf_file) --json $(filename).json --asc $(filename)-up5k.asc
	icepack $(filename).asc $(filename).bin
	icepack $(filename)-up5k.asc $(filename)-up5k.bin

prog:
	iceprog -S $(filename).bin

prog_flash:
	iceprog -d d:001/012 $(filename).bin

clean:
	rm -rf $(filename).blif $(filename).asc $(filename).bin
