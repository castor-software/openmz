/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#include "kernel.h"

#define MMODE_IPI 3
#define MMODE_TIMER 7

#define ILLEGAL_INSTRUCTION 2
#define UMODE_ECALL 8

static void HandleIrq(void)
{
    /* save the current zone */
    KERNEL(next) = KERNEL(current);
    KERNEL(next_quantum) = read_mtimecmp() - read_mtime();
    /* find the interrupting zone, and set it to current */
    uptr exception_code = KERNEL(mcause) & 0xFFF;
    IrqHandler* irq_handler = &KERNEL(irq_handlers)[exception_code];
    KERNEL(current) = irq_handler->zone;
    /* disable zone's interrupts and save zone's interrupt enabled bit */
    KERNEL(mie) &= ~CURRENT.uie;
    CURRENT.ustatus <<= 4;
    /* save the trap values */
    CURRENT.ucause = KERNEL(mcause);
    CURRENT.uepc = CURRENT.regs[PC];
    CURRENT.utval = KERNEL(mtval);
    /* zone jump to handler */
    CURRENT.regs[PC] = irq_handler->handler;
    /* kernel's interrupt mode is enabled */
    KERNEL(is_isr) = 1;
    write_mtimecmp(read_mtime() + QUANTUM);
}

static inline void HandleInterrupt(void)
{
    switch (KERNEL(mcause) & 0xFFF) {
    case MMODE_IPI:
    case MMODE_TIMER:
        /* timer is handled in HandleEvent */
        break;
    default:
        HandleIrq();
    }
}

static void HandleException(void)
{
    /* disable zone's interrupts and save zone's interrupt enabled bit */
    KERNEL(mie) &= ~CURRENT.uie;
    CURRENT.ustatus <<= 4;
    /* load handler and set trap values */
    uptr handler = CURRENT.trap_handlers[KERNEL(mcause)];
    /* save the trap values */
    CURRENT.ucause = KERNEL(mcause);
    CURRENT.uepc = CURRENT.regs[PC];
    CURRENT.utval = KERNEL(mtval);
    /* zone jumps to handler */
    CURRENT.regs[PC] = handler;
}

static inline void HandleUret(void)
{
    /* restore trap values and pc */
    CURRENT.regs[PC] = CURRENT.uepc;
    CURRENT.ucause = 0;
    CURRENT.uepc = 0;
    CURRENT.utval = 0;
    /* restore trap handling */
    CURRENT.ustatus |= 0x1 & (CURRENT.ustatus >> 4);
    KERNEL(mie) |= CURRENT.uie * (CURRENT.ustatus & 1);
    /* if kernel is in interrupt mode,
     * load next zone and go to round robin mode */
    if (KERNEL(is_isr))
        LoadNextZone();
}

static void HandleIllegalInstruction(void)
{
    /* if zone did uret, do uret else illegal instruction exception */
#define URET_CODE 0x00200073UL
    if (KERNEL(mtval) == URET_CODE)
        HandleUret();
    else
        HandleException();
}

#include "ecall.c"

static void HandleEcall(void)
{
    /* ecall table */
    typedef void (*Handler)(void);
    static const Handler handlers[] = {
        [ECALL_YIELD] = EcallYield,
        [ECALL_WFI] = EcallWfi,
        [ECALL_SEND] = EcallSend,
        [ECALL_RECV] = EcallRecv,
        [ECALL_TRP_VECT] = EcallTrpVect,
        [ECALL_IRQ_VECT] = EcallIrqVect,
        [ECALL_CSRS_MIE] = EcallCsrsMie,
        [ECALL_CSRC_MIE] = EcallCsrcMie,
        [ECALL_CSRW_MTIMECMP] = EcallCsrwMtimecmp,
        [ECALL_CSRR_MTIME] = EcallCsrrMtime,
        [ECALL_CSRR_MCYCLE] = EcallCsrrMcycle,
        [ECALL_CSRR_MINSTR] = EcallCsrrMinstr,
        [ECALL_CSRR_MHPMC3] = EcallCsrrMhpmc3,
        [ECALL_CSRR_MHPMC4] = EcallCsrrMhpmc4,
        [ECALL_CSRR_MISA] = EcallCsrrMisa,
        [ECALL_CSRR_MVENDID] = EcallCsrrMvendid,
        [ECALL_CSRR_MARCHID] = EcallCsrrMarchid,
        [ECALL_CSRR_MIMPID] = EcallCsrrMimpid,
        [ECALL_CSRR_MHARTID] = EcallCsrrMhartid,
    };

    /* always advance PC! */
    CURRENT.regs[PC] += 4;
    uptr ecall = CURRENT.regs[A7];
    if (ecall < ARRAY_LEN(handlers))
        handlers[ecall]();
}

static inline void HandleTrap(void)
{
    switch (KERNEL(mcause)) {
    case UMODE_ECALL:
        HandleEcall();
        break;
    case ILLEGAL_INSTRUCTION:
        HandleIllegalInstruction();
        break;
    default:
        HandleException();
    }
}

void HandleEvent(void)
{
    /* if msb(mcause) == 1, the interrupt */
    if ((sptr)KERNEL(mcause) < 0)
        HandleInterrupt();
    else
        HandleTrap();
    /* check time out */
    while (read_mtime() >= read_mtimecmp())
        LoadNextZone();
    /* check user mode timecmp */
    if ((CURRENT.ustatus & 1) && CURRENT.timecmp) {
        if (CURRENT.timecmp <= read_mtime()) {
            CURRENT.timecmp = 0;
            KERNEL(mcause) = 3;
            KERNEL(mtval) = 0;
            HandleException();
        }
    }
}

