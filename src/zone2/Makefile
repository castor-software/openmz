#  
# Copyright 2020, Saab AB
#  
# This software may be distributed and modified according to
# the terms of the MIT License.
# Note that NO WARRANTY is provided.
# See "LICENSE.MIT" for details.
#  

INCLUDES = include $(BSP_DIR) $(HEXFIVE_DIR)
SOURCES = $(wildcard *.c *.S) $(HEXFIVE_DIR)/libhexfive.S ../zone1/counter.S
HEADERS = $(wildcard *.h $(addsuffix /*.h, $(INCLUDES)))

CFLAGS = -march=$(ARCH) -mabi=$(ABI) -mcmodel=$(CMODEL)
CFLAGS += $(addprefix -I, $(INCLUDES)) -nostdlib
CFLAGS += -T $(BSP_DIR)/memory.lds -T default.lds
CFLAGS += -g -O3

.PHONY: compile 
compile: $(PROGRAM_ELF)

$(PROGRAM_ELF): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) 
	$(SIZE) $@
