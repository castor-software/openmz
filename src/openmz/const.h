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
#define SEL(a, b) a
#define RV32 1
#else
#define RV64 1
#define SEL(a, b) b
#endif

#define XLEN SEL(32, 64)
#define XBYTES SEL(4, 8)

#define CAST64(x) (*((u64*)(&x)))

#define ARRAY_LEN(x) (sizeof(x) / sizeof(x[0]))
#endif /* CONST_H */
