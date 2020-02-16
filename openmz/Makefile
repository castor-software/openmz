# Copyright 2020, Saab AB
#
# This software may be distributed and modified according to 
# the terms of the GNU General Public License version 2. 
# Note that NO WARRANTY is provided.
# See "LICENSE.GPLv2" for details.

TARGET ?= fe310

BSP_DIR := bsp/$(TARGET)

CC := ccomp
OBJCOPY := riscv64-unknown-elf-objcopy


#include $(BSP_DIR)/settings.mk
CFLAGS += -O2 -g -Wall -finline-asm -fno-fpu
CFLAGS += -T $(BSP_DIR)/default.lds -nostdlib -static
CFLAGS += -Wl,-Map,openmz.map 

SOURCES = head.S traps.S event_handler.c kernel.c $(BSP_DIR)/entry.S

.PHONY: all
all: openmz.elf openmz.hex

openmz.elf: $(wildcard *.S *.h *.c) $(BSP_DIR)/entry.S
	$(CC) $(CFLAGS) -v $(SOURCES) -o $@

openmz.hex: openmz.elf
	$(OBJCOPY) -O ihex openmz.elf openmz.hex

.PHONY: clean
clean:
	rm -f openmz.elf openmz.hex openmz.hex
