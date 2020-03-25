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
