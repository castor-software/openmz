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

#define MAKE_READ64(reg)                \
    static inline u64 Read64##reg(void) \
    {                                   \
        uptr res;                       \
        CSRR(res, reg);                 \
        return res;                     \
    }

#ifdef RV32
#define MAKE_READ64_COUNTER(reg)            \
    static inline u64 Read64##reg(void)     \
    {                                       \
        register volatile u32 lo, hi1, hi2; \
        do {                                \
            CSRR(hi1, reg##h);              \
            CSRR(lo, reg);                  \
            CSRR(hi2, reg##h);              \
        } while (hi1 != hi2);               \
        return ((u64)hi1 << 32) | lo;       \
    }
#else
#define MAKE_READ64_COUNTER(reg)        \
    static inline u64 Read64##reg(void) \
    {                                   \
        register volatile u64 res;      \
        CSRR(res, reg);                 \
        return res;                     \
    }
#endif

MAKE_READ64(misa)
MAKE_READ64(mvendorid)
MAKE_READ64(marchid)
MAKE_READ64(mimpid)
MAKE_READ64(mhartid)
MAKE_READ64_COUNTER(mcycle)
MAKE_READ64_COUNTER(minstret)
MAKE_READ64_COUNTER(mhpmcounter3)
MAKE_READ64_COUNTER(mhpmcounter4)

#endif /* CSR_H */
