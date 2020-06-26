/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to 
 * the terms of the GNU General Public License version 2. 
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */

.section .text.init
.globl _start
_start:
/* initialize global pointer */
.option push
.option norelax
    la      gp, __global_pointer$
.option pop
    la      sp, _sp

    la		t0, hang
    csrw	mtvec, t0

// copy the ITIM section
    la      t0, _itim_lma
    la      t1, _itim
    la      t2, _eitim

    beq     t0, t1, 2f
1:  lw      a0, 0(t0)
    sw      a0, 0(t1)
    addi    t0, t0, 4
    addi    t1, t1, 4
    bltu    t1, t2, 1b
2:
	fence.i

// copy the DATA section
    la      t0, _data_lma
    la      t1, _data
    la      t2, _edata

    beq     t0, t1, 2f
1:  lw      a0, 0(t0)
    sw      a0, 0(t1)
    addi    t0, t0, 4
    addi    t1, t1, 4
    bltu    t1, t2, 1b
2:

// zero bss segment (i.e. uninitiated data)
    la      t1, _bss
    la      t2, _ebss
    beq     t1, t2, 2f
1:  sw      x0, 0(t1)
    addi    t1, t1, 4
    bltu    t1, t2, 1b
2:

/* start the kernel */
    call    kmain      
 hang:
 	ebreak
    j       hang
