filename = top
pcf_file = io.pcf

build:
	/usr/bin/clash -i. -v --verilog Top.hs -outputdir build
	yosys -p "synth_ice40 -json $(filename).json" build/Top.topEntity/blinker.v
	nextpnr-ice40 --up5k --package sg48 --pcf $(pcf_file) --json $(filename).json --asc $(filename).asc
	icepack $(filename).asc $(filename).bin

prog:
	iceprog -S $(filename).bin

prog_flash:
	iceprog -d d:001/012 $(filename).bin

clean:
	rm -rf $(filename).blif $(filename).asc $(filename).bin
