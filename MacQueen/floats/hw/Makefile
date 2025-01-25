ECHO=@echo

.PHONY: help

help::
	$(ECHO) ""
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all TARGET=<hw/hw_emu>"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."
	$(ECHO) ""

PLATFORM := xilinx_vck5000_gen4x8_qdma_2_202220_1
#PLATFORM := xilinx_vck5000_gen4x8_xdma_2_202210_1
TARGET := hw

XOCCLFLAGS := --kernel_frequency 400 --platform $(PLATFORM) -t $(TARGET)  -s -g

AIE_OBJ := ../aie/libadf.a
XOS     := ../data_movers/setup_aie_$(TARGET).xo 
XOS     += ../data_movers/sink_from_aie_$(TARGET).xo 
XSA_OBJ := overlay_$(TARGET).xsa
XCLBIN  := overlay_$(TARGET).xclbin

.phony: clean

all: $(XCLBIN)

$(XCLBIN): $(XSA_OBJ) $(AIE_OBJ)
	v++ -p -t $(TARGET) -f $(PLATFORM) $^ -o $@ --package.boot_mode=ospi

$(XSA_OBJ): $(XOS) $(AIE_OBJ)
	v++ -l $(XOCCFLAGS) $(XOCCLFLAGS) --config xclbin_overlay.cfg -o $@ $^

clean:
	$(RM) -r _x .Xil .ipcache *.ltx *.log *.sh *.jou *.info *.xclbin *.xo.* *.str *.xsa *.cdo.bin *bif *BIN *.package_summary *.link_summary *.txt *.bin && rm -rf cfg emulation_data sim
	
