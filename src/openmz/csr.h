/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#ifndef CSR_H
#define CSR_H
#include "types.h"

#define CSRW(reg, in) __asm__ volatile("csrw " #reg ", %0" ::"r"(in))
#define CSRR(out, reg) __asm__ volatile("csrr %0, " #reg \
                                        : "=r"(out))

#ifdef RV32
/* x5 = t0, x6 = t1, x7 = x2 */
#define CSRR64(out, reg)                            \
    __asm__(                                        \
        "csrr   x5, " #reg ";"                      \
        "sw     x5, 0(%[outp]);"                    \
        "sw     x0, 4(%[outp]);" ::[outp] "r"(&out) \
        : "x5");
#define CSRR_COUNTER(out, reg)                       \
    __asm__(                                         \
        "1: csrr x5, " #reg "h;"                     \
        "   csrr x6, " #reg ";"                      \
        "   csrr x7, " #reg "h;"                     \
        "   bne  x5, x7, 1b;"                        \
        "   sw   x6, 0(%[outp]);"                    \
        "   sw   x5, 4(%[outp]);" ::[outp] "r"(&out) \
        : "x5", "x6", "x7");
#else
#define CSRR64(out, reg) CSRR(out, reg)
#define CSR_COUNTER(out, reg) CSRR(out, reg)
#endif

#endif /* CSR_H */
