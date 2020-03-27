/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#ifndef ILLEGAL_INSTRUCTION_H
#define ILLEGAL_INSTRUCTION_H
#include "kernel.h"
void HandleIllegalInstruction(uptr mcause, uptr mtval);
#endif /* ILLEGAL_INSTRUCTION_H */
