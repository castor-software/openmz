/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to 
 * the terms of the GNU General Public License version 2. 
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
OUTPUT_ARCH("riscv")

ENTRY(_entry)

MEMORY
{
        flash (rxai!w) : ORIGIN = 0x20010000, LENGTH = 0x10000
        ram (wxa!ri) : ORIGIN = 0x80000000 + 4K, LENGTH = 4K
}

PHDRS
{
        flash PT_LOAD; 
        ram PT_LOAD; 
        ram_init PT_LOAD; 
}

SECTIONS
{
        .text : {
                KEEP (*(.text.init))
                KEEP (*(.text.init.*))
                *(.text)
                *(.text.*)
                . = ALIGN(4); 
        } >flash AT>flash :flash

        PROVIDE(__text = ADDR(.text)); 
        PROVIDE (__etext = ADDR(.text) + SIZEOF(.text)); 
  
        .rodata : ALIGN(4) {
                *(.rdata)
                *(.rodata)
                *(.rodata.*)
        } >flash AT>flash :flash

        .data : ALIGN(4) {
                *(.data)
                *(.data.*)
                . = ALIGN(4); 
                PROVIDE(__global_pointer$ = . + 0x800); 
                *(.sdata)
                *(.sdata.*)
                . = ALIGN(4); 
        } >ram AT>flash :ram_init

        PROVIDE(_data_lma = LOADADDR(.data)); 
        PROVIDE(_data = ADDR(.data)); 
        PROVIDE(_edata = ADDR(.data) + SIZEOF(.data)); 

        .bss : ALIGN(4) {
                *(.bss)
                *(.bss.*)
                *(.sbss*)
                *(COMMON)
                . = ALIGN(8); 
        } >ram AT>ram :ram

        PROVIDE(_bss = ADDR(.bss)); 
        PROVIDE(_ebss = ADDR(.bss) + SIZEOF(.bss)); 
}
