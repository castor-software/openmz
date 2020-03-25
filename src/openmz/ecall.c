/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */

#include "ecall.h"
#include "timer.h"

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

/* handler takes a caller and its argument registers a0-a7 */
typedef void (*Handler)(Zone* caller, uptr args[8]);

static void EcallYield(Zone UNUSED(*caller), uptr UNUSED(args[8]))
{
    HART.event = EVENT_YIELD;
}

static void EcallWfi(Zone* UNUSED(caller), uptr UNUSED(args[8]))
{
    HART.event = EVENT_WFI;
}

static void EcallSend(Zone* caller, uptr args[8])
{
    uptr receiver = args[0] - 1;
    args[0] = 0;
    if (receiver >= N_ZONES)
        return;
    Inbox* inbox = &INBOXES[receiver][caller->id - 1];
    if (inbox->full)
        return;
    for (int i = 0; i < N_MSGS; ++i)
        inbox->msgs[i] = args[1 + i];
    args[0] = 1;
}

static void EcallRecv(Zone* caller, uptr args[8])
{
    uptr sender = args[0] - 1;
    args[0] = 0;
    if (sender >= N_ZONES)
        return;
    Inbox* inbox = &INBOXES[caller->id - 1][sender];
    if (!inbox->full)
        return;
    for (int i = 0; i < N_MSGS; ++i)
        args[i + 1] = inbox->msgs[i];
    args[0] = 1;
}

static void EcallTrpVect(Zone* caller, uptr args[8])
{
    uptr code = args[0];
    uptr handler = args[1];
    if (code >= N_TRAPS)
        return;
    caller->trap_handlers[code] = handler;
}

static void EcallIrqVect(Zone* caller, uptr args[8])
{
    uptr code = args[0];
    uptr handler = args[1];
    if ((code >= N_INTERRUPTS) || (IRQ_HANDLERS[code].zone != caller))
        return;
    IRQ_HANDLERS[code].handler = handler;
    if (handler)
        caller->uie |= (1 << code);
    else
        caller->uie &= ~(1 << code);
}

static void EcallCsrsMie(Zone* caller, uptr UNUSED(args[8]))
{
    caller->ustatus |= USTATUS_IE;
}

static void EcallCsrcMie(Zone* caller, uptr UNUSED(args[8]))
{
    caller->ustatus &= ~USTATUS_IE;
}

static void EcallCsrwMtimecmp(Zone* caller, uptr args[8])
{
    caller->deadline = *((u64*)args);
    if ((caller->ustatus & USTATUS_IE)
        && caller->deadline && (caller->deadline < HART.deadline)) {
        // zone interrupts are enabled
        // and deadline is in current timeslice
        SetDeadline(caller->deadline);
    }
}

static void EcallCsrrMtime(Zone* UNUSED(caller), uptr args[8])
{
    *((u64*)args) = GetTime();
}

static void EcallCsrrMcycle(Zone* UNUSED(caller), uptr args[8])
{
    CSRR_COUNTER(args[0], mcycle);
}

static void EcallCsrrMinstr(Zone* UNUSED(caller), uptr args[8])
{
    CSRR_COUNTER(args[0], minstret);
}

static void EcallCsrrMhpmc3(Zone* UNUSED(caller), uptr args[8])
{
    CSRR_COUNTER(args[0], mhpmcounter3);
}

static void EcallCsrrMhpmc4(Zone* UNUSED(caller), uptr args[8])
{
    CSRR_COUNTER(args[0], mhpmcounter4);
}

static void EcallCsrrMisa(Zone* UNUSED(caller), uptr args[8])
{
    CSRR64(args[0], misa);
}

static void EcallCsrrMvendid(Zone* UNUSED(caller), uptr args[8])
{
    CSRR64(args[0], mvendorid);
}

static void EcallCsrrMarchid(Zone* UNUSED(caller), uptr args[8])
{
    CSRR64(args[0], marchid);
}

static void EcallCsrrMimpid(Zone* UNUSED(caller), uptr args[8])
{
    CSRR64(args[0], mimpid);
}

static void EcallCsrrMhartid(Zone* UNUSED(caller), uptr args[8])
{
    CSRR64(args[0], mhartid);
}

void HandleEcall(void)
{
    /* ecall table */
    static const Handler handlers[] = {
        [ECALL_YIELD] = EcallYield,
        [ECALL_WFI] = EcallWfi,
        [ECALL_SEND] = EcallSend,
        [ECALL_RECV] = EcallRecv,
        [ECALL_TRP_VECT] = EcallTrpVect,
        [ECALL_IRQ_VECT] = EcallIrqVect,
        [ECALL_CSRS_MIE] = EcallCsrsMie,
        [ECALL_CSRC_MIE] = EcallCsrcMie,
        [ECALL_CSRW_MTIMECMP] = EcallCsrwMtimecmp,
        [ECALL_CSRR_MTIME] = EcallCsrrMtime,
        [ECALL_CSRR_MCYCLE] = EcallCsrrMcycle,
        [ECALL_CSRR_MINSTR] = EcallCsrrMinstr,
        [ECALL_CSRR_MHPMC3] = EcallCsrrMhpmc3,
        [ECALL_CSRR_MHPMC4] = EcallCsrrMhpmc4,
        [ECALL_CSRR_MISA] = EcallCsrrMisa,
        [ECALL_CSRR_MVENDID] = EcallCsrrMvendid,
        [ECALL_CSRR_MARCHID] = EcallCsrrMarchid,
        [ECALL_CSRR_MIMPID] = EcallCsrrMimpid,
        [ECALL_CSRR_MHARTID] = EcallCsrrMhartid,
    };

    Zone* caller = &CURRENT;
    /* always advance PC! */
    caller->regs[PC] += 4;
    uptr ecall = caller->regs[A7];
    if (ecall < ARRAY_LEN(handlers))
        handlers[ecall](caller, caller->regs + A0);
}
