/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#include "kernel.h"

#define MIE_TIMER (1 << 7)

/* contains ALL global data */
Kernel kernel = { .zones = { ZONE_CONFIG }, .irq_handlers = { IRQ_CONFIG } };

extern void AsmTrapVector(void);
extern void AsmLoadContext(uptr regs[32]) __attribute__((noreturn));

void KernelInit(void)
{
    /* kernel initialization */
    CSRW(mtvec, AsmTrapVector);
    CSRW(mstatus, 0);
    /* set the id's */
    for (int i = 0; i < N_ZONES; ++i) {
        ZONES[i].id = i + 1;
    }
    /* load the first zone */
    KERNEL(next) = KERNEL(zones);
    KERNEL(next_quantum) = QUANTUM;
    LoadNextZone();
    KernelExit();
}

void KernelEntry(void)
{
    /* save trap information */
    CSRR(KERNEL(mcause), mcause);
    CSRR(KERNEL(mtval), mtval);
    CSRR(KERNEL(mip), mip);
}
void KernelExit(void)
{
    if (KERNEL(is_isr))
        CSRW(mie, MIE_TIMER);
    else
        CSRW(mie, KERNEL(mie) | MIE_TIMER);
        /* set pmp */
#if RV32
    CSRW(pmpcfg0, (u32)CURRENT.pmpcfg0);
    CSRW(pmpcfg1, (u32)(CURRENT.pmpcfg0 >> 32));
#else
    CSRW(pmpcfg0, CURRENT.pmpcfg0);
#endif
    CSRW(pmpaddr0, CURRENT.pmpaddr[0]);
    CSRW(pmpaddr1, CURRENT.pmpaddr[1]);
    CSRW(pmpaddr2, CURRENT.pmpaddr[2]);
    CSRW(pmpaddr3, CURRENT.pmpaddr[3]);
    CSRW(pmpaddr4, CURRENT.pmpaddr[4]);
    CSRW(pmpaddr5, CURRENT.pmpaddr[5]);
    CSRW(pmpaddr6, CURRENT.pmpaddr[6]);
    CSRW(pmpaddr7, CURRENT.pmpaddr[7]);
    /* load the context */
    AsmLoadContext(CURRENT.regs);
}
