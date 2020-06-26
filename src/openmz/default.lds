/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to 
 * the terms of the GNU General Public License version 2. 
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
OUTPUT_ARCH("riscv")

ENTRY(_start)

MEMORY
{
        /* 64 KiB rom */
        rom (rxai!w) : ORIGIN = rom + 0x10000, LENGTH = 0x2000
        itim (wx!rai) : ORIGIN = itim, LENGTH = 0x2000
        /* 4 KiB ram */
        ram (wxa!ri) : ORIGIN = ram, LENGTH = 0x1000 
}

SECTIONS
{
        /* 1 KiB stack */
        __stack_size = DEFINED(__stack_size) ? __stack_size : 0x200;
        PROVIDE(__stack_size = __stack_size);

        .text : {
                KEEP(*(.text.init))
                . = ALIGN(8); 
        } >rom AT>rom

        .data : ALIGN(8) {
                *(.data)
                *(.data.*)
                *(.sdata)
                *(.sdata.*)
                *(.rodata .rodata.*)
                *(.rdata .rdata.*)
                . = ALIGN(8); 
        } >ram AT>rom

        PROVIDE(__global_pointer$ = ADDR(.data)); 
        PROVIDE(_data_lma = LOADADDR(.data)); 
        PROVIDE(_data = ADDR(.data)); 
        PROVIDE(_edata = ADDR(.data) + SIZEOF(.data)); 
        
        .itim : ALIGN(8) {
                *(.text)
                *(.text.*)
        		*(.itim .itim.*)
                . = ALIGN(8); 
        } >ram AT>rom
        
        PROVIDE(_itim_lma = LOADADDR(.itim));
        PROVIDE(_itim = ADDR(.itim));
        PROVIDE(_eitim = ADDR(.itim) + SIZEOF(.itim));

        .bss : ALIGN(8) {
                *(.bss)
                *(.bss.*)
                *(.sbss*)
                *(COMMON)
                . = ALIGN(8); 
        } >ram AT>ram
        PROVIDE(_bss = ADDR(.bss)); 
        PROVIDE(_ebss = ADDR(.bss) + SIZEOF(.bss)); 
        PROVIDE(_heap = _ebss); 

        .stack ORIGIN(ram) + LENGTH(ram) - __stack_size : {
            . += __stack_size;
        } >ram AT>ram
        PROVIDE(_eheap = ADDR(.stack));
        PROVIDE(_sp = ADDR(.stack) + SIZEOF(.stack));
}
