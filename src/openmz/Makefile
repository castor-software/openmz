# Copyright 2020, Saab AB
#
# This software may be distributed and modified according to 
# the terms of the GNU General Public License version 2. 
# Note that NO WARRANTY is provided.
# See "LICENSE.GPLv2" for details.

PROGRAM = openmz
ELF = $(PROGRAM).elf
HEX = $(PROGRAM).hex
SOURCES = head.S traps.S ecall.c illegal_instruction.c kernel.c scheduler.c traps.c user_timer.c user_trap.c 


SIZE = riscv64-unknown-elf-size
OBJCOPY = riscv64-unknown-elf-objcopy
BSP_DIR = ../bsp/$(TARGET)

# include compiler specific flags
ifeq ($(CC), riscv64-unknown-elf-gcc) # GCC
  CFLAGS += -std=c99 -Og -g -Wall -mabi=ilp32 -march=rv32imac
else ifeq ($(CC), riscv32-ccomp) # CompCert
  CFLAGS += -O -g -Wall -finline-asm -fno-fpu
endif



# load the memory layout from the BSP (memory.lds), and the linker script (default.lds).
CFLAGS += -T $(BSP_DIR)/memory.lds -T default.lds -nostdlib

all: $(ELF) $(HEX)

$(ELF): $(wildcard *.S *.h *.c)
	$(CC) $(CFLAGS) $(SOURCES) -o $(ELF)
	$(SIZE) $(ELF)

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $(ELF) $(HEX)

.PHONY: clean
clean:
	rm -f $(ELF) $(HEX)
