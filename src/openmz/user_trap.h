/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.  
 */
#ifndef USER_TRAP_H
#define USER_TRAP_H
#include "kernel.h"
void HandleUserTrap(uptr mcause, uptr mtval);
#endif /* USER_TRAP_H */
