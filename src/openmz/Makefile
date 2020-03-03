# Copyright 2020, Saab AB
#
# This software may be distributed and modified according to 
# the terms of the GNU General Public License version 2. 
# Note that NO WARRANTY is provided.
# See "LICENSE.GPLv2" for details.

PROGRAM = openmz.elf
TARGET ?= fe310

SOURCES = traps.S event_handler.c kernel.c head.S

BSP_DIR := ../bsp/$(TARGET)
SIZE = riscv64-unknown-elf-size

# include compiler specific flags
ifeq ($(CC), riscv64-unknown-elf-gcc)
    CFLAGS += -std=c99 -O2 -g -Wall -mabi=ilp32 -march=rv32imac
else ifeq ($(CC), ccomp)
    CFLAGS += -O2 -g -Wall -finline-asm -fno-fpu
endif


CFLAGS += -T $(BSP_DIR)/memory.lds -T default.lds -nostdlib

all: $(PROGRAM)
.PHONY: $(PROGRAM)
$(PROGRAM): $(wildcard *.S *.h *.c)
	$(CC) $(CFLAGS) $(SOURCES) -o $(PROGRAM)
	$(SIZE) $(PROGRAM)

.PHONY: clean
clean:
	rm -f $(PROGRAM)
