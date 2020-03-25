/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#ifndef CONST_H
#define CONST_H

// XLEN is the length of a register in bits
#define REGBITS __riscv_xlen
// XBYTES is length of register in bytes
#define REGBYTES (__riscv_xlen / 8)

#define RV32 (__riscv_xlen == 32)
#define RV64 (__riscv_xlen == 64)

/* selection macro */
#if RV32
#define SEL(a, b) a
#else
#define SEL(a, b) b
#endif

#define ARRAY_LEN(x) (sizeof(x) / sizeof(x[0]))
#define UNUSED(x) x __attribute__((unused))

#define XSTR(s) STR(s)
#define STR(s) #s

#define REG_S SEL(sw, sd) // register store
#define REG_L SEL(lw, ld) // register load

// Register index
#define PC 0
#define RA 1
#define SP 2
#define GP 3
#define TP 4
#define T0 5
#define T1 6
#define T2 7
#define FP 8
#define S0 8
#define S1 9
#define A0 10
#define A1 11
#define A2 12
#define A3 13
#define A4 14
#define A5 15
#define A6 16
#define A7 17
#define S2 18
#define S3 19
#define S4 20
#define S5 21
#define S6 22
#define S7 23
#define S8 24
#define S9 25
#define S10 26
#define S11 27
#define T3 28
#define T4 29
#define T5 30
#define T6 31
#endif /* CONST_H */
