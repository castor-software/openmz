/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to 
 * the terms of the GNU General Public License version 2. 
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#ifndef MACHINE_H
#define MACHINE_H
#include "csr.h"
#include <platform.h>

static inline uint64_t read_mtime(void)
{
    register uint32_t lo, hi1, hi2;
    do {
        hi1 = CLIC.mtimeh;
        lo = CLIC.mtime;
        hi2 = CLIC.mtimeh;
    } while (hi1 != hi2);
    return (uint64_t)hi1 << 32 | lo;
}

static inline void write_mtimecmp(uint64_t val)
{
    CLIC.mtimecmp = -1;
    CLIC.mtimecmph = val >> 32;
    CLIC.mtimecmp = val;
}

static inline void clear_cache(void)
{
    // no cache clearing
}

static inline void init_hardware(void)
{
    // Sets core clock frequency to 16 MHz
    uint32_t pllr = 0x1;
    uint32_t pllf = 0x2F;
    uint32_t pllq = 0x2;
    uint32_t pllsel = 0x1;
    uint32_t pllrefsel = 0x1;
    uint32_t pllbypass = 0x0;
    PRCI.pllcfg = pllr | (pllf << 4) | (pllq << 10) | (pllsel << 16) | (pllrefsel << 17) | (pllbypass << 18);
    while (!(PRCI.pllcfg >> 31))
        ; // wait until configuration is locked

    CSRW(mcounteren, 0x1F);
    CSRW(mhpmevent3, 0x0102);
}

#endif // MACHINE_H
