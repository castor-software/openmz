/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to 
 * the terms of the GNU General Public License version 2. 
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */

#define ENTRY(x) \
.globl x; \
x:

.section .text.libhexfive
ENTRY(ECALL_YIELD)
        li      a7, 0           # sys call #0
        ecall
        ret

ENTRY(ECALL_WFI)
        li      a7, 1           # sys call #1
        ecall
        ret

ENTRY(ECALL_SEND)
#if __riscv_xlen == 32 
        lw      a4, 12(a1)      # load msg[3]
        lw      a3, 8(a1)       # load msg[2]
        lw      a2, 4(a1)       # load msg[1]
        lw      a1, 0(a1)       # load msg[0]
#else
        ld      a2, 8(a1)       # load msg[2-3]
        ld      a1, 0(a1)       # load msg[0-1]
#endif
        li      a7, 2           # sys call #2
        ecall
        ret

ENTRY(ECALL_RECV)
        mv      a6, a1          # save pointer
        li      a7, 3           # sys call #3
        ecall
        beqz    a0, 1f          # status == 0, go to ret
#if __riscv_xlen == 32 
        sw      a4, 12(a6)      # save msg[3]
        sw      a3, 8(a6)       # save msg[2]
        sw      a2, 4(a6)       # save msg[1]
        sw      a1, 0(a6)       # save msg[0]
#else
        sd      a2, 8(a6)       # save msg[2-3]
        sd      a1, 0(a6)       # save msg[0-1]
#endif
1:      ret

ENTRY(ECALL_TRP_VECT)
        li      a7, 4           # sys call #4
        ecall
        ret

ENTRY(ECALL_IRQ_VECT)
        li      a7, 5           # sys call #5
        ecall
        ret

ENTRY(ECALL_CSRS_MIE)
        li      a7, 6           # sys call #6
        ecall
        ret

ENTRY(ECALL_CSRC_MIE)
        li      a7, 7           # sys call #7
        ecall
        ret

ENTRY(ECALL_CSRW_MTIMECMP)
        li      a7, 8           # sys call #8
        ecall
        ret

ENTRY(ECALL_CSRR_MTIME)
        li      a7, 9           # sys call #9
        ecall
        ret

ENTRY(ECALL_CSRR_MCYCLE)
        li      a7, 10          # sys call #10
        ecall
        ret

ENTRY(ECALL_CSRR_MINSTR)
        li      a7, 11          # sys call #11
        ecall
        ret

ENTRY(ECALL_CSRR_MHPMC3)
        li      a7, 12          # sys call #12
        ecall
        ret

ENTRY(ECALL_CSRR_MHPMC4)
        li      a7, 13          # sys call #13
        ecall
        ret


ENTRY(ECALL_CSRR_MISA)
        li      a7, 14          # sys call #14
        ecall
        ret

ENTRY(ECALL_CSRR_MVENDID)
        li      a7, 15          # sys call #15
        ecall
        ret

ENTRY(ECALL_CSRR_MARCHID)
        li      a7, 16          # sys call #16
        ecall
        ret

ENTRY(ECALL_CSRR_MIMPID)
        li      a7, 17          # sys call #17
        ecall
        ret

ENTRY(ECALL_CSRR_MHARTID)
        li      a7, 18          # sys call #18
        ecall
        ret
