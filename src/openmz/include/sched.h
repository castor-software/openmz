/**
 * Copyright 2020, Saab AB
 *
 * This software may be distributed and modified according to 
 * the terms of the GNU General Public License version 2. 
 * Note that NO WARRANTY is provided.
 * See "LICENSE.GPLv2" for details.
 */
#ifndef SCHED_H
#define SCHED_H
#include "kernel.h"
void sched_init(void);
void sched_yield(void);
void sched(void);
void sched_intrp(zone_t* zone);
#endif // SCHED_H
