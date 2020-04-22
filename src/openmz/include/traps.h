/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.  
 */
#ifndef TRAPS_H
#define TRAPS_H
#include <stdint.h>
void handle_event(uintptr_t mcause, uintptr_t mtval);
void handle_ecall(uintptr_t, uintptr_t);
void handle_user_timer(uintptr_t, uintptr_t);
void handle_illegal_instruction(uintptr_t, uintptr_t);
void handle_user_intrp(uintptr_t, uintptr_t);
void handle_user_excpt(uintptr_t, uintptr_t);

#endif // TRAPS_H
