# MacQueen's Implementation - integers

This implementation performs MacQueen's version of K-means clustering by randomly generating integer input coordinates for points and clusters. The number of generated data attains to the constraint of being a multiple of the "POINTS" constant defined in ```common/constants.h``` and the maximum number of cluster is 32.

## Main Structure

The archive is structured as follows:
```
├─ aie/                # AIE kernel for transfomation
|  └─ src/             # Contains the code
├─ data_movers/        # PL kernels for setting up the AIE and for fetching
├─ mutual_info/        # PL Kernel for computing mutual information
├─ common/             # Common codes and variables
├─ hw/                 # System integration
├─ sw/                 # Host application and dataset
└─ Makefile/           # Top-level Makefile to build and run  
```

## Main Commands

### aie

_make aie_compile_x86_ : compile your code for x86 architecture.  
_make aie_simulate_x86_ : simulate your x86 architecture.  
_make aie_compile_ : compile your code for VLIW architecture, as your final hardware for HW ad HW_EMU.  
_make aie_simulate_ : simulate your code for VLIW architecture, as your final hardware.  
_make clean_ : removes all the output file created by the commands listed above.  

### data_movers

_make compile TARGET=HW/HW_EMU_: compiles all your kernel, skipping the ones already compiled.  
_make run_testbench_setup_aie_ : compiles and execute the testbench for the kernel setup_aie.  
_make run_testbench_sink_from_aie_ : compiles and execute the testbench for the kernel setup_aie.  

### HW

_make all TARGET=HW_ : builds the hardware linking your components
_make all TARGET=HW_EMU_ : builds the hardware emu linking your components
_make clean_: removes all files.

### SW

_make build_sw_ : compiles the sw
_./setup_emu.sh -s on_ : enables the hardware emulation
_./host_overlay.exe_ : runs the emulation

## General useful commands:

If you need to move your bitstream and executable on the target machine, you may want it prepared in a single folder that contains all the required stuff to be moved. In this case, you can use the

_make build_and_pack TARGET=hw/hw_emu_ :  it allows you to pack our build in a single folder. Notice that the hw_emu does not have to be moved on the device, it must be executed on the development machine.
