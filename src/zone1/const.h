/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#ifndef CONST_H
#define CONST_H

/* selection macro */
#if __riscv_xlen == 32
#define RV32 1
#define SEL(a, b) a
#else
#define RV64 1
#define SEL(a, b) b
#endif

#define XLEN SEL(32, 64)
#define XBYTES SEL(4, 8)

#endif /* CONST_H */
