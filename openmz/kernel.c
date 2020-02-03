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
Kernel kernel = {.zones = {ZONE_CONFIG}, .irq_handlers = {IRQ_CONFIG}};

extern void AsmTrapVector(void);
extern void AsmLoadContext(uptr regs[32]) __attribute__((noreturn));

void KernelInit(void)
{
    /* kernel initialization */
    write_csr(mtvec, AsmTrapVector);
    write_csr(mstatus, 0);
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
    KERNEL(mcause) = read_csr(mcause);
    KERNEL(mtval) = read_csr(mtval);
    KERNEL(mip) = read_csr(mip);
}
void KernelExit(void)
{
    if (KERNEL(is_isr))
        write_csr(mie, MIE_TIMER);
    else
        write_csr(mie, KERNEL(mie) | MIE_TIMER);
    /* set pmp */
    write_csr(pmpcfg0, CURRENT.pmpcfg0);
#if RV32
    write_csr(pmpcfg1, CURRENT.pmpcfg0 >> 32);
#endif
    write_csr(pmpaddr0, CURRENT.pmpaddr[0]);
    write_csr(pmpaddr1, CURRENT.pmpaddr[1]);
    write_csr(pmpaddr2, CURRENT.pmpaddr[2]);
    write_csr(pmpaddr3, CURRENT.pmpaddr[3]);
    write_csr(pmpaddr4, CURRENT.pmpaddr[4]);
    write_csr(pmpaddr5, CURRENT.pmpaddr[5]);
    write_csr(pmpaddr6, CURRENT.pmpaddr[6]);
    write_csr(pmpaddr7, CURRENT.pmpaddr[7]);
    /* load the context */
    AsmLoadContext(CURRENT.regs);
}
