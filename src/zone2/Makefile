# Copyright 2020, Saab AB
#
# This software may be distributed and modified according to 
# the terms of the GNU General Public License version 2. 
# Note that NO WARRANTY is provided.
# See "LICENSE.GPLv2" for details.

PROGRAM = zone2
ELF = $(PROGRAM).elf
HEX = $(PROGRAM).hex
SOURCES = main.c head.S

BSP_DIR := ../bsp/$(TARGET)
SIZE = riscv64-unknown-elf-size
OBJCOPY = riscv64-unknown-elf-objcopy

# add flags
ifeq ($(CC), riscv64-unknown-elf-gcc)
    CFLAGS += -std=c99 -O2 -mabi=ilp32 -march=rv32imac
else ifeq ($(CC), ccomp)
    CFLAGS += -O2 -Wall -finline-asm -fno-fpu
endif


CFLAGS += -T $(BSP_DIR)/memory.lds -T default.lds -nostdlib -static

all: $(ELF) $(HEX)

$(ELF): $(wildcard *.S *.h *.c)
	$(CC) $(CFLAGS) $(SOURCES) -o $(ELF)
	$(SIZE) $(ELF)

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $(ELF) $(HEX)

.PHONY: clean
clean:
	rm -f $(ELF) $(HEX)
