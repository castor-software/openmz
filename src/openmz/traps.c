/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#include "traps.h"

#include "machine.h"
#include "sched.h"

#include "const.h"
#include "kernel.h"

typedef void (*handler_t)(uintptr_t, uintptr_t);
static const handler_t intrp_handlers[12] = {
    [0] = handle_user_intrp,
    [1] = handle_user_intrp,
    [2] = handle_user_intrp,
    [3] = handle_user_intrp,
    [4] = handle_user_intrp,
    [5] = handle_user_intrp,
    [6] = handle_user_intrp,
    [7] = handle_user_timer,
    [8] = handle_user_intrp,
    [9] = handle_user_intrp,
    [10] = handle_user_intrp,
    [11] = handle_user_intrp
};

static const handler_t excpt_handlers[12] = {
    [0] = handle_user_excpt,
    [1] = handle_user_excpt,
    [2] = handle_illegal_instruction,
    [3] = handle_user_excpt,
    [4] = handle_user_excpt,
    [5] = handle_user_excpt,
    [6] = handle_user_excpt,
    [7] = handle_user_excpt,
    [8] = handle_ecall,
    [9] = handle_user_excpt,
    [10] = handle_user_excpt,
    [11] = handle_user_excpt
};

void handle_event(uintptr_t mcause, uintptr_t mtval)
{
    if ((intptr_t)mcause < 0) {
        intrp_handlers[mcause & ~INTRP_BIT](mcause, mtval);
    } else {
        excpt_handlers[mcause](mcause, mtval);
    }
    sched();
    restore_context();
}

void handle_user_timer(uintptr_t mcause, uintptr_t mtval)
{
#if QUANTUM
    if ((CURRENT.ustatus & USTATUS_IE) && CURRENT.deadline && CURRENT.deadline < KERNEL.deadline) {
        // user timer must have trigger interrupt
        handle_user_excpt(UMODE_SOFT_TIMER, 0);
        CURRENT.deadline = 0;
    }
#else
    handle_user_exception(UMODE_SOFT_TIMER, 0);
    CURRENT.deadline = 0;
#endif
}

void handle_user_intrp(uintptr_t mcause, uintptr_t mtval)
{
    intrp_handler_t intrp_handler = INTRP_HANDLERS[mcause & ~INTRP_BIT];
    zone_t* zone = intrp_handler.zone;
    zone->uepc = zone->regs[PC];
    zone->ucause = mcause;
    zone->utval = 0;
    zone->regs[PC] = intrp_handler.handler;
    zone->ustatus = USTATUS_PIE;
    KERNEL.mie &= ~zone->uie;
    sched_intrp(zone);
}

void handle_user_excpt(uintptr_t mcause, uintptr_t mtval)
{
    zone_t* zone = KERNEL.current;
    uintptr_t handler = zone->trap_handlers[mcause];
    zone->uepc = zone->regs[PC];
    zone->ucause = mcause;
    zone->utval = mtval;
    zone->regs[PC] = handler;
    if (zone->ustatus & USTATUS_IE) {
        zone->ustatus = USTATUS_PIE;
        KERNEL.mie &= ~zone->uie;
    }
}

static inline void handle_uret(void)
{
    /* restore trap values and pc */
    zone_t* zone = KERNEL.current;
    zone->regs[PC] = zone->uepc;
    zone->ucause = 0;
    zone->uepc = 0;
    zone->utval = 0;
    /* restore trap handling */
    if (zone->ustatus & USTATUS_PIE) {
        zone->ustatus |= USTATUS_IE;
    }
    if (KERNEL.intrp_mode) {
        KERNEL.deadline = read_mtime();
        sched_yield();
    }
}

void handle_illegal_instruction(uintptr_t mcause, uintptr_t mtval)
{
    // mtval is an opcode
    if (mtval == OP_URET) {
        handle_uret();
    } else {
        handle_user_excpt(mcause, mtval);
    }
    // TODO: add CSR instruction emulation
}
