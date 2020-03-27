/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.  
 */
#include "user_timer.h"
#include "timer.h"
#include "user_trap.h"

void HandleUserTimer(void)
{
    if ((CURRENT.ustatus & USTATUS_IE)
        && CURRENT.deadline && (CURRENT.deadline <= GetTime())) {
        HandleUserTrap(0x3, 0);
    }
}
