/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the MIT License.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.MIT" for details.
 */
#ifndef CONFIG_H
#define CONFIG_H
#include "config_macro.h"

// Number of zones
#define N_ZONES 2

// max trap code + 1
#define N_TRAPS 12

// max interrupt code + 1
#define N_INTERRUPTS 12

// time slice in ms
#define QUANTUM 100

/*
 * ZONE_N has N PMP regions. ZONE_N is defined for N = 2, 3, ..., 8.
 * ZONE_N's first argument is the program counter, the following 
 * arguments are the PMP configurations.
 *
 * Example:
 * ZONE_2(0x20020000, NAPOT(RX, 0x20020000, 13), NAPOT(RW, 0x80001000, 12))
 *      pc: 0x20020000
 *      region 1: read/execute, Base address 0x20020000, size 2^13
 *      region 2: read/execute, Base address 0x80001000, size 2^12
 *
 * PMP configurations are:
 * - TOR(Permissions, address)
 * - NA4(Permissions, base address)
 * - NAPOT(Permissions, base address, log_2(size))
 *      The base address should be 2^size aligned.
 *      
 * For all addresses, the two least significant bits will be zero since
 * PMP has the minimum granularity of 4 bytes.
 */
#define ZONE_CONFIG                \
    ZONE_4(0x20100000,             \
        NAPOT(RX, 0x20100000, 20), \
        NAPOT(RW, 0x80001000, 12), \
        NAPOT(RW, 0x10012000, 12), \
        NAPOT(RW, 0x10013000, 12)) \
    ZONE_2(0x20200000,             \
        NAPOT(RX, 0x20200000, 20), \
        NAPOT(RW, 0x80002000, 12))

/*
 * Here we register hardware interrupts. 
 * IRQ(Zone ID, Interrupt code)
 */
#define IRQ_CONFIG IRQ(1, 11)
#endif //  CONFIG_H
