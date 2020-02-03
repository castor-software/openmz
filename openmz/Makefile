# Copyright 2020, Saab AB
#
# This software may be distributed and modified according to 
# the terms of the GNU General Public License version 2. 
# Note that NO WARRANTY is provided.
# See "LICENSE.GPLv2" for details.

TARGET ?= fe310

BSP_DIR := bsp/$(TARGET)

CC := riscv64-unknown-elf-gcc
OBJCOPY := riscv64-unknown-elf-objcopy


include $(BSP_DIR)/settings.mk
CFLAGS += -std=c99 -Og -Wall -mpreferred-stack-boundary=4 -I $(BSP_DIR)
CFLAGS += -T $(BSP_DIR)/default.lds -nostartfiles -static
CFLAGS += -Wl,-Map,openmz.map

SOURCES = head.S traps.S event_handler.c kernel.c $(BSP_DIR)/entry.S

.PHONY: all
all: openmz.elf openmz.hex

openmz.elf: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

openmz.hex: openmz.elf
	$(OBJCOPY) -O ihex openmz.elf openmz.hex

.PHONY: clean
clean:
	rm -f openmz.elf openmz.hex openmz.hex
