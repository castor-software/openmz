/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#ifndef ASM_H
#define ASM_H

#include "const.h"

#define REG_S SEL(sw, sd)  // register store
#define REG_L SEL(lw, ld)  // register load

#define ENTRY(name) \
    .globl name;    \
    .align 4;       \
    name:

#endif /* ASM_H */
