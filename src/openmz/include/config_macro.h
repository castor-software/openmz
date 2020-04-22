/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to
 * the terms of the GNU General Public License version 2.
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.  
 */
#ifndef CONFIG_MACRO_H
#define CONFIG_MACRO_H
/* Macros used for zone configuration */
#define XZONE(pc,                                                             \
    cfg0, addr0, cfg1, addr1, cfg2, addr2, cfg3, addr3,                       \
    cfg4, addr4, cfg5, addr5, cfg6, addr6, cfg7, addr7)                       \
    {                                                                         \
        .regs[PC] = pc,                                                       \
        .pmpcfg = {                                                           \
            cfg0 | cfg1 << 8 | cfg2 << 16 | cfg3 << 24,                       \
            cfg4 | cfg5 << 8 | cfg6 << 16 | cfg7 << 24 },                     \
        .pmpaddr = { addr0, addr1, addr2, addr3, addr4, addr5, addr6, addr7 } \
    },

#define ZONE_8(pc, pmp0, pmp1, pmp2, pmp3, pmp4, pmp5, pmp6, pmp7) \
    XZONE(pc, pmp0, pmp1, pmp2, pmp3, pmp4, pmp5, pmp6, pmp7)
#define ZONE_7(pc, pmp0, pmp1, pmp2, pmp3, pmp4, pmp5, pmp6) \
    XZONE(pc, pmp0, pmp1, pmp2, pmp3, pmp4, pmp5, pmp6, 0, 0)
#define ZONE_6(pc, pmp0, pmp1, pmp2, pmp3, pmp4, pmp5) \
    XZONE(pc, pmp0, pmp1, pmp2, pmp3, pmp4, pmp5, 0, 0, 0, 0)
#define ZONE_5(pc, pmp0, pmp1, pmp2, pmp3, pmp4) \
    XZONE(pc, pmp0, pmp1, pmp2, pmp3, pmp4, 0, 0, 0, 0, 0, 0)
#define ZONE_4(pc, pmp0, pmp1, pmp2, pmp3) \
    XZONE(pc, pmp0, pmp1, pmp2, pmp3, 0, 0, 0, 0, 0, 0, 0, 0)
#define ZONE_3(pc, pmp0, pmp1, pmp2) \
    XZONE(pc, pmp0, pmp1, pmp2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
#define ZONE_2(pc, pmp0, pmp1) \
    XZONE(pc, pmp0, pmp1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)

#define IRQ(zone_id, irq_num) \
    [irq_num]                 \
        = { .zone = &kernel.zones[zone_id - 1], .handler = 0 },

#define W 2
#define RW 3
#define X 4
#define RX 5
#define WX 6
#define RWX 7

#define TOR(rwx, addr) \
    (0x8 | rwx), (addr >> 2)
#define NA4(rwx, addr) \
    (0x10 | rwx), (addr >> 2)
#define NAPOT(rwx, addr, size) \
    (0x18 | rwx), (((addr >> 2) | ((1 << (size - 2)) - 1)) & ~(1 << (size - 2)))
#endif // CONFIG_MACRO_H
