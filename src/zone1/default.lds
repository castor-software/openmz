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
        rom (rxai!w) : ORIGIN = rom + 0x100000, LENGTH = 0x100000
        /* 6 KiB ram */
        ram (wxa!ri) : ORIGIN = ram + 0x1000, LENGTH = 0x1000 
}

SECTIONS
{
        /* 1 KiB stack */
        __stack_size = DEFINED(__stack_size) ? __stack_size : 0x400;
        PROVIDE(__stack_size = __stack_size);

        .text : {
                KEEP(*(.text.init))
                *(.text)
                *(.text.*)
                . = ALIGN(8); 
        } >rom AT>rom

        .rodata : ALIGN(8) {
                *(.rdata)
                *(.rodata)
                *(.rodata.*)
        } >rom AT>rom

        .data : ALIGN(8) {
                *(.data)
                *(.data.*)
                *(.sdata)
                *(.sdata.*)
                . = ALIGN(8); 
        } >ram AT>rom
        PROVIDE(_data_lma = LOADADDR(.data)); 
        PROVIDE(_data = ADDR(.data)); 
        PROVIDE(_edata = ADDR(.data) + SIZEOF(.data)); 
        PROVIDE(__global_pointer$ = ADDR(.data) + 0x800); 

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
