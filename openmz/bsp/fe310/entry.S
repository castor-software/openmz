/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to 
 * the terms of the GNU General Public License version 2. 
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
.section .text.init
.globl _entry
_entry:

.option push
.option norelax
    la      gp, __global_pointer$
.option pop

    la      t0, _data
    la      t1, _edata
    la      t2, _data_lma
1:  beq     t0, t1, 2f
    lw      t3, 0(t2)
    sw      t3, 0(t0)
    addi    t0, t0, 4
    addi    t2, t2, 4
    j       1b
2:
    la      t0, _bss
    la      t1, _ebss
1:  beq     t0, t1, 2f
    sw      x0, 0(t0)
    addi    t0, t0, 4
    j       1b
2:
    j   _start
