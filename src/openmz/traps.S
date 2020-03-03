/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to 
 * the terms of the GNU General Public License version 2. 
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#include "asm.h"
#include "reg_index.h"

.extern kernel_stack
.extern __global_pointer$
.extern HandleEvent

.globl AsmTrapVector
.globl AsmLoadContext

.align 4
AsmTrapVector:
        /* load trap_frame pointer */
        csrrw   a0, mscratch, a0
        beqz    a0, _hang
        /* save gpr */
        REG_S   ra,  (XBYTES * RA)(a0)
        REG_S   sp,  (XBYTES * SP)(a0)
        REG_S   gp,  (XBYTES * GP)(a0)
        REG_S   tp,  (XBYTES * TP)(a0)
        REG_S   t0,  (XBYTES * T0)(a0)
        REG_S   t1,  (XBYTES * T1)(a0)
        REG_S   t2,  (XBYTES * T2)(a0)
        REG_S   s0,  (XBYTES * S0)(a0)
        REG_S   s1,  (XBYTES * S1)(a0)
        REG_S   a1,  (XBYTES * A1)(a0)
        REG_S   a2,  (XBYTES * A2)(a0)
        REG_S   a3,  (XBYTES * A3)(a0)
        REG_S   a4,  (XBYTES * A4)(a0)
        REG_S   a5,  (XBYTES * A5)(a0)
        REG_S   a6,  (XBYTES * A6)(a0)
        REG_S   a7,  (XBYTES * A7)(a0)
        REG_S   s2,  (XBYTES * S2)(a0)
        REG_S   s3,  (XBYTES * S3)(a0)
        REG_S   s4,  (XBYTES * S4)(a0)
        REG_S   s5,  (XBYTES * S5)(a0)
        REG_S   s6,  (XBYTES * S6)(a0)
        REG_S   s7,  (XBYTES * S7)(a0)
        REG_S   s8,  (XBYTES * S8)(a0)
        REG_S   s9,  (XBYTES * S9)(a0)
        REG_S   s10, (XBYTES * S10)(a0)
        REG_S   s11, (XBYTES * S11)(a0)
        REG_S   t3,  (XBYTES * T3)(a0)
        REG_S   t4,  (XBYTES * T4)(a0)
        REG_S   t5,  (XBYTES * T5)(a0)
        REG_S   t6,  (XBYTES * T6)(a0)
        csrr    t0,  mepc
        REG_S   t0,  (XBYTES * PC)(a0)
        csrrw   t0,  mscratch, zero
        REG_S   t0,  (XBYTES * A0)(a0)

        .option push
        .option norelax
        la      gp, __global_pointer$
        .option pop
        la      sp, _sp

        call    KernelEntry
        call    HandleEvent
        tail    KernelExit

AsmLoadContext:
        /* save pointer to registers */
        csrw    mscratch, a0
        /* load pc */
        REG_L   t0,  (XBYTES * PC)(a0)
        csrw    mepc, t0
        /* load gpr */
        REG_L   ra,  (XBYTES * RA)(a0)
        REG_L   sp,  (XBYTES * SP)(a0)
        REG_L   gp,  (XBYTES * GP)(a0)
        REG_L   tp,  (XBYTES * TP)(a0)
        REG_L   t0,  (XBYTES * T0)(a0)
        REG_L   t1,  (XBYTES * T1)(a0)
        REG_L   t2,  (XBYTES * T2)(a0)
        REG_L   s0,  (XBYTES * S0)(a0)
        REG_L   s1,  (XBYTES * S1)(a0)
        REG_L   a1,  (XBYTES * A1)(a0)
        REG_L   a2,  (XBYTES * A2)(a0)
        REG_L   a3,  (XBYTES * A3)(a0)
        REG_L   a4,  (XBYTES * A4)(a0)
        REG_L   a5,  (XBYTES * A5)(a0)
        REG_L   a6,  (XBYTES * A6)(a0)
        REG_L   a7,  (XBYTES * A7)(a0)
        REG_L   s2,  (XBYTES * S2)(a0)
        REG_L   s3,  (XBYTES * S3)(a0)
        REG_L   s4,  (XBYTES * S4)(a0)
        REG_L   s5,  (XBYTES * S5)(a0)
        REG_L   s6,  (XBYTES * S6)(a0)
        REG_L   s7,  (XBYTES * S7)(a0)
        REG_L   s8,  (XBYTES * S8)(a0)
        REG_L   s9,  (XBYTES * S9)(a0)
        REG_L   s10, (XBYTES * S10)(a0)
        REG_L   s11, (XBYTES * S11)(a0)
        REG_L   t3,  (XBYTES * T3)(a0)
        REG_L   t4,  (XBYTES * T4)(a0)
        REG_L   t5,  (XBYTES * T5)(a0)
        REG_L   t6,  (XBYTES * T6)(a0)
        REG_L   a0,  (XBYTES * A0)(a0)
        fence
        mret                    
