// See LICENSE for license details.

#define RISCV_PGSHIFT 12
#define RISCV_PGSIZE (1 << RISCV_PGSHIFT)
#define REGBYTES 8

  .section .text.init,"ax",@progbits
  .align 2
  .globl _start
_start:

  la sp, stacks + RISCV_PGSIZE

  j boot_loader


  .macro SAVE_ALL
    csrw mscratch, sp
    addi sp, sp, -34 * REGBYTES
    sw x0, 0*REGBYTES(sp)
    sw x1, 1*REGBYTES(sp)
    sw x3, 3*REGBYTES(sp)
    sw x4, 4*REGBYTES(sp)
    sw x5, 5*REGBYTES(sp)
    sw x6, 6*REGBYTES(sp)
    sw x7, 7*REGBYTES(sp)
    sw x8, 8*REGBYTES(sp)
    sw x9, 9*REGBYTES(sp)
    sw x10, 10*REGBYTES(sp)
    sw x11, 11*REGBYTES(sp)
    sw x12, 12*REGBYTES(sp)
    sw x13, 13*REGBYTES(sp)
    sw x14, 14*REGBYTES(sp)
    sw x15, 15*REGBYTES(sp)
    sw x16, 16*REGBYTES(sp)
    sw x17, 17*REGBYTES(sp)
    sw x18, 18*REGBYTES(sp)
    sw x19, 19*REGBYTES(sp)
    sw x20, 20*REGBYTES(sp)
    sw x21, 21*REGBYTES(sp)
    sw x22, 22*REGBYTES(sp)
    sw x23, 23*REGBYTES(sp)
    sw x24, 24*REGBYTES(sp)
    sw x25, 25*REGBYTES(sp)
    sw x26, 26*REGBYTES(sp)
    sw x27, 27*REGBYTES(sp)
    sw x28, 28*REGBYTES(sp)
    sw x29, 29*REGBYTES(sp)
    sw x30, 30*REGBYTES(sp)
    sw x31, 31*REGBYTES(sp)
    csrrw s0, mscratch, 0
    csrr s1, mstatus
    csrr s2, mepc
    sw s0, 2*REGBYTES(sp)
    sw s1, 32*REGBYTES(sp)
    sw s2, 33*REGBYTES(sp)
  .endm
  .macro RESTORE_ALL
    # Restore all of the registers.
    lw s1, 32*REGBYTES(sp)
    lw s2, 33*REGBYTES(sp)
    csrw mstatus, s1
    csrw mepc, s2
    lw x1, 1*REGBYTES(sp)
    lw x3, 3*REGBYTES(sp)
    lw x4, 4*REGBYTES(sp)
    lw x5, 5*REGBYTES(sp)
    lw x6, 6*REGBYTES(sp)
    lw x7, 7*REGBYTES(sp)
    lw x8, 8*REGBYTES(sp)
    lw x9, 9*REGBYTES(sp)
    lw x10,10*REGBYTES(sp)
    lw x11,11*REGBYTES(sp)
    lw x12,12*REGBYTES(sp)
    lw x13,13*REGBYTES(sp)
    lw x14,14*REGBYTES(sp)
    lw x15,15*REGBYTES(sp)
    lw x16,16*REGBYTES(sp)
    lw x17,17*REGBYTES(sp)
    lw x18,18*REGBYTES(sp)
    lw x19,19*REGBYTES(sp)
    lw x20,20*REGBYTES(sp)
    lw x21,21*REGBYTES(sp)
    lw x22,22*REGBYTES(sp)
    lw x23,23*REGBYTES(sp)
    lw x24,24*REGBYTES(sp)
    lw x25,25*REGBYTES(sp)
    lw x26,26*REGBYTES(sp)
    lw x27,27*REGBYTES(sp)
    lw x28,28*REGBYTES(sp)
    lw x29,29*REGBYTES(sp)
    lw x30,30*REGBYTES(sp)
    lw x31,31*REGBYTES(sp)
    lw x2, 2*REGBYTES(sp)
    
  .endm

  .globl __alltraps
__alltraps:
  SAVE_ALL

  mv a0,sp
  jal trap

  RESTORE_ALL
  mret

  .bss
  .align RISCV_PGSHIFT
stacks:
  .skip RISCV_PGSIZE
