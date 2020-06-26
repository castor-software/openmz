/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to 
 * the terms of the GNU General Public License version 2. 
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#include "macro.h"

.extern kernel_stack
.extern __global_pointer$
.extern HandleEvent
.extern Scheduler

.globl asm_trap_entry 
.globl asm_restore_context 

.align 4
// void asm_trap_entry(void)
asm_trap_entry:
    // load regs, store user a0
    csrrw   a0, mscratch, a0
    beqz	a0, hang
    // save x1-x31 to regs
    REG_S   ra, (REGBYTES * RA)(a0)
    REG_S   sp, (REGBYTES * SP)(a0)
    REG_S   gp, (REGBYTES * GP)(a0)
    REG_S   tp, (REGBYTES * TP)(a0)
    REG_S   t0, (REGBYTES * T0)(a0)
    REG_S   t1, (REGBYTES * T1)(a0)
    REG_S   t2, (REGBYTES * T2)(a0)
    REG_S   s0, (REGBYTES * S0)(a0)
    REG_S   s1, (REGBYTES * S1)(a0)
    REG_S   a1, (REGBYTES * A1)(a0)
    REG_S   a2, (REGBYTES * A2)(a0)
    REG_S   a3, (REGBYTES * A3)(a0)
    REG_S   a4, (REGBYTES * A4)(a0)
    REG_S   a5, (REGBYTES * A5)(a0)
    REG_S   a6, (REGBYTES * A6)(a0)
    REG_S   a7, (REGBYTES * A7)(a0)
    REG_S   s2, (REGBYTES * S2)(a0)
    REG_S   s3, (REGBYTES * S3)(a0)
    REG_S   s4, (REGBYTES * S4)(a0)
    REG_S   s5, (REGBYTES * S5)(a0)
    REG_S   s6, (REGBYTES * S6)(a0)
    REG_S   s7, (REGBYTES * S7)(a0)
    REG_S   s8, (REGBYTES * S8)(a0)
    REG_S   s9, (REGBYTES * S9)(a0)
    REG_S   s10, (REGBYTES * S10)(a0)
    REG_S   s11, (REGBYTES * S11)(a0)
    REG_S   t3, (REGBYTES * T3)(a0)
    REG_S   t4, (REGBYTES * T4)(a0)
    REG_S   t5, (REGBYTES * T5)(a0)
    REG_S   t6, (REGBYTES * T6)(a0)
    csrrw   t0, mscratch, x0
    REG_S   t0, (REGBYTES * A0)(a0)
    // save pc to regs
    csrr    t0, mepc
    REG_S   t0, (REGBYTES * PC)(a0)

    // load kernel gp and sp
    .option push
    .option norelax
    la      gp, __global_pointer$
    .option pop
    la      sp, _sp

    // get arguments mcause, mtval
    csrr    a0, mcause
    csrr    a1, mtval
    tail    handle_event


/** 
 * void asm_restore_context(regs, pmpcfg, pmpaddr)
 * Restores the zone's context from its register `regs`. 
 * Does not setup the pmp registers.
 */
asm_restore_context:
    mv      s0, a0
    mv      s1, a1
    mv      s2, a2
    // load pmpaddr
    REG_L   a0, (REGBYTES * 0)(s2)
    REG_L   a1, (REGBYTES * 1)(s2)
    REG_L   a2, (REGBYTES * 2)(s2)
    REG_L   a3, (REGBYTES * 3)(s2)
    REG_L   a4, (REGBYTES * 4)(s2)
    REG_L   a5, (REGBYTES * 5)(s2)
    REG_L   a6, (REGBYTES * 6)(s2)
    REG_L   a7, (REGBYTES * 7)(s2)
    csrw    pmpaddr0, a0
    csrw    pmpaddr1, a1
    csrw    pmpaddr2, a2
    csrw    pmpaddr3, a3
    csrw    pmpaddr4, a4
    csrw    pmpaddr5, a5
    csrw    pmpaddr6, a6
    csrw    pmpaddr7, a7

    // load pmpcfg
    REG_L   a0, 0(s1)
    csrw    pmpcfg0, a0
#if __riscv_xlen == 32
    REG_L   a1, REGBYTES(s1)
    csrw    pmpcfg1, a1
#endif

    // save pointer to regs[32]
    csrw    mscratch, s0
    // load pc from regs
    REG_L   t0, (REGBYTES * PC)(s0)
    csrw    mepc, t0
    // load x1-x31 from regs
    REG_L   ra, (REGBYTES * RA)(s0)
    REG_L   sp, (REGBYTES * SP)(s0)
    REG_L   gp, (REGBYTES * GP)(s0)
    REG_L   tp, (REGBYTES * TP)(s0)
    REG_L   t0, (REGBYTES * T0)(s0)
    REG_L   t1, (REGBYTES * T1)(s0)
    REG_L   t2, (REGBYTES * T2)(s0)
    REG_L   s1, (REGBYTES * S1)(s0)
    REG_L   a0, (REGBYTES * A0)(s0)
    REG_L   a1, (REGBYTES * A1)(s0)
    REG_L   a2, (REGBYTES * A2)(s0)
    REG_L   a3, (REGBYTES * A3)(s0)
    REG_L   a4, (REGBYTES * A4)(s0)
    REG_L   a5, (REGBYTES * A5)(s0)
    REG_L   a6, (REGBYTES * A6)(s0)
    REG_L   a7, (REGBYTES * A7)(s0)
    REG_L   s2, (REGBYTES * S2)(s0)
    REG_L   s3, (REGBYTES * S3)(s0)
    REG_L   s4, (REGBYTES * S4)(s0)
    REG_L   s5, (REGBYTES * S5)(s0)
    REG_L   s6, (REGBYTES * S6)(s0)
    REG_L   s7, (REGBYTES * S7)(s0)
    REG_L   s8, (REGBYTES * S8)(s0)
    REG_L   s9, (REGBYTES * S9)(s0)
    REG_L   s10,(REGBYTES * S10)(s0)
    REG_L   s11,(REGBYTES * S11)(s0)
    REG_L   t3, (REGBYTES * T3)(s0)
    REG_L   t4, (REGBYTES * T4)(s0)
    REG_L   t5, (REGBYTES * T5)(s0)
    REG_L   t6, (REGBYTES * T6)(s0)
    REG_L   s0, (REGBYTES * S0)(s0)
    // go to user mode
    mret

hang:
	ebreak
	j		hang
