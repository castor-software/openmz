/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to 
 * the terms of the GNU General Public License version 2. 
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#include "libhexfive.h"
#include "platform.h"

uint32_t vec32[4] = { 0, 0, 0, 0 };
uint64_t vec64[2] = { 0, 0 };

extern void asm_adder(uint32_t a0) __attribute__((noreturn));
void send_add_loop(uint32_t a0)
{
    if (a0 != 0) {
        // send additions to zone 1
        vec32[0] = a0;
        while (!ECALL_SEND(1, vec32))
            ECALL_YIELD();
    }
    // enable interrupts
    ECALL_CSRS_MIE();
    // get start and deadline
    while (!ECALL_RECV(1, vec64))
        ECALL_YIELD();
    uint64_t start_time = vec64[0];
    uint64_t deadline = vec64[1];
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
    ECALL_TRP_VECT(0x3, send_add_loop);
    send_add_loop(0);
}
