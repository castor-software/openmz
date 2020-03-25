#ifndef TRAPS_H
#define TRAPS_H
#include "kernel.h"
void HandleException(uptr mcause, uptr mtval);
void HandleInterrupt(uptr mcause, uptr mtval);
#endif /* TRAPS_H */
