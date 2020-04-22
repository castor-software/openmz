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
#include "macro.h"
#include <stdint.h>

#define USTATUS_IE 1
#define USTATUS_PIE (1 << 4)

#define N_MSGS SEL(4, 2)

/* inbox_t for inter-zone communication. */
typedef struct inbox {
    // if full != 0, then we have a message, else no message.
    uintptr_t full;
    // 128-bit message
    uintptr_t msgs[N_MSGS];
} inbox_t;

/* zone_t data, e.g. Processor Control Block (PCB) */
typedef struct zone {
    uintptr_t id;
    // zone context, pc + x1-x31
    uintptr_t regs[32];
    // pmp configuration registers
    uint32_t pmpcfg[2];
    uintptr_t pmpaddr[8];
    // trap handling regusters
    // ustatus[0] = interrupt enabled
    // ustatus[4] = prev. interrupt enabled */
    uintptr_t ustatus;
    uintptr_t uie;
    uintptr_t utval;
    uintptr_t uepc;
    uintptr_t ucause;
    // user mtimecmp emulation
    uint64_t deadline;
    // exception handlers
    uintptr_t trap_handlers[N_TRAPS];
} zone_t;

/* Interrupt handler. */
typedef struct intrp_handler {
    /* The zone handling the interrupt */
    zone_t* zone;
    /* The pointer to the interrupt handler */
    uintptr_t handler;
} intrp_handler_t;

/* Kernel data */
typedef struct kernel {
    /* Scheduler data */
    zone_t* current;
    uint64_t deadline;
    zone_t* next;
    uint64_t next_quantum;
    uintptr_t mie;
    uintptr_t intrp_mode;
    /* zone_t data */
    zone_t zones[N_ZONES];
    /* inbox_tes[receiver][sender]*/
    inbox_t inboxes[N_ZONES][N_ZONES];
    /* irq_num => handler mapping */
    intrp_handler_t intrp_handlers[N_INTERRUPTS];
} kernel_t;

/* The ONLY global data */
extern kernel_t kernel;

#define KERNEL kernel
#define ZONES (kernel.zones)
#define INTRP_HANDLERS (kernel.intrp_handlers)
#define INBOXES (kernel.inboxes)
#define CURRENT (*kernel.current)

/* Load zone/user context */
void restore_context(void) __attribute__((noreturn));

#endif // KERNEL_H
