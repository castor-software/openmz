#include "scheduler.h"
#include "timer.h"
#define MIE_TIMER (1 << 7)
#define SCHED_RR 0
#define SCHED_IRQ 1

static void NextZoneRR(void)
{
    HART.current = HART.next;
    HART.deadline = GetTime() + HART.next_quantum;
    HART.next = (CURRENT.id == N_ZONES) ? &ZONES[0] : (HART.current + 1);
    HART.next_quantum = QUANTUM;
    HART.state = SCHED_RR;
}

static void ScheduleInterrupt()
{
    HART.next = HART.current;
    s64 time_remaining = HART.deadline - GetTime();
    HART.next_quantum = time_remaining > 0 ? time_remaining : 0;
    uptr mcause;
    CSRR(mcause, mcause);
    HART.current = IRQ_HANDLERS[mcause & 0xFFF].zone;
    HART.deadline = GetTime() + QUANTUM;
    HART.state = SCHED_IRQ;
}

void Scheduler(void)
{
    switch (HART.event) {
    case EVENT_URET:
        if (HART.state == SCHED_IRQ)
            break;
    case EVENT_YIELD:
    case EVENT_WFI:
        NextZoneRR();
        break;
    case EVENT_INTERRUPT:
        ScheduleInterrupt();
        break;
    }
    HART.event = 0;
    while (GetTime() >= HART.deadline) {
        NextZoneRR();
    }
    if ((CURRENT.ustatus & USTATUS_IE)
        && CURRENT.deadline && (CURRENT.deadline < HART.deadline)) {
        // if zone's deadline is earlier, use it for timer interrupt
        SetDeadline(CURRENT.deadline);
    } else {
        SetDeadline(HART.deadline);
    }
    uptr mie = MIE_TIMER;
    if (HART.state != SCHED_IRQ) {
        for (int i = 0; i < N_ZONES; ++i) {
            if (ZONES[i].ustatus & USTATUS_IE) {
                mie |= ZONES[i].uie;
            }
        }
    }
    CSRW(mie, mie);
}

void InitScheduler(void)
{
    HART.next = ZONES;
    HART.next_quantum = QUANTUM;
    HART.state = SCHED_RR;
    NextZoneRR();
    SetDeadline(HART.deadline);
    CSRW(mie, MIE_TIMER);
}
