/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
/* ECALL numbers */
#define ECALL_YIELD 0
#define ECALL_WFI 1
#define ECALL_SEND 2
#define ECALL_RECV 3
#define ECALL_TRP_VECT 4
#define ECALL_IRQ_VECT 5
#define ECALL_CSRS_MIE 6
#define ECALL_CSRC_MIE 7
#define ECALL_CSRW_MTIMECMP 8
#define ECALL_CSRR_MTIME 9
#define ECALL_CSRR_MCYCLE 10
#define ECALL_CSRR_MINSTR 11
#define ECALL_CSRR_MHPMC3 12
#define ECALL_CSRR_MHPMC4 13
#define ECALL_CSRR_MISA 14
#define ECALL_CSRR_MVENDID 15
#define ECALL_CSRR_MARCHID 16
#define ECALL_CSRR_MIMPID 17
#define ECALL_CSRR_MHARTID 18

static void EcallYield(void)
{
    LoadNextZone();
}

static void EcallWfi(void)
{
    /* we have not implemented WFI, do a yield instead */
    LoadNextZone();
}

static void EcallSend(void)
{
    uptr receiver = CURRENT.regs[A0] - 1;
    /* check bound */
    if (receiver >= N_ZONES)
        return;
    /* check if inbox is full */
    Inbox* inbox = ZONES[receiver].inboxes + (CURRENT.id - 1);
    if (inbox->full)
        return;
    /* if empty, put message */
    inbox->msgs[0] = CURRENT.regs[A1];
    inbox->msgs[1] = CURRENT.regs[A2];
#ifdef RV32
    inbox->msgs[2] = CURRENT.regs[A3];
    inbox->msgs[3] = CURRENT.regs[A4];
#endif
    /* inbox is now full */
    CURRENT.regs[A0] = 1;
}

static void EcallRecv(void)
{
    uptr sender = CURRENT.regs[A0] - 1;
    /* check bounds */
    if (sender >= N_ZONES)
        return;
    /* check if inbox is full */
    Inbox* inbox = CURRENT.inboxes + sender;
    if (!inbox->full)
        return;
    /* if full, get message */
    CURRENT.regs[A1] = inbox->msgs[0];
    CURRENT.regs[A2] = inbox->msgs[1];
#ifdef RV32
    CURRENT.regs[A3] = inbox->msgs[2];
    CURRENT.regs[A4] = inbox->msgs[3];
#endif
    /* inbox is now empty */
    CURRENT.regs[A0] = 0;
}

static void EcallTrpVect(void)
{
    uptr trap_num = CURRENT.regs[A0];
    uptr handler = CURRENT.regs[A1];
    if (trap_num >= N_TRAPS)
        return;
    CURRENT.trap_handlers[trap_num] = handler;
    /* if it is mtimecmp emulation, reset timecmp */
    if (trap_num == 3)
        CURRENT.timecmp = 0;
}

static void EcallIrqVect(void)
{
    uptr irq_num = CURRENT.regs[A0];
    uptr handler = CURRENT.regs[A1];
    /* check bounds */
    if (irq_num >= N_INTERRUPTS)
        return;
    /* check irq->zone mapping */
    IrqHandler* irq_handler = &KERNEL(irq_handlers)[irq_num];
    if (irq_handler->zone != &CURRENT)
        return;
    /* if it is the correct zone, put the handler */
    irq_handler->handler = handler;
    /* check if we should activate or deactivate interrupt */
    if (handler) {
        /* if the handler is non-zero */
        /* activate the interrupt */
        CURRENT.uie |= (1 << irq_num);
        KERNEL(mie) |= CURRENT.uie * (CURRENT.ustatus & 1);
    } else {
        /* handler == 0 ==> deactivate the interrupt */
        CURRENT.uie &= ~(1 << irq_num);
        KERNEL(mie) &= ~CURRENT.uie;
    }
}

static void EcallCsrsMie(void)
{
    CURRENT.ustatus |= 1;
    KERNEL(mie) |= CURRENT.uie;
}

static void EcallCsrcMie(void)
{
    CURRENT.ustatus &= ~1;
    KERNEL(mie) &= ~CURRENT.uie;
}

static void EcallCsrwMtimecmp(void)
{
    CURRENT.timecmp = CAST64(CURRENT.regs[A0]);
}

static void EcallCsrrMtime(void)
{
    CAST64(CURRENT.regs[A0]) = ReadMtime();
}

static void EcallCsrrMcycle(void)
{
    CSRR_COUNTER(CURRENT.regs[A0], mcycle);
}

static void EcallCsrrMinstr(void)
{
    CSRR_COUNTER(CURRENT.regs[A0], minstret);
}

static void EcallCsrrMhpmc3(void)
{
    CSRR_COUNTER(CURRENT.regs[A0], mhpmcounter3);
}

static void EcallCsrrMhpmc4(void)
{
    CSRR_COUNTER(CURRENT.regs[A0], mhpmcounter4);
}

static void EcallCsrrMisa(void)
{
    CSRR64(CURRENT.regs[A0], misa);
}

static void EcallCsrrMvendid(void)
{
    CSRR64(CURRENT.regs[A0], mvendorid);
}

static void EcallCsrrMarchid(void)
{
    CSRR64(CURRENT.regs[A0], marchid);
}

static void EcallCsrrMimpid(void)
{
    CSRR64(CURRENT.regs[A0], mimpid);
}

static void EcallCsrrMhartid(void)
{
    CSRR64(CURRENT.regs[A0], mhartid);
}
