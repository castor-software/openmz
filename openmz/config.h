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

/* do not touch */
#define ZONE(pc, cfg0, addr) { .regs[PC] = pc, .pmpcfg0 = cfg0, .pmpaddr = addr },
#define IRQ(zone_id, irq_num) \
    [irq_num] = { .zone = &kernel.zones[zone_id - 1], .handler = 0 },
/**/

/* this is our example configuration */
/* memory-mapped addresses of mtime and mtimecmp */
#define MTIME 0x200bff8
#define MTIMECMP 0x2004000

/* number of zones */
#define N_ZONES 3
/* max trap code */
#define N_TRAPS 12
/* max interrupt code */
#define N_INTERRUPTS 12
/* time slice in ticks */
#define QUANTUM (32768 / 100)

#define ZONE_CONFIG               \
    ZONE(0x20020000, 0xf, { -1 }) \
    ZONE(0x20030000, 0xf, { -1 }) \
    ZONE(0x20040000, 0xf, { -1 })

#define IRQ_CONFIG IRQ(1, 11)
/**/

#endif /* CONFIG_H */

