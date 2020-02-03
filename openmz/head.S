/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to 
 * the terms of the GNU General Public License version 2. 
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */

.globl _start 
.extern __global_pointer$
.extern kernel_stack
.extern KernelInit

.macro reset
        mv x1, x0
        mv x2, x0
        mv x3, x0
        mv x4, x0
        mv x5, x0
        mv x6, x0
        mv x7, x0
        mv x8, x0
        mv x9, x0
        mv x10, x0
        mv x11, x0
        mv x12, x0
        mv x13, x0
        mv x14, x0
        mv x15, x0
        mv x16, x0
        mv x17, x0
        mv x18, x0
        mv x19, x0
        mv x20, x0
        mv x21, x0
        mv x22, x0
        mv x23, x0
        mv x24, x0
        mv x25, x0
        mv x26, x0
        mv x27, x0
        mv x28, x0
        mv x29, x0
        mv x30, x0
        mv x31, x0
        csrw mtvec, x0
        csrw mstatus, x0
        csrw mie, x0
        csrw mip, x0
        csrw mcause, x0
        csrw mepc, x0
        csrw mtval, x0
.endm

_start:
        reset
        fence
.option push
.option norelax
        la      gp, __global_pointer$
.option pop
        la      sp, kernel_stack 
        csrw    mscratch, x0
        la      t0, __hang
        csrw    mtvec, t0
        call    KernelInit    # call the kernel entry function
__hang:
        ebreak
        j       __hang

