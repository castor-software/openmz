#  
# Copyright 2020, Saab AB
#  
# This software may be distributed and modified according to
# the terms of the MIT License.
# Note that NO WARRANTY is provided.
# See "LICENSE.MIT" for details.
#  

INCLUDES = include $(BSP_DIR)

SOURCES = $(wildcard *.c *.S)
HEADERS = $(wildcard *.h $(addsuffix /*.h, $(INCLUDES)))

#CC = ccomp
#CFLAGS = -finline-asm -fno-fpu -g

CFLAGS = -march=$(ARCH) -mabi=$(ABI) -mcmodel=$(CMODEL)
CFLAGS += -g -O2

CFLAGS += $(addprefix -I, $(INCLUDES)) -nostdlib -static
CFLAGS += -T $(BSP_DIR)/memory.lds -T default.lds

.PHONY: compile 
compile: $(PROGRAM_ELF)

$(PROGRAM_ELF): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) 
	$(SIZE) $@
