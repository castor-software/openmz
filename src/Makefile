.PHONY: all

PROGRAM_FOLDERS = openmz zone1 zone2
MERGED = merged.hex

CC = riscv64-unknown-elf-gcc 
OBJCOPY = riscv64-unknown-elf-objcopy

all: $(PROGRAM_FOLDERS) $(MERGED)

.PHONY: $(PROGRAM_FOLDERS)
$(PROGRAM_FOLDERS):
	make -C $@ CC=$(CC) TARGET=fe310

$(MERGED): $(PROGRAM_FOLDERS)
	cat $$(ls **/*.hex) > $(MERGED)
	sed '/^:00000001FF$$/d' -i $(MERGED)
	echo ':00000001FF' >> $(MERGED)

clean:
	for folder in $(PROGRAM_FOLDERS); do \
	    make -C $$folder clean; \
	done	
	rm -f $(MERGED)
