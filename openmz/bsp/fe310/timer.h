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

#include <stdint.h>
#define MTIME 0x200bff8
#define MTIMECMP 0x2004000

#define REG32(val) (*((volatile uint32_t*)(val)))
#define REG64(val) (*((volatile uint64_t*)(val)))

static inline uint64_t read_mtime(void)
{
    volatile register uint32_t lo, hi1, hi2;
    do {
        hi1 = REG32(MTIME + 4);
        lo = REG32(MTIME);
        hi2 = REG32(MTIME + 4);
    } while (hi1 != hi2);
    return (uint64_t)hi1 << 32 | lo;
}

static inline void write_mtimecmp(uint64_t mtimecmp)
{
    REG32(MTIMECMP) = -1;
    REG32(MTIMECMP + 4) = mtimecmp >> 32;
    REG32(MTIMECMP) = mtimecmp;
}

static inline uint64_t read_mtimecmp(void)
{
    return REG64(MTIMECMP);
}
#endif /* TIMER_H */
