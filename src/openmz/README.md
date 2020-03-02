# OpenMZ kernel
To compile the kernel, write the following in bash:
```bash
make CC=[riscv64-unknown-elf-gcc] OBJCOPY=[riscv64-unknown-elf-objcopy] TARGET=[fe310]
# [X] ==> X is default value
```

`TARGET` sets the bsp directory to `bsp/$(TARGET)`.
