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

#define CSRW(reg, in) __asm__ volatile("csrw " #reg ", %0" ::"r"(in) \
                                       : "memory")
#define CSRR(out, reg) __asm__ volatile("csrr %0, " #reg \
                                        : "=r"(out))

#if __riscv_xlen == 32
/* x5, x5, x7 = t0, t1, t2 */
#define CSRR64(out, reg)                             \
    __asm__ volatile("csrr   x5, " #reg ";"          \
                     "sw     x5, 0(%0);"             \
                     "sw     x0, 4(%0);" ::"r"(&out) \
                     : "x5", "memory")
#define CSRR_COUNTER(out, reg)                       \
    __asm__ volatile("  csrr x5, " #reg "h;"         \
                     "  csrr x6, " #reg ";"          \
                     "  csrr x7, " #reg "h;"         \
                     "  beq  x5, x7, 1f;"            \
                     "  csrr x6, " #reg ";"          \
                     "1:sw   x6, 0(%0);"             \
                     "  sw   x7, 4(%0);" ::"r"(&out) \
                     : "x5", "x6", "x7", "memory")
#else
#define CSRR64(out, reg) CSRR(out, reg)
#define CSRR_COUNTER(out, reg) CSRR(out, reg)
#endif

#endif /* CSR_H */
