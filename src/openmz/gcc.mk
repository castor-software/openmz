# Copyright 2020, Saab AB
#
# This software may be distributed and modified according to 
# the terms of the GNU General Public License version 2. 
# Note that NO WARRANTY is provided.
# See "LICENSE.GPLv2" for details.
CC=riscv64-unknown-elf-gcc
CFLAGS += -std=c99 -Og -g -mabi=ilp32 -march=rv32imac
