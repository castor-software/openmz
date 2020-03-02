/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */

#ifndef TIMER_H
#define TIMER_H
#include "kernel.h"

#define REG32(val) (*((volatile u32*)(val)))
#define REG64(val) (*((volatile u64*)(val)))

#if RV32
static inline u64 ReadMtime(void)
{
    volatile register u32 lo, hi1, hi2;
    do {
        hi1 = REG32(MTIME + 4);
        lo = REG32(MTIME);
        hi2 = REG32(MTIME + 4);
    } while (hi1 != hi2);
    return (u64)hi1 << 32 | lo;
}

static inline void WriteMtime(u64 val)
{
    REG32(MTIME) = 0;
    REG32(MTIME + 4) = 0;
    REG32(MTIME) = 0;
}

static inline void WriteMtimecmp(u64 mtimecmp)
{
    REG32(MTIMECMP) = -1;
    REG32(MTIMECMP + 4) = mtimecmp >> 32;
    REG32(MTIMECMP) = mtimecmp;
}

static inline u64 ReadMtimecmp(void)
{
    return REG64(MTIMECMP);
}
#else
static inline u64 ReadMtime(void)
{
    return REG64(MTIME);
}
static inline void WriteMtime(u64 val)
{
    REG64(MTIME) = val;
}

static inline void WriteMtimecmp(u64 mtimecmp)
{
    REG64(MTIMECMP) = mtimecmp;
}

static inline u64 ReadMtimecmp(void)
{
    return REG64(MTIMECMP);
}
#endif
#endif /* TIMER_H */
