/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#include "kernel.h"

#include "machine.h"
#include "sched.h"

#include "csr.h"

kernel_t kernel = { .zones = { ZONE_CONFIG }, .intrp_handlers = { IRQ_CONFIG } };

extern void asm_restore_context(uintptr_t[32], uint32_t[2], uintptr_t[8]) __attribute__((noreturn));
void restore_context(void)
{
    clear_cache();
    asm_restore_context(CURRENT.regs, CURRENT.pmpcfg, CURRENT.pmpaddr);
}

extern void asm_trap_entry(void) __attribute__((noreturn));
void kmain(void)
{
    // Kernel csr initialization.
    CSRW(mtvec, asm_trap_entry);
    CSRW(mstatus, 0);
    // set the zones' id
    for (int i = 0; i < N_ZONES; ++i)
        ZONES[i].id = i + 1;
    // initilize the hardware
    init_hardware();
    // initialize the scheduler
    sched_init();
    // Start zone context
    restore_context();
}
