/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.  
 */
#include "sched.h"

#include "const.h"
#include "csr.h"
#include "kernel.h"
#include "machine.h"

#define QUANTUM_CYC (RTCK_FREQ * QUANTUM / 1000)

void sched_yield(void)
{
    KERNEL.intrp_mode = 0;
    KERNEL.current = KERNEL.next;
    KERNEL.next = (CURRENT.id == N_ZONES) ? &ZONES[0] : (KERNEL.current + 1);
#if QUANTUM > 0
    KERNEL.deadline = KERNEL.deadline + KERNEL.next_quantum;
    KERNEL.next_quantum = QUANTUM_CYC;
#endif
}

void sched_intrp(zone_t* zone)
{
    KERNEL.intrp_mode = 1;
    KERNEL.next = KERNEL.current;
    KERNEL.current = zone;
#if QUANTUM > 0
    uint64_t now = read_mtime();
    if (now < KERNEL.deadline) {
        KERNEL.next_quantum = KERNEL.deadline - now;
    } else {
        KERNEL.next_quantum = 0;
    }
    KERNEL.deadline = read_mtime() + QUANTUM_CYC;
#endif
}

void sched(void)
{
#if QUANTUM > 0
    if (KERNEL.deadline <= read_mtime()) {
        sched_yield();
    }
    if ((CURRENT.ustatus & USTATUS_IE) && CURRENT.deadline && (CURRENT.deadline < KERNEL.deadline)) {
        write_mtimecmp(CURRENT.deadline);
    } else {
        write_mtimecmp(KERNEL.deadline);
    }
    if (KERNEL.intrp_mode) {
        CSRW(mie, MIE_TIMER);
    } else {
        CSRW(mie, KERNEL.mie);
    }
#else
    uintptr_t mie = 0;
    if ((CURRENT.ustatus & USTATUS_IE) && CURRENT.deadline) {
        write_mtimecmp(CURRENT.deadline);
        mie |= MIE_TIMER;
    }
    if (!KERNEL.intrp_mode) {
        mie |= KERNEL.mie;
    }
    CSRW(mie, mie);
#endif
}

void sched_init(void)
{
    KERNEL.intrp_mode = 0;
    KERNEL.next = ZONES;
#if QUANTUM > 0
    KERNEL.mie = MIE_TIMER;
    KERNEL.next_quantum = QUANTUM_CYC;
#else
    KERNEL.mie = 0;
#endif
    sched_yield();
    sched();
}
