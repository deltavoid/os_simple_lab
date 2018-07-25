// See LICENSE for license details.

#define RISCV_PGSHIFT 12
#define RISCV_PGSIZE (1 << RISCV_PGSHIFT)

  .section .text.init,"ax",@progbits
  .align 2
  .globl _start
_start:

  la sp, stacks + RISCV_PGSIZE

  jal boot_loader

  .bss
  .align RISCV_PGSHIFT
stacks:
  .skip RISCV_PGSIZE
