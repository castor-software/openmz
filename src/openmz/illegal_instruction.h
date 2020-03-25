#ifndef ILLEGAL_INSTRUCTION_H
#define ILLEGAL_INSTRUCTION_H
#include "kernel.h"
void HandleIllegalInstruction(uptr mcause, uptr mtval);
#endif /* ILLEGAL_INSTRUCTION_H */
