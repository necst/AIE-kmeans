ECHO=@echo

.PHONY: help

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."
	$(ECHO) ""

.phony: clean

################## software build for XRT Native API code
CXXFLAGS := -std=c++17 -Wno-deprecated-declarations
CXXFLAGS += -I$(XILINX_XRT)/include -I$(XILINX_HLS)/include

LDFLAGS := -L$(XILINX_XRT)/lib
LDFLAGS += $(LDFLAGS) -lxrt_coreutil

# You can use them for including something. For example, opencv
#LIBS = `pkg-config --libs opencv`
#CFLAGS = `pkg-config --cflags opencv`

EXECUTABLE := host_overlay.exe

HOST_SRCS := ./host_code.cpp

all: build_sw
build_sw: $(EXECUTABLE)

run_sw:
	./$(EXECUTABLE)

#Eventually add LIBS and CFLAGS
$(EXECUTABLE): $(HOST_SRCS)
	$(CXX) -o $(EXECUTABLE) $^ $(CXXFLAGS) $(LDFLAGS) 
	@rm -f ./overlay_hw.xclbin
	@rm -f ./overlay_hw_emu.xclbin
	@ln -s ../hw/overlay_hw.xclbin
	@ln -s ../hw/overlay_hw_emu.xclbin

################## clean up
clean:
	$(RM) -r _x .Xil *.ltx *.log *.jou *.info host_overlay.exe *.xo *.xo.* *.str *.xclbin .run *.wdb *.json *.wcfg *.protoinst *.csv
	
