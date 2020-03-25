/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#include "kernel.h"
#include "scheduler.h"
#include "timer.h"

Kernel kernel = { .zones = ZONE_CONFIG, .irq_handlers = IRQ_CONFIG };

extern void AsmRestoreContext(uptr regs[32]) __attribute__((noreturn));
void RestoreContext(void)
{
    // Here we should clear cache.
    Zone* zone = HART.current;
#if REGBITS == 32
    CSRW(pmpcfg0, zone->pmpcfg0);
    CSRW(pmpcfg1, zone->pmpcfg1);
#else
    CSRW(pmpcfg0, (u64)(zone->pmpcfg1 << 32) | zone->pmpcfg0);
#endif
    CSRW(pmpaddr0, zone->pmpaddr[0]);
    CSRW(pmpaddr1, zone->pmpaddr[1]);
    CSRW(pmpaddr2, zone->pmpaddr[2]);
    CSRW(pmpaddr3, zone->pmpaddr[3]);
    CSRW(pmpaddr4, zone->pmpaddr[4]);
    CSRW(pmpaddr5, zone->pmpaddr[5]);
    CSRW(pmpaddr6, zone->pmpaddr[6]);
    CSRW(pmpaddr7, zone->pmpaddr[7]);
    AsmRestoreContext(zone->regs);
}

void KernelInit(void)
{
    // Kernel csr initialization.
    CSRW(mtvec, AsmTrapEntry);
    CSRW(mstatus, 0);
    CSRW(mcounteren, 0xF);
    // set the zones' id
    for (int i = 0; i < N_ZONES; ++i)
        ZONES[i].id = i + 1;
    // initialize the scheduler
    InitScheduler();
    // Start zone context
    RestoreContext();
}
