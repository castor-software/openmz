# OpenMZ 

OpenMZ is an open-source implementation of [HexFive's MultiZone API](https://github.com/hex-five/multizone-api). 

OpenMZ is a security kernel for RISC-V processors with machine-mode and user-mode. OpenMZ partitions a system into an unlimited number of *zones*. Each zone has full access to eight regions of memory-mapped I/O (e.g., RAM, UART, GPIO), protected using RISC-V's physical memory protection (PMP) mechanism.

## Copyright

This project is under three different copyright licenses. HexFive's license for their `libhexfive.h,` GPLv2 for the kernel and drivers, and MIT for the kernel configuration. The owner of the copyright of a source file is on the top.

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
*Coming soon.*

## Kernel configuration
*Coming soon.*

## (Potential) Future Work
- Complete formal verification - from the hardware level to source code.
- Asynchronous user-mode timer interrupts.
- `YIELD_TO` call - allowing zones to yield their remaining time slice to a specific zone.
- An optional *temporal* isolation of zones by disabling `YIELD/WFI` calls, and immediate context switch for interrupts.
- More advanced round-robin scheduling with more configurable zone order and time slices.
- Compatibility with a secure bootloader, or build a new secure bootloader.
- Add remote attestation.
- RISC-V user-interrupt extension (N extension) compatibility.
- Multicore support.
- Supervisor support.
- More memory-mapped I/O regions per zone.
- Inter-processor communication (IPI) - allowing communication with a Linux companion on another core.

*The above list is mostly sorted by priority.*
## Example zone implementation
*Coming soon.*

## Attribution
*Coming soon.*
