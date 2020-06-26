/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.  
 */
#ifndef CONST_H_
#define CONST_H_

#define MIE_TIMER (1 << 7)

#define INTRP_BIT (1UL << (__riscv_xlen - 1))

#define UMODE_SOFT_INTRP 0
#define SMODE_SOFT_INTRP 1
#define MMODE_SOFT_INTRP 3
#define UMODE_TIMER_INTRP 4
#define SMODE_TIMER_INTRP 5
#define MMODE_TIMER_INTRP 7
#define UMODE_EXTERN_INTRP 8
#define SMODE_EXTERN_INTRP 9
#define MMODE_EXTERN_INTRP 11

#define INSTRUCTION_ADDRESS_MISALIGNED 0
#define ILLEGAL_ACCESS_FAULT 1
#define ILLEGAL_INSTRUCTION 2
#define BREAKPOINT 3
#define UMODE_SOFT_TIMER 3
#define LOAD_ADDRESS_MISALIGNED 4
#define LOAD_ACCESS_FAULT 5
#define STORE_ADDRESS_MISALIGNED 6
#define STORE_ACCESS_FAULT 7
#define UMODE_ECALL 8
#define SMODE_ECALL 9
#define MMODE_ECALL 11
#define INSTRUCTION_PAGE_FAULT 12
#define LOAD_PAGE_FAULT 13
#define STORE_PAGE_FAULT 15

#define OP_URET 0x00200073UL
#endif // CONST_H_
