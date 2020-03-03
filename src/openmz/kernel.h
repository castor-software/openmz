/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#ifndef KERNEL_H
#define KERNEL_H

#include "config.h"
#include "const.h"
#include "csr.h"
#include "reg_index.h"
#include "timer.h"
#include "types.h"

/* A message for one zone */
typedef struct inbox {
    /* if full != 0, then we have a message, else no message. */
    uptr full;
    /*
     * uptr is 32-64 bits depending on arch
     * If uptr has 32-bits, set msgs length to 4 otherwise 2.
     * This ensures that we have 128-bit messages at all time.
     */
    uptr msgs[SEL(4, 2)];
} Inbox;

/* all information we need about a zone */
typedef struct zone {
    uptr id;
    /* gpr + pc */
    uptr regs[32];
    /* pmp configuration */
    u64 pmpcfg0;
    uptr pmpaddr[8];
    /* trap handling */
    /* ustatus[0] = interrupt enabled, ustatus[4] = prev. interrupt enabled */
    uptr ustatus;
    uptr uie;
    uptr utval;
    uptr uepc;
    uptr ucause;
    /* user mtimecmp emulation */
    u64 timecmp;
    uptr trap_handlers[N_TRAPS];
    /* messages */
    Inbox inboxes[N_ZONES];
} Zone;

/* interrupt information */
typedef struct irq_handler {
    /* The zone handling the interrupt */
    Zone* zone;
    /* The pointer to the interrupt handler */
    uptr handler;
} IrqHandler;

typedef struct kernel {
    /* data */
    Zone zones[N_ZONES];
    /* irq_num => handler mapping */
    IrqHandler irq_handlers[N_INTERRUPTS];
    /* trap data */
    uptr mcause, mtval, mie, mip;
    /* the current running zone */
    Zone* current;
    /* the next zone to run */
    Zone* next;
    sptr next_quantum;
    /* interrupt handling mode */
    uptr is_isr;
} Kernel;

#define KERNEL(x) (kernel.x)
#define CURRENT (*kernel.current)
#define ZONES (kernel.zones)
extern Kernel kernel;

/* Kernel initialization routine */
void KernelInit(void);
/* Entry routine preparing for trap handling */
void KernelEntry(void);
/* Exit routine, starting the user process */
void KernelExit(void);

static inline void LoadNextZone(void)
{
    /* let next zone be current zone and set timer */
    KERNEL(current) = KERNEL(next);
    WriteMtimecmp(ReadMtime() + KERNEL(next_quantum));
    /* set the next zone in round robin fashion */
    KERNEL(next) = KERNEL(current) + 1;
    if (KERNEL(next) == KERNEL(zones) + N_ZONES)
        KERNEL(next) = KERNEL(zones);
    KERNEL(next_quantum) = QUANTUM;
    /* we are not in isr mode */
    KERNEL(is_isr) = 0;
}

#endif /* KERNEL_H */
