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
        flash (rxai!w) : ORIGIN = flash + 0x10000, LENGTH = 0x10000
        ram (wxa!ri) : ORIGIN = ram + 0x1000, LENGTH = 0x1000 
}

PHDRS
{
        flash PT_LOAD; 
        ram PT_LOAD; 
        ram_init PT_LOAD; 
}

SECTIONS
{
        __stack_size = DEFINED(__stack_size) ? __stack_size : 0x400;
        PROVIDE(__stack_size = __stack_size);
        .text : {
                KEEP(*(.text.init))
                KEEP(*(.text.trap))
                *(.text)
                *(.text.*)
                . = ALIGN(4); 
        } >flash AT>flash :flash

        PROVIDE(_text_start = ADDR(.text)); 
        PROVIDE(_text_end = ADDR(.text) + SIZEOF(.text)); 
  
        .rodata : ALIGN(8) {
                *(.rdata)
                *(.rodata)
                *(.rodata.*)
        } >flash AT>flash :flash

        .data : ALIGN(8) {
                *(.data)
                *(.data.*)
                . = ALIGN(4); 
                PROVIDE(__global_pointer$ = . + 0x800); 
                *(.sdata)
                *(.sdata.*)
                . = ALIGN(4); 
        } >ram AT>flash :ram_init

        PROVIDE(_data_src_start = LOADADDR(.data)); 
        PROVIDE(_data_dst_start = ADDR(.data)); 
        PROVIDE(_data_dst_end = ADDR(.data) + SIZEOF(.data)); 

        .bss : ALIGN(8) {
                *(.bss)
                *(.bss.*)
                *(.sbss*)
                *(COMMON)
                . = ALIGN(8); 
        } >ram AT>ram :ram

        PROVIDE(_bss_start = ADDR(.bss)); 
        PROVIDE(_bss_end = ADDR(.bss) + SIZEOF(.bss)); 

        .stack : {
            PROVIDE(stack_start = .);
            . += __stack_size;
            PROVIDE(_sp = .);
            PROVIDE(stack_end = .);
        } >ram AT>ram :ram
}
