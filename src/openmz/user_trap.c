/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.  
 */
#include "user_trap.h"

void HandleUserTrap(uptr mcause, uptr mtval)
{
    Zone* zone;
    uptr handler;
    if ((sptr)mcause < 0) {
        zone = IRQ_HANDLERS[0xFFF & mcause].zone;
        handler = IRQ_HANDLERS[0xFFF & mcause].handler;
        HART.event = EVENT_INTERRUPT;
    } else {
        zone = HART.current;
        handler = zone->trap_handlers[mcause];
    }
    zone->uepc = zone->regs[PC];
    zone->ucause = mcause;
    zone->utval = mtval;
    zone->regs[PC] = handler;
    if (zone->ustatus & USTATUS_IE) {
        zone->ustatus = USTATUS_PIE;
    }
}
