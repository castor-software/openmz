/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to 
 * the terms of the GNU General Public License version 2. 
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#include "libhexfive.h"
#include <platform.h>
#include <stdint.h>
#include <stdio.h>
#define RR 1
uint32_t vec32[4] = { 0, 0, 0, 0 };
uint64_t vec64[2] = { 0, 0 };
uint64_t prev_cache_misses;

void init_uart(void)
{
    UART0.div = 1666;
    UART0.txctrl = 0x1;
    UART0.rxctrl = 0x1;
    GPIO.iof_sel &= ~(3 << 16);
    GPIO.iof_en |= (3 << 16);
}

extern void asm_adder(int a0) __attribute__((noreturn));

void print_add_loop(uint32_t a0)
{
    uint64_t cache_misses = ECALL_CSRR_MHPMC3();
    if (a0 != 0) {
#if RR
        while (!ECALL_RECV(2, vec32))
            ECALL_YIELD();
        // get the count of the other zone
        a0 += vec32[0];
#endif
        printf("%u %u\n\r", a0, cache_misses - prev_cache_misses);
    }
    prev_cache_misses = cache_misses;
    // set this as timer handler
    ECALL_TRP_VECT(0x3, print_add_loop);
    // enable interrupts
    ECALL_CSRS_MIE();
    // calculate deadline
    uint64_t time = ECALL_CSRR_MTIME();
    uint64_t start_time = time + RTCK_FREQ / 2; // start in half a second
    uint64_t deadline = start_time + 10 * RTCK_FREQ; // run for 10 seconds
    // send start and deadline to zone 2
    vec64[0] = start_time;
    vec64[1] = deadline;
#if RR
    while (!ECALL_SEND(2, vec64))
        ECALL_YIELD();
#endif
    // set deadline
    ECALL_CSRW_MTIMECMP(deadline);
    // wait for start time
    while (start_time > ECALL_CSRR_MTIME())
        ;
    // start adding
    asm_adder(0);
}

void main(void)
{
    init_uart();
    printf("Test started\n\r");
    print_add_loop(0);
}
