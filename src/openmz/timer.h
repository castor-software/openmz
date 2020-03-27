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

// address of mtime and mtimecmp are defined in memory.lds
#if REGBITS == 32
extern volatile u32 mtime, mtimeh;
extern volatile u32 mtimecmp, mtimecmph;
#else
extern volatile u64 mtime;
extern volatile u64 mtimecmp;
#endif

static inline u64 GetTime(void)
{
#if REGBITS == 32
    register u32 lo, hi;
    do {
        hi = mtimeh;
        lo = mtime;
    } while (hi != mtimeh);
    return ((u64)hi << 32) | lo;
#else
    return mtime;
#endif
}

static inline void SetDeadline(u64 deadline)
{
#if REGBITS == 32
    mtimecmp = -1;
    mtimecmph = (u32)(deadline >> 32);
    mtimecmp = (u32)deadline;
#else
    mtimecmp = deadline;
#endif
}
#endif /* TIMER_H */
