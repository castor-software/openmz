# Copyright 2020, Saab AB
#
# This software may be distributed and modified according to 
# the terms of the GNU General Public License version 2. 
# Note that NO WARRANTY is provided.
# See "LICENSE.GPLv2" for details.

COMPILER ?= gcc
TARGET ?= fe310

SOURCES = traps.S event_handler.c kernel.c crt0.S

BSP_DIR := ../bsp/$(TARGET)
SIZE = riscv64-unknown-elf-size

# include compiler specific flags
include $(COMPILER).mk
CFLAGS += -T $(BSP_DIR)/memory.lds -T default.lds -nostdlib -static

.PHONY: openmz.elf
openmz.elf: $(wildcard *.S *.h *.c)
	$(CC) $(CFLAGS) -v $(SOURCES) -o openmz.elf
	$(SIZE) openmz.elf

.PHONY: clean
clean:
	rm openmz.elf
