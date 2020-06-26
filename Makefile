#  
# Copyright 2020, Saab AB
#  
# This software may be distributed and modified according to
# the terms of the MIT License.
# Note that NO WARRANTY is provided.
# See "LICENSE.MIT" for details.
#  

PROGRAMS = openmz zone1 zone2

BOARD ?= fe310
BSP_DIR = $(abspath bsp/$(BOARD))
include $(BSP_DIR)/settings.mk

HEXFIVE_DIR = $(abspath libhexfive)

ELF_DIR = $(abspath elf)
HEX_DIR = $(abspath hex)

export CC=riscv64-unknown-elf-gcc
export SIZE=riscv64-unknown-elf-size
export OBJCOPY=riscv64-unknown-elf-objcopy
export OBJDUMP=riscv64-unknown-elf-objdump

.PHONY: all
all: compile 

.PHONY: compile
.PHONY: clean
.PHONY: upload
ifeq ($(PROGRAM),)
compile:
	for file in $(PROGRAMS) ; do \
	    $(MAKE) -C . compile PROGRAM=$$file ; \
	done
clean:
	for file in $(PROGRAMS) ; do \
	    $(MAKE) -C . clean PROGRAM=$$file ; \
	done
upload:
	for file in $(PROGRAMS) ; do \
	    $(MAKE) -C . upload PROGRAM=$$file ; \
	done
else

PROGRAM_ELF = $(ELF_DIR)/$(PROGRAM).elf
PROGRAM_HEX = $(HEX_DIR)/$(PROGRAM).hex

compile: $(PROGRAM_ELF) $(PROGRAM_HEX)

.PHONY: $(PROGRAM_ELF)
$(PROGRAM_ELF):
	mkdir -p $(ELF_DIR)
	$(MAKE) -C src/$(PROGRAM) compile \
	    PROGRAM_ELF=$(PROGRAM_ELF) \
	    BSP_DIR=$(BSP_DIR) \
	    HEXFIVE_DIR=$(HEXFIVE_DIR)

$(PROGRAM_HEX): $(PROGRAM_ELF)
	mkdir -p $(HEX_DIR)
	$(OBJCOPY) -O ihex $(PROGRAM_ELF) $(PROGRAM_HEX)

upload: $(PROGRAM_HEX)
	echo "loadfile $(PROGRAM_HEX)\nexit" \
	    | JLinkExe -device fe310 -if JTAG -speed 4000 -jtagconf -1,-1 -autoconnect 1

clean:
	rm -f $(PROGRAM_ELF)
endif


