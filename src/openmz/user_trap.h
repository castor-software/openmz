#ifndef USER_TRAP_H
#define USER_TRAP_H
#include "kernel.h"
void HandleUserTrap(uptr mcause, uptr mtval);
#endif /* USER_TRAP_H */
