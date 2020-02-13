# OpenMZ 
OpenMZ is an open-source implementation of [HexFive's MultiZone API](https://github.com/hex-five/multizone-api). 

OpenMZ is a security kernel for RISC-V processors with machine-mode and user-mode. OpenMZ partitions a system into an unlimited number of zones. Each zone has full access to eight regions of memory-mapped I/O (e.g., RAM, UART, GPIO), protected using RISC-V's physical memory protection (PMP) mechanism.

## Features
- Preemptive round-robin scheduler with a configurable time slices.
- Self-contained and open-source, written in C and assembly.
- Theoretically unlimited number of *spatially* isolated zones.
- Up to eight memory-mapped I/O regions per zone with protections defined by you.
- Support for user-mode platform-level and core-local interrupts.
- Support for user-mode exception handling.
- A secure user-mode timer interrupts emulation.
- Fast register-based inter-zone communication.

## Attacker model

## Kernel configuration
The kernel's configurations is in [config.h](openmz/config.h). The PMP fields `pmpcfg0` and `pmpaddr[0-8]` are configured as in RISC-V's PMP registers `pmpcfg0` and `pmpaddr0-7`. The initial program counter is stored in `regs[0]`. Interrupt mappings are stored in the `irq_handlers` field. All of these configurations options are set in `config.h`.
```C
/* ... */
/* number of zone */
#define N_ZONES 3 

/* max trap code */
#define N_TRAPS 12

/* max interrupt code */
#define N_INTERRUPTS 12

/* time slice in timer cycles */
#define QUANTUM (32768 / 100)

/* Zone configuration */
/* ZONE(program counter, pmpcfg0, {pmpaddr0, ..., pmpaddr7}) */
#define ZONE_CONFIG             \
    ZONE(0x20020000, ..., ...) \
    ZONE(0x20030000, ..., ...) \
    ZONE(0x20040000, ..., ...)

/* Mapping interrupts to zones */
/* IRQ(zone id, interrupt exception code) */
#define IRQ_CONFIG IRQ(1, 11) IRQ(2, 18)
/* ... */
```

## (Potential) Future Work
- Complete formal verification - from the hardware level to source code.
- Asynchronous user-mode timer interrupts.
- `YIELD_TO` call - allowing zones to yield their remaining time slice to a specific zone.
- An optional *temporal* isolation of zones by disabling `YIELD/WFI` calls, and immediate context switch for interrupts.
- More advanced round-robin scheduling with more configurable zone order and time slices.
- Compatibility with a secure bootloader, or a new secure bootloader.
- System call for supporting remote attestation.
- RISC-V user-interrupt extension (N extension) compatibility.
- Multicore support.
- Supervisor support.
- More memory-mapped I/O regions per zone.
- Inter-processor communication (IPI) - allowing communication with a Linux companion on a different core.

*The above list is mostly sorted by priority.*
## Example zone implementation

## Attribution

## Copyright

This project is under three different copyright licenses. HexFive's license for the MultiZone API (`libhexfive.h`), GPLv2 for the kernel and drivers, and MIT for the kernel configuration. The owner of the copyright of a source file is on the top.
