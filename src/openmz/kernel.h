/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.  */
#ifndef KERNEL_H
#define KERNEL_H

#include "config.h"
#include "csr.h"
#include "macro.h"

#define EVENT_YIELD 1
#define EVENT_WFI 2
#define EVENT_URET 3
#define EVENT_INTERRUPT 4

#define USTATUS_IE 1
#define USTATUS_PIE (1 << 4)

typedef unsigned int u32;
typedef unsigned long long u64;
typedef unsigned long uptr;
typedef signed int s32;
typedef signed long long s64;
typedef signed long sptr;

/* Inbox for inter-zone communication. */
typedef struct inbox {
    // if full != 0, then we have a message, else no message.
    uptr full;
    // uptr is 32-64 bits depending on for RV32 and RV64 resp.
    // If uptr has 32-bits, we set msgs length to 4 otherwise 2.
    // This ensures that we have 128-bit messages at all time.
#define N_MSGS (128 / REGBITS)
    uptr msgs[N_MSGS];
} Inbox;

/* Zone data, e.g. Processor Control Block (PCB) */
typedef struct zone {
    uptr id;
    // zone context, pc + x1-x31
    uptr regs[32];
    // pmp configuration registers
    u32 pmpcfg0;
    u32 pmpcfg1;
    uptr pmpaddr[8];
    // trap handling regusters
    // ustatus[0] = interrupt enabled
    // ustatus[4] = prev. interrupt enabled */
    uptr ustatus;
    uptr uie;
    uptr utval;
    uptr uepc;
    uptr ucause;
    // user mtimecmp emulation
    u64 deadline;
    // exception handlers
    uptr trap_handlers[N_TRAPS];
} Zone;

/* Interrupt handler. */
typedef struct irq_handler {
    /* The zone handling the interrupt */
    Zone* zone;
    /* The pointer to the interrupt handler */
    uptr handler;
} IrqHandler;

/* Scheduling information per-hart */
typedef struct hart {
    Zone* current;
    u64 deadline;
    Zone* next;
    u64 next_quantum;
    uptr event;
    uptr state;
} Hart;

/* Kernel data */
typedef struct kernel {
    /* Scheduler data */
    Hart hart;
    /* Zone data */
    Zone zones[N_ZONES];
    /* Inboxes[receiver][sender]*/
    Inbox inboxes[N_ZONES][N_ZONES];
    /* irq_num => handler mapping */
    IrqHandler irq_handlers[N_INTERRUPTS];
} Kernel;

/* The ONLY global data */
extern Kernel kernel;

#define ZONES (kernel.zones)
#define IRQ_HANDLERS (kernel.irq_handlers)
#define INBOXES (kernel.inboxes)
#define HART (kernel.hart)
#define CURRENT (*kernel.hart.current)

/* Initialize the kernel and start the first zone */
void InitKernel(void);
/* Trap entry point, is in assembly */
void AsmTrapEntry(void);
/* Load zone/user context */
void RestoreContext(void) __attribute__((noreturn));

#endif /* KERNEL_H */
