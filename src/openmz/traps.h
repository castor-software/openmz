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
#include "kernel.h"
void HandleException(uptr mcause, uptr mtval);
void HandleInterrupt(uptr mcause, uptr mtval);
#endif /* TRAPS_H */
