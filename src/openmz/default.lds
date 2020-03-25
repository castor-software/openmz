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
        flash (rxai!w) : ORIGIN = flash, LENGTH = 0x10000
        ram (wxa!ri) : ORIGIN = ram, LENGTH = 0x10000 
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
                KEEP (*(.text.init))
                *(.text)
                *(.text.*)
                . = ALIGN(8); 
        } >flash AT>flash :flash

        .data : ALIGN(8) {
                PROVIDE(__global_pointer$ = . + 0x800); 
                *(.data)
                *(.data.*)
        } >ram AT>flash :ram_init

        PROVIDE(_data_src_start = LOADADDR(.data)); 
        PROVIDE(_data_dst_start = ADDR(.data)); 
        PROVIDE(_data_dst_end = ADDR(.data) + SIZEOF(.data)); 

        .stack : ALIGN(8) {
            . += __stack_size;
            PROVIDE( _sp = .);
        } >ram AT>ram :ram
}
