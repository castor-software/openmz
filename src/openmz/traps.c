/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#include "traps.h"
#include "ecall.h"
#include "illegal_instruction.h"
#include "scheduler.h"
#include "timer.h"
#include "user_timer.h"
#include "user_trap.h"

#define INTERRUPT_BIT (1 << (REGBITS - 1))
#define MMODE_IPI (INTERRUPT_BIT | 3)
#define MMODE_TIMER (INTERRUPT_BIT | 7)

#define ILLEGAL_INSTRUCTION 2
#define UMODE_ECALL 8

void HandleInterrupt(uptr mcause, uptr mtval)
{
    /* Interrupt handling */
    switch (mcause) {
    case MMODE_TIMER:
        HandleUserTimer();
        break;
    default:
        HandleUserTrap(mcause, mtval);
    }
    Scheduler();
    RestoreContext();
}

void HandleException(uptr mcause, uptr mtval)
{
    /* Exception handling */
    switch (mcause) {
    case UMODE_ECALL:
        HandleEcall();
        break;
    case ILLEGAL_INSTRUCTION:
        HandleIllegalInstruction(mcause, mtval);
        break;
    default:
        HandleUserTrap(mcause, mtval);
    }
    Scheduler();
    RestoreContext();
}
