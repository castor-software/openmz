/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */

#include "machine.h"
#include "sched.h"

#include "const.h"
#include "csr.h"
#include "kernel.h"
#include "macro.h"
#include "traps.h"

/* handler takes a caller and its argument registers a0-a7 */
typedef void (*handler_t)(zone_t* caller, uintptr_t args[8]);

/* ECALL numbers */
#define ECALL_YIELD 0
#define ECALL_WFI 1
#define ECALL_SEND 2
#define ECALL_RECV 3
#define ECALL_TRP_VECT 4
#define ECALL_IRQ_VECT 5
#define ECALL_CSRS_MIE 6
#define ECALL_CSRC_MIE 7
#define ECALL_CSRW_MTIMECMP 8
#define ECALL_CSRR_MTIME 9
#define ECALL_CSRR_MCYCLE 10
#define ECALL_CSRR_MINSTR 11
#define ECALL_CSRR_MHPMC3 12
#define ECALL_CSRR_MHPMC4 13
#define ECALL_CSRR_MISA 14
#define ECALL_CSRR_MVENDID 15
#define ECALL_CSRR_MARCHID 16
#define ECALL_CSRR_MIMPID 17
#define ECALL_CSRR_MHARTID 18

void ecall_yield(zone_t* caller, uintptr_t args[8])
{
    KERNEL.deadline = read_mtime();
    sched_yield();
}

static void ecall_send(zone_t* caller, uintptr_t args[8])
{
    uintptr_t receiver = args[0] - 1;
    args[0] = 0;
    if (receiver >= N_ZONES)
        return;
    inbox_t* inbox = &INBOXES[receiver][caller->id - 1];
    if (inbox->full)
        return;
    inbox->full = 1;
    for (int i = 0; i < N_MSGS; ++i)
        inbox->msgs[i] = args[1 + i];
    args[0] = 1;
}

static void ecall_recv(zone_t* caller, uintptr_t args[8])
{
    uintptr_t sender = args[0] - 1;
    args[0] = 0;
    if (sender >= N_ZONES)
        return;
    inbox_t* inbox = &INBOXES[caller->id - 1][sender];
    if (!inbox->full)
        return;
    inbox->full = 0;
    for (int i = 0; i < N_MSGS; ++i)
        args[i + 1] = inbox->msgs[i];
    args[0] = 1;
}

static void ecall_trp_vect(zone_t* caller, uintptr_t args[8])
{
    uintptr_t code = args[0];
    uintptr_t handler = args[1];
    if (code >= N_TRAPS)
        return;
    caller->trap_handlers[code] = handler;
    if (code == UMODE_SOFT_TIMER)
        caller->deadline = 0;
}

static void ecall_irq_vect(zone_t* caller, uintptr_t args[8])
{
    uintptr_t code = args[0];
    uintptr_t handler = args[1];
    if ((code >= N_INTERRUPTS) || (INTRP_HANDLERS[code].zone != caller))
        return;
    INTRP_HANDLERS[code].handler = handler;
    if (handler) {
        caller->uie |= (1 << code);
        if (caller->ustatus & USTATUS_IE) {
            KERNEL.mie |= (1 << code);
        }
    } else {
        caller->uie &= ~(1 << code);
        if (caller->ustatus & USTATUS_IE) {
            KERNEL.mie &= ~(1 << code);
        }
    }
}

static void ecall_csrs_mie(zone_t* caller, uintptr_t UNUSED(args[8]))
{
    caller->ustatus |= USTATUS_IE;
    KERNEL.mie |= caller->uie;
}

static void ecall_csrc_mie(zone_t* caller, uintptr_t UNUSED(args[8]))
{
    caller->ustatus &= ~USTATUS_IE;
    KERNEL.mie &= ~caller->uie;
}

static void ecall_csrw_mtimecmp(zone_t* caller, uintptr_t args[8])
{
    caller->deadline = *((uint64_t*)args);
}

static void ecall_csrr_mtime(zone_t* caller, uintptr_t args[8])
{
    *((uint64_t*)args) = read_mtime();
}

static void ecall_csrr_mcycle(zone_t* caller, uintptr_t args[8])
{
    CSRR_COUNTER(args[0], mcycle);
}

static void ecall_csrr_minstr(zone_t* caller, uintptr_t args[8])
{
    CSRR_COUNTER(args[0], minstret);
}

static void ecall_csrr_mhpmc3(zone_t* caller, uintptr_t args[8])
{
    CSRR_COUNTER(args[0], mhpmcounter3);
}

static void ecall_csrr_mhpmc4(zone_t* caller, uintptr_t args[8])
{
    CSRR_COUNTER(args[0], mhpmcounter4);
}

static void ecall_csrr_misa(zone_t* caller, uintptr_t args[8])
{
    CSRR64(args[0], misa);
}

static void ecall_csrr_mvendid(zone_t* caller, uintptr_t args[8])
{
    CSRR64(args[0], mvendorid);
}

static void ecall_csrr_marchid(zone_t* caller, uintptr_t args[8])
{
    CSRR64(args[0], marchid);
}

static void ecall_csrr_mimpid(zone_t* caller, uintptr_t args[8])
{
    CSRR64(args[0], mimpid);
}

static void ecall_csrr_mhartid(zone_t* caller, uintptr_t args[8])
{
    CSRR64(args[0], mhartid);
}

static const handler_t ecall_handlers[] = {
    [ECALL_YIELD] = ecall_yield,
    [ECALL_WFI] = ecall_yield,
    [ECALL_SEND] = ecall_send,
    [ECALL_RECV] = ecall_recv,
    [ECALL_TRP_VECT] = ecall_trp_vect,
    [ECALL_IRQ_VECT] = ecall_irq_vect,
    [ECALL_CSRS_MIE] = ecall_csrs_mie,
    [ECALL_CSRC_MIE] = ecall_csrc_mie,
    [ECALL_CSRW_MTIMECMP] = ecall_csrw_mtimecmp,
    [ECALL_CSRR_MTIME] = ecall_csrr_mtime,
    [ECALL_CSRR_MCYCLE] = ecall_csrr_mcycle,
    [ECALL_CSRR_MINSTR] = ecall_csrr_minstr,
    [ECALL_CSRR_MHPMC3] = ecall_csrr_mhpmc3,
    [ECALL_CSRR_MHPMC4] = ecall_csrr_mhpmc4,
    [ECALL_CSRR_MISA] = ecall_csrr_misa,
    [ECALL_CSRR_MVENDID] = ecall_csrr_mvendid,
    [ECALL_CSRR_MARCHID] = ecall_csrr_marchid,
    [ECALL_CSRR_MIMPID] = ecall_csrr_mimpid,
    [ECALL_CSRR_MHARTID] = ecall_csrr_mhartid,
};

void handle_ecall(uintptr_t mcause, uintptr_t mtval)
{
    /* ecall table */

    zone_t* caller = &CURRENT;
    /* always advance PC! */
    caller->regs[PC] += 4;
    uintptr_t ecall = caller->regs[A7];
    if (ecall < ARRAY_LEN(ecall_handlers))
        ecall_handlers[ecall](caller, caller->regs + A0);
}
