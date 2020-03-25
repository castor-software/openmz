# Source folder structure

 - `openmz`, the OpenMZ kernel's source, header and linker scripts. 
 - `zoneX`, the example zone's source, header and linker scripts. 
 - `libhexfive`, MultiZone API.
 - `bsp`, board support package.

# Compiling the kernel and the example zones

To compile the kernel and the example zones, use the following terminal command:
```bash
make CC=[riscv64-unknown-elf-gcc] TARGET=[fe310]
# [X] ==> X is default value
# CC is the compiler
# TARGET is the name of the board support package. Sets the bsp directory to bsp/$(TARGET)
```
The makefile is set to use the same compiler for the kernel and the zones. If you want to use different compilers and compiler flags, alter the makefiles.
