/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */

#include "illegal_instruction.h"
#include "user_trap.h"

#define URET_CODE 0x00200073UL
static inline void HandleUret(void)
{
    /* restore trap values and pc */
    Zone* zone = HART.current;
    zone->regs[PC] = zone->uepc;
    zone->ucause = 0;
    zone->uepc = 0;
    zone->utval = 0;
    /* restore trap handling */
    if (zone->ustatus & USTATUS_PIE)
        zone->ustatus |= USTATUS_IE;
    HART.event = EVENT_URET;
}

void HandleIllegalInstruction(uptr mcause, uptr mtval)
{
    if (mtval == URET_CODE) {
        HandleUret();
    } else {
        HandleUserTrap(mcause, mtval);
    }
    // TODO: add CSR instruction emulation
}
