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

#define write_csr(reg, val) __asm__ volatile("csrw " #reg ", %0" ::"rK"(val))

#define read_csr(reg)                                     \
    ({                                                    \
        uptr __tmp;                                       \
        __asm__ volatile("csrr %0, " #reg : "=r"(__tmp)); \
        __tmp;                                            \
    })

#define swap_csr(reg, val)                                                   \
    ({                                                                       \
        uptr __tmp;                                                          \
        __asm__ volatile("csrr %0, " #reg ", %1" : "=r"(__tmp) : "rK"(val)); \
        __tmp;                                                               \
    })

#ifdef RV32
#define read_counter(reg)                         \
    ({                                            \
        register volatile u32 __lo, __hi1, __hi2; \
        do {                                      \
            __hi1 = read_csr(reg##h);             \
            __lo = read_csr(reg);                 \
            __hi2 = read_csr(reg##h);             \
        } while (__hi1 != __hi2);                 \
        ((u64)__hi1 << 32) | __lo;                \
    })
#define write_counter(reg, val)       \
    ({                                \
        write_csr(reg, 0);            \
        write_csr(reg##h, val >> 32); \
        write_csr(reg, val);          \
    })
#else
#define read_counter(reg) read_csr(reg)
#define write_counter(reg, val) write_csr(reg, val)
#endif

#endif /* CSR_H */
